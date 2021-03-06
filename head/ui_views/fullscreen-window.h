/*
 * fullscreen-window.h
 *
 *  Created on: 2012-1-8
 *      Author: wjp
 */

#ifndef FULLSCREEN_WINDOW_H_
#define FULLSCREEN_WINDOW_H_

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <string>

using namespace std;

class CFullScreenWindow
{
private:
	GtkWindow* window;

	GtkLabel* restTimeLabel;
	GtkLabel* whipLabel;

	void init(GladeXML* ui);

public:
	CFullScreenWindow();
	CFullScreenWindow(GladeXML* ui);

	void show_fullscreen_window();
	void set_whip_word(string whipWord);
	void show_rest_time(string time);

	static CFullScreenWindow* fullScreenWindow;
};


#endif /* FULLSCREEN_WINDOW_H_ */
