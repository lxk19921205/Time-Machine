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
			//=====start service=====
			//	CDaemonController daemon_controller;
			//	daemon_controller.init_daemon();
			//  TODO
		}
		else if (strcmp(argv[1], STOP_CMD) == 0)
		{
			//=====stop service=====
			//TODO
		}
		break;
	}

	default:
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

