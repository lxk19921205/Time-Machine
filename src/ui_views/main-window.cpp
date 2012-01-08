/*
 * main-window.cpp
 *
 *  Created on: 2012-1-5
 *      Author: wjp
 */

#include "../../head/ui_views/main-window.h"
#include <glade/glade.h>

CMainWindow* CMainWindow::mainWindow = NULL;

CMainWindow::CMainWindow(GladeXML* ui) {
	window = (GtkWindow*)glade_xml_get_widget(ui, "window");
	startButton = glade_xml_get_widget(ui, "startButton");
	stopButton = glade_xml_get_widget(ui, "stopButton");
	delayButton = glade_xml_get_widget(ui, "delayButton");
	//gtk_widget_show_all((GtkWidget*)window);
}

GtkWindow* CMainWindow::get_main_window()
{
	return window;
}

GtkWidget* CMainWindow::get_start_button(){
	return startButton;
}

GtkWidget* CMainWindow::get_stop_button(){
	return stopButton;
}

GtkWidget* CMainWindow::get_delay_button(){
	return delayButton;
}

