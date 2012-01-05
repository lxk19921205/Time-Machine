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

#include "../../head/logic/daemon_controller.h"


const char* CDaemonController::LOG_NAME = "-----Time Machine d-----";
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
		//parent
		exit(0);
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
	openlog(LOG_NAME, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "Initializing error: unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}


bool CDaemonController::already_running()
{
	int fd = open(TM_LOCK_FILE, O_RDWR | O_CREAT, TM_LOCK_MODE);
	if (fd < 0)
	{
		syslog(LOG_ERR, "can't open %s : %s", TM_LOCK_FILE, strerror(errno));
		exit(1);
	}

	if (this->lock_file(fd) < 0)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return true;
		}

		syslog(LOG_ERR, "can't lock %s : %s", TM_LOCK_FILE, strerror(errno));
		exit(1);
	}

	ftruncate(fd, 0);
	char buffer[16];
	sprintf(buffer, "%ld", (long) getpid());
	write(fd, buffer, strlen(buffer) + 1);
	return false;
}


bool CDaemonController::lock_file(int fd)
{
	return true;
}

