/*
 * daemon_controller.h
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#ifndef DAEMON_CONTROLLER_H_
#define DAEMON_CONTROLLER_H_

#include "abstract_process_controller.h"

class CDaemonController : public CAbsProcController
{
public:
	CDaemonController();
	~CDaemonController();

	/**
	 * 初始化daemon process，之后此进程就一直默默地工作着
	 * 如果失败的话，此进程直接exit！
	 */
	void init_daemon();

	void kill_rest_child();

private:
	/**
	 * 在daemon process结束的时候解锁文件
	 * 但现在不许要unlock了，因为进程结束自动释放，而进程还在的时候释放没有意义
	 * 所以直接关进程好了
	 */
	//void unlock_file();

private:
	static const char* TM_LOG_NAME;

	/**
	 * 初始化检测休息子进程
	 */
	void init_rest_child();
	/**
	 * 初始化检测激励子进程
	 */
	void init_whip_child();
};


#endif /* DAEMON_CONTROLLER_H_ */
