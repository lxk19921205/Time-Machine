/*
 * whip_controller.cpp
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#include <unistd.h>

#include "../../head/logic/whip_controller.h"


void CWhipController::start_waiting()
{
	while (true)
	{
		pause();
	}
}
