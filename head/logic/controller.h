/*
 * controller.h
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

/**
 * 总的管理控制的类，管理console的输出和服务的启动
 */
class CController {
public:
	/**
	 * 开始在后台的服务
	 */
	void start_service();

	/**
	 * 停止在后台的服务
	 */
	void stop_service();

	/**
	 * 推迟休息
	 */
	void postpone_rest();

	/**
	 * 显示休息相关的参数
	 */
	void show_status();

	/**
	 * 在console打印帮助
	 */
	void show_help();

private:

};

#endif /* CONTROLLER_H_ */
