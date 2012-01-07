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

	/**
	 * 关闭屏幕！
	 */
	void turn_off_screen();

private:
	void set_signal();

	/**
	 * 判断现在是不是晚上 22:00 ~ 6:00
	 */
	bool is_night();

	/**
	 * 设置是否可以alt，比如alt+f4，用来休息的时候避免被关掉，及移开tty
	 * 通过xmodmap的方法设置的
	 */
public:	void set_alt_enable(bool enabled);
};


#endif /* REST_CONTROLLER_H_ */
