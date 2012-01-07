/*
 * daemon_controller.cpp
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
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
#include "../../head/logic/whip_controller.h"

using namespace std;

const char* CDaemonController::TM_LOG_NAME = "-----Time Machine d-----";


CDaemonController::CDaemonController()
	: CAbsProcController("/var/run/TimeMachine.pid")
{
}

CDaemonController::~CDaemonController()
{
}

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

	//TODO sigaction，处理SIGUSER1，结束all

	while(true)
	{
		pause();
	}

	exit(0);
}


void CDaemonController::init_rest_child()
{
	//=====[0] read, [1] write=====
//	int fds[2];
//	pipe(fds);
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error in init_rest_child()");
		return;
	}
	else if (pid == 0)
	{
		//child
//		close(fds[0]);
//		pid_t child_pid = getpid();
//		write(fds[1], &child_pid, sizeof(pid_t));

		CRestController restController;
		restController.start_waiting();
		exit(0);
	}
	else
	{
		//parent
//		close(fds[1]);
//		read(fds[0], &rest_pid, sizeof(pid_t));
//		syslog(LOG_INFO, "rest_pid %d", rest_pid);
		return;
	}
}

void CDaemonController::init_whip_child()
{
	//=====[0] read, [1] write=====
//	int fds[2];
//	pipe(fds);
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error in init_whip_child()");
		return;
	}
	else if (pid == 0)
	{
		//child
//		close(fds[0]);
//		pid_t child_pid = getpid();
//		write(fds[1], &child_pid, sizeof(pid_t));

		syslog(LOG_INFO, "prepare to whip waiting");
		CWhipController whipController;
		whipController.start_waiting();
		exit(0);
	}
	else
	{
		//parent
//		close(fds[1]);
//		read(fds[0], &whip_pid, sizeof(pid_t));
//		syslog(LOG_INFO, "whip_pid %d", whip_pid);
		return;
	}
}

void CDaemonController::kill_rest_child()
{
//	int status;
//	syslog(LOG_INFO, "try to kill rest_child %d", rest_pid);
//	kill(rest_pid, SIGKILL);
//	wait(&status);
	//TODO
}


