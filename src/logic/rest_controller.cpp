/*
 * rest_controller.cpp
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#include <unistd.h>

#include "../../head/logic/rest_controller.h"


const char* CRestController::TM_REST_LOCK_FILE = "/var/run/TimeMachineRest.pid";
mode_t CRestController::TM_REST_LOCK_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;


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


pid_t CRestController::get_unique_pid()
{
	int fd = open(TM_REST_LOCK_FILE, O_RDONLY, TM_REST_LOCK_MODE);
	if (fd < 0)
	{
		return -1;
	}

	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	fcntl(fd, F_GETLK, &fl);
	//=====如果不存在锁，则将l_type设为F_UNLCK，其余信息不变=====
	close(fd);
	if (fl.l_type == F_UNLCK)
	{
		return -1;
	}
	else
	{
		return fl.l_pid;
	}
}
