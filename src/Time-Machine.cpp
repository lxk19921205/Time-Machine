//============================================================================
// Name        : Time-Machine.cpp
// Author      : Andriy, 590, WJP
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <gtk/gtk.h>


#include "../head/logic/daemon_controller.h"

using namespace std;


void init_main_window();

int main(int argc, char** argv)
{
	gtk_init(&argc, &argv);
	init_main_window();

//	CDaemonController daemon_controller;
//	daemon_controller.init_daemon();

	return 0;
}

/**
 * 初始化主界面设置
 */

void init_main_window() {
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_main();
}

