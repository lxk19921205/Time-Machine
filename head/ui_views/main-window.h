/*
 * main-window.h
 *
 *  Created on: 2012-1-5
 *      Author: wjp
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_
#include <glade/glade.h>
#include <gtk/gtk.h>

class CMainWindow {

private:
	GtkWindow* window;
	GtkWidget* startButton;
	GtkWidget* stopButton;
	GtkWidget* delayButton;

public:
	CMainWindow(GladeXML* ui);

	GtkWindow* get_main_window();
	GtkWidget* get_start_button();
	GtkWidget* get_stop_button();
	GtkWidget* get_delay_button();

	static CMainWindow* mainWindow;
};

#endif /* MAIN_WINDOW_H_ */
