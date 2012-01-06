/*
 * persistence_controller.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: 590
 */

#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

#include "../../head/Persistence/persistence_controller.h"

using namespace std;

#define BUFFSIZE 4096
string file = "/home/jolin/Desktop/user_data.txt";
const char* file_path = file.c_str();

int str2int(string str)
{
	istringstream str2int(str);
	int num;
	str2int >> num;
	return num;
}

UserData analyseString(char* buff)
{
	UserData result;

	//analyse string
	stringstream temp(buff);
	string sub_str;
	while(getline(temp,sub_str,'\n'))
	{
		stringstream eachLine(sub_str);
		string head;
		string value;
		getline(eachLine,head,':');
		getline(eachLine,value,':');
		if(head.compare("intervalTime") == 0)
			result.intervalTime = str2int(value);
		else if(head.compare("delayTime") == 0)
			result.delayTime = str2int(value);
		else if(head.compare("lockTime") == 0)
			result.lockTime = str2int(value);
		else if(head.compare("unLockTime") == 0)
			result.unLockTime = str2int(value);
		else if(head.compare("canDelay") == 0)
			result.canDelay = str2int(value);
		else if(head.compare("canForceToExit") == 0)
			result.canForceToExit = str2int(value);
		else if(head.compare("ifBeep") == 0)
			result.ifBeep = str2int(value);
		else if(head.compare("ifCloseScreen") == 0)
			result.ifCloseScreen = str2int(value);
		else if(head.compare("ifStartWithPower") == 0)
			result.ifStartWithPower = str2int(value);
		else if(head.compare("ifStopTiming") == 0)
			result.ifStopTiming = str2int(value);
		else if(head.compare("userLevel") == 0)
		{
			int userLevel = str2int(value);
			level t;
			if(userLevel == 0)
				t = remind;
			else if(userLevel == 1)
				t = remind_force;
			else if(userLevel == 2)
				t = force;

			result.userLevel = t;
		}
	}
	return result;
}

UserData PersistenceController::readUserData()
{
	UserData result;
	int fd;
	char buff[BUFFSIZE];

	if((fd = open(file_path, O_RDONLY)) < 0)
	{
		cout <<"open file failed!"<< endl;
	}
	if(read(fd, buff, BUFFSIZE) < 0)
	{
		cout <<"read file failed!"<< endl;
	}

	result = analyseString(buff);

	close(fd);
	return result;
}

void writeLineByInt(int fd, string head, int aim)
{
	string tail = "\n";

	//store head
	char userTemp[10];

	//transform int to string
	sprintf(userTemp, "%d", aim);
	string totalString = head + userTemp + tail;

	const char* total = totalString.c_str();
	int length = totalString.length();
	if(write(fd, total, length) != length)
	{
		cout <<"write file failed!"<< endl;
	}
}

void PersistenceController::writeUserData(UserData userData)
{
	int fd;

	if((fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH)) < 0)
	{
		cout <<"open file failed!"<< endl;
	}

	writeLineByInt(fd, "intervalTime:", userData.intervalTime);
	writeLineByInt(fd, "lockTime:", userData.lockTime);
	writeLineByInt(fd, "unLockTime:", userData.unLockTime);
	writeLineByInt(fd, "delayTime:", userData.delayTime);
	writeLineByInt(fd, "ifStartWithPower:", userData.ifStartWithPower);
	writeLineByInt(fd, "ifBeep:", userData.ifBeep);
	writeLineByInt(fd, "ifCloseScreen:", userData.ifCloseScreen);
	writeLineByInt(fd, "ifStopTiming:", userData.ifStopTiming);
	writeLineByInt(fd, "canForceToExit:", userData.canForceToExit);
	writeLineByInt(fd, "canDelay:", userData.canDelay);
	writeLineByInt(fd, "level:", userData.userLevel);

	close(fd);
}

