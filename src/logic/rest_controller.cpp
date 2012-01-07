/*
 * rest_controller.cpp
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#include <unistd.h>

#include "../../head/logic/rest_controller.h"


CRestController::CRestController()
	: go_on_waiting(true)
{

}

void CRestController::start_waiting()
{
	while (true)
	{
		pause();
	}
}

void CRestController::do_rest()
{

}
