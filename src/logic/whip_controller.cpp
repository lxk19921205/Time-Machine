/*
 * whip_controller.cpp
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include "../../head/logic/whip_controller.h"


CWhipController::CWhipController()
	: CAbsProcController("/var/run/TimeMachineWhip.pid")
{
}

CWhipController::~CWhipController()
{
}

void CWhipController::init_process()
{
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error in init_whip_child()");
		return;
	}
	else if (pid > 0)
	{
		//parent
		return;
	}

	//child

	if (this->lock_file() == false)
	{
		//=====不是唯一运行的=====
		syslog(LOG_ERR, "try to start up whip process again when there is one running!");
		exit(1);
	}

	this->read_setting();
	this->set_signal();
	this->start_waiting();
	exit(0);
}

static void signal_handler(int signum, siginfo_t *info, void* context)
{
	if (signum == SIGALRM)
	{
		CWhipController whipController;
		whipController.read_setting();
		whipController.do_whip();
	}
}

void CWhipController::set_signal()
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGALRM, &sa, NULL);
}

void CWhipController::start_waiting()
{
	while (true)
	{
		struct timeval interval;
//		interval.tv_sec = setting.intervalTime * 60;
		interval.tv_usec = 0;

		//TODO testing
		interval.tv_sec = 10;

		struct itimerval value;
		value.it_interval = interval;
		value.it_value = interval;

		if (setitimer(ITIMER_REAL, &value, NULL) == -1)
		{
			//error
			syslog(LOG_ERR, "whip timer error: %s", strerror(errno));
			exit(1);
		}
		pause();
	}
}


void CWhipController::do_whip()
{
	//TODO
}
