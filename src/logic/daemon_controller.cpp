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

void CDaemonController::init_process()
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
		syslog(LOG_ERR, "try to start up daemon process again when there is one running!");
		exit(1);
	}

	CRestController restController;
	restController.init_process();

	CWhipController whipController;
	whipController.init_process();

	this->set_signal();

	while(true)
	{
		pause();
	}

	exit(0);
}


static void signal_handler(int signum, siginfo_t *info, void* context)
{
	if (signum == SIG_STOP_ALL)
	{
		CRestController rest;
		CWhipController whip;
		pid_t rpid = rest.get_unique_pid();
		if (rpid != -1)
		{
			kill(rpid, SIGKILL);
			waitpid(rpid, NULL, 0);
		}
		pid_t wpid = whip.get_unique_pid();
		if (wpid != -1)
		{
			kill(wpid, SIGKILL);
			waitpid(wpid, NULL, 0);
		}
		exit(0);
	}
}


void CDaemonController::set_signal()
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_sigaction = signal_handler;
	sigaction(SIG_STOP_ALL, &sa, NULL);
}

