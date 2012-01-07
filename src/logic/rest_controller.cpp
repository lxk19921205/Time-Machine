/*
 * rest_controller.cpp
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

#include <iostream>

#include "../../head/logic/rest_controller.h"
#include "../../head/Persistence/persistence_controller.h"

using namespace std;

CRestController::CRestController()
	: CAbsProcController("/var/run/TimeMachineRest.pid")
{
}

CRestController::~CRestController()
{
}

void CRestController::init_process()
{
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error in init_rest_child()");
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
		syslog(LOG_ERR, "try to start up rest process again when there is one running!");
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
		CRestController restController;
		restController.read_setting();
		restController.do_rest();
	}
}


void CRestController::set_signal()
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGALRM, &sa, NULL);
}


void CRestController::start_waiting()
{
	while (true)
	{
		struct timeval interval;
		interval.tv_sec = setting.restIntervalTime * 60;
		interval.tv_usec = 0;

		//TODO testing
		interval.tv_sec = 10;

		struct itimerval value;
		value.it_interval = interval;
		value.it_value = interval;

		if (setitimer(ITIMER_REAL, &value, NULL) == -1)
		{
			//error
			syslog(LOG_ERR, "rest timer error: %s", strerror(errno));
			exit(1);
		}
		pause();
	}
}


void CRestController::do_rest()
{
	//TODO 黑屏、放歌、锁键盘

	//TODO BEEP

	if (setting.ifCloseScreen)
	{
		this->turn_off_screen();
	}


}

void CRestController::turn_off_screen()
{
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error in do_rest()");
		exit(1);
	}
	else if (pid == 0)
	{
		sleep(1);
		execlp("xset","", "dpms", "force", "off", NULL);
		exit(0);
	}
}
