/*
 * abstract_process_controller.cpp
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../head/logic/abstract_process_controller.h"


mode_t CAbsProcController::TM_LOCK_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;


CAbsProcController::CAbsProcController(const char* file)
	: TM_LOCK_FILE(file)
{
}

CAbsProcController::~CAbsProcController()
{
}

pid_t CAbsProcController::get_unique_pid()
{
//	syslog(LOG_INFO, "start to get pid of %s", TM_LOCK_FILE);
	int fd = open(TM_LOCK_FILE, O_RDONLY, TM_LOCK_MODE);
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

bool CAbsProcController::already_running()
{
	return this->get_unique_pid() != -1;
}


bool CAbsProcController::lock_file()
{
//	syslog(LOG_INFO, "start to lock file of %s", TM_LOCK_FILE);
	int fd = open(TM_LOCK_FILE, O_RDWR | O_CREAT, TM_LOCK_MODE);
	if (fd < 0)
	{
		syslog(LOG_ERR, "can't open %s : %s in lock_file()", TM_LOCK_FILE, strerror(errno));
		exit(1);
	}

	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;

	if (fcntl(fd, F_SETLK, &fl) == -1)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			syslog(LOG_INFO, "LOCKING ALREADY RUNNING %s", TM_LOCK_FILE);
			//无法锁住文件，已经被锁住了，说明已经有一个程序在运行了
			close(fd);
			return false;
		}
		else
		{
			syslog(LOG_ERR, "can't lock %s : %s in lock_file()", TM_LOCK_FILE, strerror(errno));
			exit(1);
		}
	}

	//=====可以锁，写入pid=====
	ftruncate(fd, 0);
	char buffer[16];
	sprintf(buffer, "%ld", (long) getpid());
	write(fd, buffer, strlen(buffer) + 1);

	//=====不需要close，否则锁就没了！进程结束自动close=====
//	close(fd);

	return true;
}


void CAbsProcController::read_setting()
{
	PersistenceController persistence;
	persistence.readUserData(&setting);
}
