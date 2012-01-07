/*
 * daemon_controller.h
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#ifndef DAEMON_CONTROLLER_H_
#define DAEMON_CONTROLLER_H_

#include "abstract_process_controller.h"

#define SIG_STOP_ALL SIGUSR1

class CDaemonController : public CAbsProcController
{
public:
	CDaemonController();
	~CDaemonController();

	virtual void init_process();

private:
	/**
	 * 在daemon process结束的时候解锁文件
	 * 但现在不许要unlock了，因为进程结束自动释放，而进程还在的时候释放没有意义
	 * 所以直接关进程好了
	 */
	//void unlock_file();

	void set_signal();

private:
	static const char* TM_LOG_NAME;

};


#endif /* DAEMON_CONTROLLER_H_ */
