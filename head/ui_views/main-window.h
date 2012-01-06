/*
 * main-window.h
 *
 *  Created on: 2012-1-5
 *      Author: wjp
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <gtk/gtk.h>

class CMainWindow {
public:
	CMainWindow(GtkWidget* window);
private:
	void AddBackGroundImage(GtkWidget* window);
	void AddHorizontalScrollBar(GtkWidget* panel);
};

#endif /* MAIN_WINDOW_H_ */
