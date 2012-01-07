/*
 * daemon_controller.cpp
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <iostream>

#include "../../head/logic/daemon_controller.h"
#include "../../head/logic/rest_controller.h"

using namespace std;

const char* CDaemonController::TM_LOG_NAME = "-----Time Machine d-----";
const char* CDaemonController::TM_LOCK_FILE = "/var/run/TimeMachine.pid";
mode_t CDaemonController::TM_LOCK_MODE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);


void CDaemonController::init_daemon()
{
	//step 1
	umask(0);

	struct rlimit rl;
	getrlimit(RLIMIT_NOFILE, &rl);

	pid_t pid = fork();
	if (pid < 0)
	{
		//fork()失败了
		exit(1);
	}
	else if (pid > 0)
	{
		//parent，这里不exit，因为是从外边调的，外边可能还有事情要做
		return;
	}
	//是child process了现在

	setsid();

	//确保将来的open不会分配控制着的tty
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGHUP, &sa, NULL);
	pid = fork();
	if (pid < 0)
	{
		//fork()失败了
		exit(1);
	}
	else if (pid > 0)
	{
		//parent process
		exit(0);
	}

	//改变当前目录到根目录
	chdir("/");

	//关闭所有打开的file descriptors
	if (rl.rlim_max == RLIM_INFINITY)
	{
		rl.rlim_max = 1024;
	}

	//显式的一定要关闭的3个fd，与for循环里也许会重复，无所谓～
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	for (unsigned int i=0; i<rl.rlim_max;i++)
	{
		close(i);
	}

	//把fd 0，1，2和/dev/null关联起来
	int fd0 = open("/dev/null", O_RDWR);	//fd0应该==0
	int fd1 = dup(fd0);	//fd1应该==1
	int fd2 = dup(fd0);	//fd2应该==2

	//init Log and check fds' correctness
	openlog(TM_LOG_NAME, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "Initializing error: unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}

	//=====测试是否唯一运行=====
	if (this->lock_file() == false)
	{
		//=====不是唯一运行的=====
		syslog(LOG_INFO, "try to start up again when there is one service running!");
		exit(1);
	}

	this->init_rest_child();
	this->init_whip_child();

	while(true)
	{

		pause();
	}

	exit(0);
}


void CDaemonController::init_rest_child()
{
	//=====[0] read, [1] write=====
	int fds[2];
	pipe(fds);
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error when init_rest_child");
		return;
	}
	else if (pid == 0)
	{
		//child
		close(fds[0]);
		pid_t child_pid = getpid();
		write(fds[1], &child_pid, sizeof(pid_t));

		//TODO
		CRestController restController();
		exit(0);
	}
	else
	{
		//parent
		close(fds[1]);
		read(fds[0], &this->rest_pid, sizeof(pid_t));
		syslog(LOG_INFO, "rest_pid %d", rest_pid);
		return;
	}
}

void CDaemonController::init_whip_child()
{
	//=====[0] read, [1] write=====
	int fds[2];
	pipe(fds);
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error when init_whip_child");
		return;
	}
	else if (pid == 0)
	{
		//child
		close(fds[0]);
		pid_t child_pid = getpid();
		write(fds[1], &child_pid, sizeof(pid_t));

		//TODO
		exit(0);
	}
	else
	{
		//parent
		close(fds[1]);
		read(fds[0], &this->whip_pid, sizeof(pid_t));
		syslog(LOG_INFO, "whip_pid %d", whip_pid);
		return;
	}
}


//=====public method=====
bool CDaemonController::already_running()
{
	return this->get_unique_pid() != -1;
}


//=====public method=====
pid_t CDaemonController::get_unique_pid()
{
	int fd = open(TM_LOCK_FILE, O_RDONLY, TM_LOCK_MODE);
	if (fd < 0)
	{
		return -1;
	}

	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	fcntl(fd, F_GETLK, &fl);
	//=====如果不存在锁，则将l_type设为F_UNLCK，其余信息不变=====
	close(fd);
	if (fl.l_type == F_UNLCK)
	{
		return -1;
	}
	else
	{
		return fl.l_pid;
	}
}


//=====private method=====
bool CDaemonController::lock_file()
{
	int fd = open(TM_LOCK_FILE, O_RDWR | O_CREAT, TM_LOCK_MODE);
	if (fd < 0)
	{
		syslog(LOG_ERR, "can't open %s : %s in lock_file()", TM_LOCK_FILE, strerror(errno));
		exit(1);
	}

	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	if (fcntl(fd, F_SETLK, &fl) == -1)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			//无法锁住文件，已经被锁住了，说明已经有一个程序在运行了
			close(fd);
			return false;
		}
		else
		{
			syslog(LOG_ERR, "can't lock %s : %s in lock_file()", TM_LOCK_FILE, strerror(errno));
			exit(1);
		}
	}

	//=====可以锁，写入pid=====
	ftruncate(fd, 0);
	char buffer[16];
	sprintf(buffer, "%ld", (long) getpid());
	write(fd, buffer, strlen(buffer) + 1);

	//=====不需要close，否则锁就没了！进程结束自动close=====
//	close(fd);

	return true;
}


//void CDaemonController::unlock_file()
//{
//	int fd = open(TM_LOCK_FILE, O_RDWR | O_CREAT, TM_LOCK_MODE);
//	if (fd < 0)
//	{
//		syslog(LOG_ERR, "can't open %s : %s in unlock_file()", TM_LOCK_FILE, strerror(errno));
//		exit(1);
//	}
//
//	struct flock fl;
//	fl.l_type = F_UNLCK;
//	fl.l_start = 0;
//	fl.l_whence = SEEK_SET;
//	fl.l_len = 0;
//
//	if (fcntl(fd, F_SETLK, &fl) == -1)
//	{
//		syslog(LOG_ERR, "can't unlock file %s : %s in unlock_file()", TM_LOCK_FILE, strerror(errno));
//		exit(1);
//	}
//	close(fd);
//}

