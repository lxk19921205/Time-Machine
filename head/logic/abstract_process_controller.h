/*
 * abstract_process_controller.h
 *
 *  Created on: 2012-1-7
 *      Author: andriy
 */

#ifndef ABSTRACT_PROCESS_CONTROLLER_H_
#define ABSTRACT_PROCESS_CONTROLLER_H_

#include <fcntl.h>
#include "../Persistence/persistence_controller.h"

/**
 * 继承它的都是一些全局唯一的process，
 * 可以用文件锁的方式拿到pid
 */
class CAbsProcController
{
public:
	CAbsProcController(const char* file);
	virtual ~CAbsProcController();

	/**
	 * 拿到唯一运行的那个进程的pid，如果没有在运行，就返回-1
	 */
	virtual pid_t get_unique_pid();

	/**
	 * 测试此process是否已经运行
	 */
	virtual bool already_running();

	/**
	 * 初始化process，之后此进程就一直默默地工作着
	 */
	virtual void init_process() = 0;

	/**
	 * 重新从文件里读取setting
	 */
	void read_setting();

protected:
	const char* TM_LOCK_FILE;
	static mode_t TM_LOCK_MODE;

	UserData setting;

	/**
	 * 如果返回true，说明文件可以锁，那就锁上！因此此程序是当前唯一
	 * 否则，说明文件已被锁住，已经有另一个实例在运行了
	 */
	virtual bool lock_file();

	/**
	 * 关联相关的信号处理
	 */
	virtual void set_signal() = 0;
};


#endif /* ABSTRACT_PROCESS_CONTROLLER_H_ */
