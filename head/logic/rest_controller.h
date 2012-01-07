/*
 * rest_controller.h
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#ifndef REST_CONTROLLER_H_
#define REST_CONTROLLER_H_

#include <unistd.h>

#include "abstract_process_controller.h"

/**
 * 控制休息
 */
class CRestController : public CAbsProcController
{
public:
	CRestController();
	~CRestController();

	virtual void init_process();

	/**
	 * 开始等待下一次休息
	 */
	void start_waiting();
	/**
	 * 休息了！！！
	 */
	void do_rest();

private:
	bool go_on_waiting;
};


#endif /* REST_CONTROLLER_H_ */
