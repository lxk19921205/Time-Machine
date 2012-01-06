/*
 * controller.cpp
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#include <iostream>

#include "../../head/logic/controller.h"

using namespace std;

void CController::start_service()
{
	//	CDaemonController daemon_controller;
	//	daemon_controller.init_daemon();
	//TODO
	cout << "start service" << endl;
}


void CController::stop_service()
{
	//TODO
	cout << "stop service" << endl;
}


void CController::postpone_rest()
{
	//TODO
	cout << "postpone service" << endl;
}


void CController::show_status()
{
	//TODO
	cout << "show status" << endl;
}


void CController::show_help()
{
	cout << "帮助：  (请使用超级权限运行)" << endl
			<< "  没有参数：启动图形界面" << endl
			<< "  start：开始服务" << endl
			<< "  stop: 结束服务" << endl
			<< "  postpone：推迟休息" << endl;
}
