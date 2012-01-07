/*
 * setting-window.h
 *
 *  Created on: 2012-1-6
 *      Author: wjp
 */

#ifndef SETTING_WINDOW_H_
#define SETTING_WINDOW_H_

#include <glade/glade.h>
#include <gtk/gtk.h>
#include "../Persistence/persistence_controller.h"

class CSettingWindow
{
private:
	UserData m_data;
private:
	GtkHScrollbar* levelHScrollbar;
	GtkWidget* saveButton;
	GtkEntry* intervalEntry;
	GtkEntry* lastEntry;
	GtkEntry* pauseEntry;
	GtkEntry* delayEntry;
public:
	CSettingWindow(GladeXML* ui);
	GtkWidget* get_save_button();
	UserData get_user_data();
};


#endif /* SETTING_WINDOW_H_ */
