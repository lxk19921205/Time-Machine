/*
 * daemon_controller.h
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#ifndef DAEMON_CONTROLLER_H_
#define DAEMON_CONTROLLER_H_

#include <fcntl.h>


class CDaemonController
{
public:
	/**
	 * 初始化daemon process，之后此进程就一直默默地工作着
	 * 如果失败的话，此进程直接exit！
	 */
	void init_daemon();
	/**
	 * 测试此daemon process是否已经运行
	 */
	bool already_running();

private:
	/**
	 * 如果返回true，说明文件可以锁，因此此程序是当前唯一
	 */
	bool lock_file(int fd);

private:
	static const char* TM_LOG_NAME;
	static const char* TM_LOCK_FILE;
	static mode_t TM_LOCK_MODE;
};


#endif /* DAEMON_CONTROLLER_H_ */
