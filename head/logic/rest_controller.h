/*
 * rest_controller.h
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#ifndef REST_CONTROLLER_H_
#define REST_CONTROLLER_H_

#include <unistd.h>
#include <fcntl.h>

/**
 * 控制休息
 */
class CRestController
{
public:
	CRestController();
	/**
	 * 开始等待下一次休息
	 */
	void start_waiting();
	/**
	 * 休息了！！！
	 */
	void do_rest();

	/**
	 * 拿到全局唯一运行的进程的pid，没运行返回-1
	 */
	pid_t get_unique_pid();

private:
	bool go_on_waiting;

	static const char* TM_REST_LOCK_FILE;
	static mode_t TM_REST_LOCK_MODE;
};


#endif /* REST_CONTROLLER_H_ */
