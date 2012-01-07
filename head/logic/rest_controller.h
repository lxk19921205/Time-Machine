/*
 * rest_controller.h
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#ifndef REST_CONTROLLER_H_
#define REST_CONTROLLER_H_

#include <unistd.h>

class CRestController
{
public:
	CRestController(pid_t parent_pid);
	void start_waiting();
	/**
	 * 休息了！！！
	 */
	void do_rest();
private:
	bool go_on_waiting;
	pid_t daemon_pid;
};


#endif /* REST_CONTROLLER_H_ */
