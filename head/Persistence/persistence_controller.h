/*
 * persistence_controller.h
 *
 *  Created on: 2012-1-5
 *      Author: 590
 */

#ifndef PERSISTENCE_CONTROLLER
#define PERSISTENCE_CONTROLLER

#include <fcntl.h>

enum level{
	//=====只提醒，因为用户自制力很强=====
	remind,

	//=====提醒，过一会儿强制休息=====
	remind_force,

	//=====不提醒，直接强制休息=====
	force
};

/**
 * use for store user data
 */
struct UserData {
	//=====休息间隔 单位：分钟=====
	int intervalTime;

	//=====休息锁定的时间 单位：分钟=====
	int lockTime;

	//=====空闲几分钟暂停计时 单位：分钟=====
	int unLockTime;

	//=====可以推迟几分钟=====
	int delayTime;

	//=====是否开机启动=====
	bool ifStartWithPower;

	//=====休息开始结束是否beep=====
	bool ifBeep;

	//=====休息是否关闭屏幕=====
	bool ifCloseScreen;

	//=====全屏是否暂停计时=====
	bool ifStopTiming;

	//=====休息时是否允许强制退出=====
	bool canForceToExit;

	//=====是否允许推迟休息=====
	bool canDelay;

	//=====用户设置的自制力等级，等级不同表现不同=====
	level userLevel;
};

/**
 * use for store user_data
 */
class PersistenceController {
public:
	void readUserData(UserData* data);
	void writeUserData(UserData& userData);
};

#endif
