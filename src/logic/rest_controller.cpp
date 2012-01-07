/*
 * rest_controller.cpp
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#include <unistd.h>

#include "../../head/logic/rest_controller.h"


CRestController::CRestController(pid_t parent_pid)
	: go_on_waiting(true),
	  daemon_pid(parent_pid)
{

}

void CRestController::do_rest()
{

}
