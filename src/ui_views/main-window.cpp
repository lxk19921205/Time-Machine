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
	gtk_window_set_resizable((GtkWindow*) (window), false);
	gtk_window_set_position((GtkWindow*) (window), GTK_WIN_POS_CENTER_ALWAYS);
	AddBackGroundImage(window);
	AddHorizontalScrollBar(window);
	gtk_widget_show(window);
	gtk_main();
}

void CMainWindow::AddBackGroundImage(GtkWidget* window) {
	GtkWidget* backGroundImage;
	backGroundImage = gtk_image_new_from_file("Floating Leaves.jpg");
	gtk_container_add(GTK_CONTAINER(window), backGroundImage);
	gtk_widget_show(backGroundImage);
}

void CMainWindow::AddHorizontalScrollBar(GtkWidget* window) {
	GtkWidget* horizontalScrollBar;
	GtkAdjustment* adjustment = (GtkAdjustment*)gtk_adjustment_new(50, 0, 100, 50, 50, 10);
	horizontalScrollBar = gtk_hscrollbar_new(adjustment);
	gtk_container_add(GTK_CONTAINER(window), horizontalScrollBar);
	gtk_widget_show(horizontalScrollBar);
}

