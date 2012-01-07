/*
 * controller.cpp
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#include <iostream>
#include <signal.h>
#include <sys/wait.h>

#include "../../head/logic/controller.h"
#include "../../head/logic/daemon_controller.h"

using namespace std;

void CController::start_service()
{
	CDaemonController daemon;
	if (daemon.already_running())
	{
		cout << "时光机已经在运行了哦" << endl;
	}
	else
	{
		daemon.init_process();
		cout << "时光机开始运行！" << endl;
	}
}


void CController::stop_service()
{
	CDaemonController daemon;
	pid_t pid = daemon.get_unique_pid();
	if (pid != -1)
	{
		kill(pid, SIGUSR1);
		int status;
		waitpid(pid, &status, 0);
		cout << "这么快就结束了啊..." << endl;
	}
	else
	{
		cout << "时光机还没有运行呢" << endl;
	}
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
	cout << "帮助：  (===请使用超级权限运行===)" << endl
			<< "  没有参数：启动图形界面" << endl
			<< "  start：开始服务" << endl
			<< "  stop: 结束服务" << endl
			<< "  postpone：推迟休息（敬请期待）" << endl
			<< "  rest：立即休息（敬请期待）" << endl
			<< "  status：显示休息数据（敬请期待）" << endl;
}

void CController::rest_now()
{
	//TODO
	cout << "rest now" << endl;
}
