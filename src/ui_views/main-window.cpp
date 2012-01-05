/*
 * main-window.cpp
 *
 *  Created on: 2012-1-5
 *      Author: wjp
 */

#include "../../head/ui_views/main-window.h"
#include <gtk/gtk.h>

CMainWindow::CMainWindow(GtkWidget* window) {
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(window, 600, 400);
	GtkWidget* button;
	button = gtk_button_new_with_label("Hello");
	gtk_container_add(GTK_CONTAINER(window), button);
	gtk_widget_show(button);
	gtk_widget_show(window);
	gtk_main();
}

