/*
 * whip_controller.cpp
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>

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
	this->start_waiting();
	exit(0);
}

void CWhipController::start_waiting()
{
	this->lock_file();
	while (true)
	{
		pause();
	}
}
