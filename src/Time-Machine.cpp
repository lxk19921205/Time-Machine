//============================================================================
// Name        : Time-Machine.cpp
// Author      : Andriy, 590, WJP
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "./../head/ui_views/main-window.h"
#include "../head/logic/daemon_controller.h"

using namespace std;


void init_main_window();


const char* START_CMD = "start";
const char* STOP_CMD = "stop";
const char* POSTPONE_CMD = "postpone";
const char* STATUS_CMD = "status";


/**
 * 在console打印帮助
 */
void show_help()
{
	cout << "帮助：" << endl
			<< "  没有参数：启动图形界面" << endl
			<< "  start：开始服务" << endl
			<< "  stop: 结束服务" << endl
			<< "  postpone：推迟休息" << endl;
}

/**
 * 开始在后台的服务
 */
void start_service()
{
	//	CDaemonController daemon_controller;
	//	daemon_controller.init_daemon();
	//TODO
	cout << "start service" << endl;
}

/**
 * 停止在后台的服务
 */
void stop_service()
{
	//TODO
	cout << "stop service" << endl;
}

/**
 * 推迟休息
 */
void postpone_rest()
{
	//TODO
	cout << "postpone service" << endl;
}

/**
 * 显示休息相关的参数
 */
void show_status()
{
	//TODO
	cout << "show status" << endl;
}


int main(int argc, char** argv)
{
	switch (argc)
	{
	case 1:
	{
		//=====开启GUI=====
		gtk_init(&argc, &argv);
		init_main_window();
		exit(0);
		return 0;
	}
	case 2:
	{
		if (strcmp(argv[1], START_CMD) == 0)
		{
			start_service();
		}
		else if (strcmp(argv[1], STOP_CMD) == 0)
		{
			stop_service();
		}
		else if (strcmp(argv[1], POSTPONE_CMD) == 0)
		{
			postpone_rest();
		}
		else if (strcmp(argv[1], STATUS_CMD) == 0)
		{
			show_status();
		}
		else
		{
			show_help();
		}
		break;
	}

	default:
		show_help();
		exit(1);
	}
}

/**
 * 初始化主界面设置
 */
void init_main_window() {
	GtkWidget* window;
	CMainWindow mainWindow(window);
}

