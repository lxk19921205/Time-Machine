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
#include <glib.h>
#include <glade/glade.h>

#include "./../head/ui_views/main-window.h"
#include "./../head/ui_views/setting-window.h"
#include "./../head/ui_views/fullscreen-window.h"
#include "../head/logic/daemon_controller.h"
#include "../head/logic/controller.h"
#include "../head/logic/rest_controller.h"
#include "../head/logic/whip_controller.h"
#include "../head/logic/music_controller.h"

#include <signal.h>

using namespace std;

void init_main_window();
void do_test1();
void do_test2();

const char* START_CMD = "start";
const char* STOP_CMD = "stop";
const char* POSTPONE_CMD = "postpone";
const char* STATUS_CMD = "status";
const char* REST_CMD = "rest";

/**
 * 开始
 */
void on_start_button_clicked(GtkWidget* button, gpointer userdata)
{
	//TODO
	CFullScreenWindow::fullScreenWindow->show_fullscreen_window();
	CFullScreenWindow::fullScreenWindow->set_whip_word("ABC");
	CFullScreenWindow::fullScreenWindow->show_rest_time("120分钟");
//	struct sigaction sa;
//	sigemptyset(&sa.sa_mask);
//	sa.sa_flags = 0;
//	sa.sa_handler = SIG_IGN;
//	sigaction(SIGINT, &sa, NULL);
}

/**
 * 终止
 */
void on_stop_button_clicked(GtkWidget* button, gpointer userdata)
{
	//TODO
}

/**
 * 推迟
 */
void on_delay_button_clicked(GtkWidget* button, gpointer userdata)
{
	//TODO
}

/**
 * 获得用户数据
 */
void get_user_data(GtkWidget* button, gpointer userdata)
{
	UserData data = CSettingWindow::settingWindow->get_user_data();
	PersistenceController persistence;
	persistence.writeUserData(data);

	struct UserData d;
	persistence.readUserData(&d);
	CSettingWindow::settingWindow->read_user_data(d);

	sleep(1);
	exit(0);
}

int main(int argc, char** argv)
{
	CController controller;
	gtk_init(&argc, &argv);
	switch (argc)
	{
	case 1:
	{
		//=====开启GUI=====
		init_main_window();
		g_signal_connect(G_OBJECT(CMainWindow::mainWindow->get_start_button()), "clicked",
				G_CALLBACK(on_start_button_clicked), (gpointer)"");
		g_signal_connect(G_OBJECT(CMainWindow::mainWindow->get_stop_button()), "clicked",
				G_CALLBACK(on_stop_button_clicked), (gpointer)"");
		g_signal_connect(G_OBJECT(CMainWindow::mainWindow->get_delay_button()), "clicked",
				G_CALLBACK(on_delay_button_clicked), (gpointer)"");

		g_signal_connect(G_OBJECT(CSettingWindow::settingWindow->get_save_button()), "clicked",
				G_CALLBACK(get_user_data), (gpointer)"");
		gtk_main();
		return 0;
	}
	case 2:
	{
		if (strcmp(argv[1], "test1") == 0)
		{
			do_test1();
			return 0;
		}
		if (strcmp(argv[1], "test2") == 0)
		{
			do_test2();
			return 0;
		}

		if (strcmp(argv[1], START_CMD) == 0)
		{
			//=====开启后台服务=====
			controller.start_service();
			return 0;
		}
		if (strcmp(argv[1], STOP_CMD) == 0)
		{
			//=====停止后台服务=====
			controller.stop_service();
			return 0;
		}
		if (strcmp(argv[1], POSTPONE_CMD) == 0)
		{
			//=====推迟休息=====
			controller.postpone_rest();
			return 0;
		}
		if (strcmp(argv[1], STATUS_CMD) == 0)
		{
			//=====显示休息参数=====
			controller.show_status();
			return 0;
		}
		if (strcmp(argv[1], REST_CMD) == 0)
		{
			//=====现在休息=====
			controller.rest_now();
			return 0;
		}

		controller.show_help();
		break;
	}

	default:
		controller.show_help();
		exit(1);
	}
}

/**
 * 初始化主界面设置
 */
void init_main_window()
{
	GladeXML* ui = glade_xml_new("mainFrame.glade", NULL, NULL);
	CMainWindow::mainWindow = new CMainWindow(ui);
	CSettingWindow::settingWindow = new CSettingWindow(ui);
	CFullScreenWindow::fullScreenWindow = new CFullScreenWindow(ui);

	PersistenceController persistence;
	struct UserData data;
	persistence.readUserData(&data);
	CSettingWindow::settingWindow->read_user_data(data);
}

/**
 * 专门用来调试的函数
 */
void do_test1()
{
	CDaemonController daemon;
	CRestController rest;
	CWhipController whip;

	rest.read_setting();
	rest.do_rest();
}

void do_test2()
{
	CDaemonController daemon;
	CRestController rest;
	CWhipController whip;
//	rest.set_alt_enable(true);
//	rest.turn_off_screen();
	rest.read_setting();
	rest.do_rest();
//	MusicController::end_music();
}
