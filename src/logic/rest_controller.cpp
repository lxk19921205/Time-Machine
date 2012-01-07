/*
 * rest_controller.cpp
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#include <unistd.h>

#include "../../head/logic/rest_controller.h"


CRestController::CRestController()
	: CAbsProcController("/var/run/TimeMachineRest.pid"),
	  go_on_waiting(true)
{
}

CRestController::~CRestController()
{
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

}
