/*
 * rest_controller.cpp
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>

#include "../../head/logic/rest_controller.h"
#include "../../head/Persistence/persistence_controller.h"


CRestController::CRestController()
	: CAbsProcController("/var/run/TimeMachineRest.pid"),
	  go_on_waiting(true)
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



	this->start_waiting();


	exit(0);
}

void CRestController::start_waiting()
{
	this->lock_file();
	while (true)
	{
		pause();
	}
}

void CRestController::do_rest()
{
	//TODO
}
