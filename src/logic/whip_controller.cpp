/*
 * whip_controller.cpp
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#include <unistd.h>

#include "../../head/logic/whip_controller.h"


CWhipController::CWhipController()
	: CAbsProcController("/var/run/TimeMachineWhip.pid")
{
}

CWhipController::~CWhipController()
{
}

void CWhipController::start_waiting()
{
	this->lock_file();
	while (true)
	{
		pause();
	}
}
