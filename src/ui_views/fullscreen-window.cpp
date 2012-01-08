/*
 * fullscreen-window.cpp
 *
 *  Created on: 2012-1-8
 *      Author: wjp
 */
#include "../../head/ui_views/fullscreen-window.h"
#include <glade/glade.h>
#include <gtk/gtk.h>

CFullScreenWindow* CFullScreenWindow::fullScreenWindow = NULL;

CFullScreenWindow::CFullScreenWindow()
{
	GladeXML* ui = glade_xml_new("mainFrame.glade", NULL, NULL);
	this->init(ui);
}

CFullScreenWindow::CFullScreenWindow(GladeXML* ui)
{
	this->init(ui);
}

void CFullScreenWindow::init(GladeXML* ui)
{
	window = (GtkWindow*) glade_xml_get_widget(ui, "fullScreenWindow");
	restTimeLabel = (GtkLabel*) glade_xml_get_widget(ui, "restTimeLabel");
	whipLabel = (GtkLabel*) glade_xml_get_widget(ui, "whipLabel");
//	gtk_window_fullscreen(window);
	gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
}

void CFullScreenWindow::show_fullscreen_window()
{
	gtk_widget_show_all((GtkWidget*) window);
}

void CFullScreenWindow::set_whip_word(string whipWord)
{
	gtk_label_set_text(whipLabel, whipWord.c_str());
}

void CFullScreenWindow::show_rest_time(string time)
{
	gtk_label_set_text(restTimeLabel, time.c_str());
}
