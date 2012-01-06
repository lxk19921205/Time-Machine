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
	remind,remind_force,force
};

/**
 * use for store user data
 */
struct UserData {
	int intervalTime;
	int lockTime;
	int unLockTime;
	int delayTime;

	bool ifStartWithPower;
	bool ifBeep;
	bool ifCloseScreen;
	bool ifStopTiming;
	bool canForceToExit;
	bool canDelay;

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
