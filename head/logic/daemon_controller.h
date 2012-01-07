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
	/**
	 * 拿到唯一运行的那个进程的pid，如果没有在运行，就返回-1
	 */
	pid_t get_unique_pid();

	void kill_rest_child();
private:
	/**
	 * 如果返回true，说明文件可以锁，那就锁上！因此此程序是当前唯一
	 * 否则，说明文件已被锁住，已经有另一个实例在运行了
	 */
	bool lock_file();

	/**
	 * 在daemon process结束的时候解锁文件
	 * 但现在不许要unlock了，因为进程结束自动释放，而进程还在的时候释放没有意义
	 * 所以直接关进程好了
	 */
	//void unlock_file();

private:
	static const char* TM_LOG_NAME;
	static const char* TM_LOCK_FILE;
	static mode_t TM_LOCK_MODE;

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
