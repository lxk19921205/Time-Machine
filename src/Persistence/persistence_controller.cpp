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
string file = "/etc/time_machine.conf";
const char* file_path = file.c_str();

int str2int(string str)
{
	istringstream str2int(str);
	int num;
	str2int >> num;
	return num;
}

void analyseString(UserData* userData, char* buff)
{
	//analyse string
	stringstream temp(buff);
	string sub_str;

	//cut the string into specific area
	while(getline(temp,sub_str,'\n'))
	{
		stringstream eachLine(sub_str);
		string head;
		string value;
		getline(eachLine,head,':');
		getline(eachLine,value,':');
		if(head.compare("restIntervalTime") == 0)
			userData->restIntervalTime = str2int(value);
		else if(head.compare("whipIntervalTime") == 0)
			userData->whipIntervalTime = str2int(value);
//		else if(head.compare("delayTime") == 0)
//			userData->delayTime = str2int(value);
		else if(head.compare("whipLastingTime") == 0)
			userData->whipLastingTime = str2int(value);
		else if(head.compare("lockTime") == 0)
			userData->lockTime = str2int(value);
		else if(head.compare("unLockTime") == 0)
			userData->unLockTime = str2int(value);
		else if(head.compare("canDelay") == 0)
			userData->canDelay = str2int(value);
		else if(head.compare("canForceToExit") == 0)
			userData->canForceToExit = str2int(value);
//		else if(head.compare("ifBeep") == 0)
//			userData->ifBeep = str2int(value);
		else if(head.compare("ifCloseScreen") == 0)
			userData->ifCloseScreen = str2int(value);
		else if(head.compare("ifStartWithPower") == 0)
			userData->ifStartWithPower = str2int(value);
//		else if(head.compare("ifStopTiming") == 0)
//			userData->ifStopTiming = str2int(value);
		else if(head.compare("level") == 0)
		{
			int userLevel = str2int(value);
			level t;
			if(userLevel == 0)
				t = remind;
			else if(userLevel == 1)
				t = remind_force;
			else if(userLevel == 2)
				t = force;

			userData->userLevel = t;
		}
		else if(head.compare("restMp3Path") == 0)
			userData->restMp3Path = value;
		else if(head.compare("whipMp3Path") == 0)
			userData->whipMp3Path = value;
		else if(head.compare("customCommand") == 0)
			userData->customCommand = value;
		else if(head.compare("imagePath") == 0)
			userData->imagePath = value;
		else if(head.compare("whipWord") == 0)
			userData->whipWord = value;
	}
}

void writeLineByString(int fd, string head, string aim)
{
	string tail = "\n";
	string totalString = head + aim + tail;

	const char* total = totalString.c_str();
	int length = totalString.length();
	if(write(fd, total, length) != length)
	{
		cout <<"write file failed!"<< endl;
	}
}

void writeLineByInt(int fd, string head, int aim)
{
	if(aim <= 0)
	{
//		cout <<"setting error!"<< endl;
		aim = 3;
	}
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

void PersistenceController::writeUserData(UserData& userData)
{
	int fd;

	if((fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH)) < 0)
	{
		cout <<"open file failed!"<< endl;
	}

	writeLineByInt(fd, "restIntervalTime:", userData.restIntervalTime);
	writeLineByInt(fd, "whipIntervalTime:", userData.whipIntervalTime);
	writeLineByInt(fd, "whipLastingTime:", userData.whipLastingTime);
	writeLineByInt(fd, "lockTime:", userData.lockTime);
	writeLineByInt(fd, "unLockTime:", userData.unLockTime);

	writeLineByInt(fd, "ifStartWithPower:", userData.ifStartWithPower);
	//writeLineByInt(fd, "ifBeep:", userData.ifBeep);
	writeLineByInt(fd, "ifCloseScreen:", userData.ifCloseScreen);
	//writeLineByInt(fd, "ifStopTiming:", userData.ifStopTiming);
	writeLineByInt(fd, "canForceToExit:", userData.canForceToExit);
	writeLineByInt(fd, "canDelay:", userData.canDelay);
	writeLineByInt(fd, "level:", userData.userLevel);

	writeLineByString(fd, "restMp3Path:", userData.restMp3Path);
	writeLineByString(fd, "whipMp3Path:", userData.whipMp3Path);
	writeLineByString(fd, "customCommand:", userData.customCommand);
	writeLineByString(fd, "imagePath:", userData.imagePath);
	writeLineByString(fd, "whipWord:", userData.whipWord);

	close(fd);
}

void PersistenceController::initFile(UserData *initData)
{

	initData->restIntervalTime = 45;
	initData->lockTime = 2;
	initData->unLockTime = 1;
	initData->restMp3Path = "/home/andriy/陈洁仪 - 天冷就回来.mp3";

	initData->ifStartWithPower = true;
	initData->ifCloseScreen = false;
	initData->canDelay = true;
	initData->canForceToExit = false;
	initData->userLevel = remind_force;
	initData->imagePath = "";
	initData->customCommand = "";

	initData->whipIntervalTime = 20;
	initData->whipLastingTime = 30;
	initData->whipWord = "燃烧吧！小宇宙！";
	initData->whipMp3Path = "/home/andriy/陈洁仪 - 天冷就回来.mp3";

	writeUserData(*initData);
}

void PersistenceController::readUserData(UserData *userData)
{
	if (userData == NULL)
	{
		return;
	}

	int fd;
	char buff[BUFFSIZE];

	if((fd = open(file_path, O_RDONLY)) < 0)
	{
		//TODO initialize
		initFile(userData);
		return;
	}
	if(read(fd, buff, BUFFSIZE) < 0)
	{
		cout <<"read file failed!"<< endl;
	}

	analyseString(userData, buff);

	close(fd);
}

