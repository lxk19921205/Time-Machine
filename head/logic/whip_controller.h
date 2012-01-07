/*
 * whip_controller.h
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#ifndef WHIP_CONTROLLER_H_
#define WHIP_CONTROLLER_H_

#include "abstract_process_controller.h"

/**
 * 控制激励
 */
class CWhipController : public CAbsProcController
{
public:
	CWhipController();
	~CWhipController();

	virtual void init_process();

	/**
	 * 开始等待下一次激励！
	 */
	void start_waiting();

	/**
	 * 激励！
	 */
	void do_whip();
private:
	void set_signal();
};


#endif /* WHIP_CONTROLLER_H_ */
