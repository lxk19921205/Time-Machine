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
	gtk_widget_set_usize(window, 850, 550);
	gtk_window_set_resizable((GtkWindow*)(window), false);
	AddBackGroundImage(window);
	gtk_widget_show(window);
	gtk_main();
}

void CMainWindow::AddBackGroundImage(GtkWidget* window) {
	GtkWidget* backGroundImage;
	backGroundImage = gtk_image_new_from_file("Floating Leaves.jpg");
	gtk_container_add(GTK_CONTAINER(window), backGroundImage);
	gtk_widget_show(backGroundImage);
}

