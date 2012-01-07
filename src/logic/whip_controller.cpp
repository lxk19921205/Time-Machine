/*
 * whip_controller.cpp
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#include <unistd.h>

#include "../../head/logic/whip_controller.h"


const char* CWhipController::TM_WHIP_LOCK_FILE = "/var/run/TimeMachineWhip.pid";
mode_t CWhipController::TM_WHIP_LOCK_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;


void CWhipController::start_waiting()
{
	while (true)
	{
		pause();
	}
}


pid_t CWhipController::get_unique_pid()
{
	int fd = open(TM_WHIP_LOCK_FILE, O_RDONLY, TM_WHIP_LOCK_MODE);
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
