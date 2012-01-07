/*
 * whip_controller.h
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#ifndef WHIP_CONTROLLER_H_
#define WHIP_CONTROLLER_H_

#include <fcntl.h>

/**
 * 控制激励
 */
class CWhipController
{
public:
	/**
	 * 开始等待下一次激励！
	 */
	void start_waiting();

	/**
	 * 拿到唯一运行着的whip process的pid，没运行返回-1
	 */
	pid_t get_unique_pid();

private:
	static const char* TM_WHIP_LOCK_FILE;
	static mode_t TM_WHIP_LOCK_MODE;

};


#endif /* WHIP_CONTROLLER_H_ */
