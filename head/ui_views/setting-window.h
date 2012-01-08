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
	GtkCheckButton* ifStartWithPowerCheck;
	GtkCheckButton* ifShutScreenCheck;
	GtkCheckButton* ifForceExitCheck;
	GtkCheckButton* ifStopTimingCheck;
	GtkCheckButton* ifCanDelayCheck;
	GtkFileChooserButton* restMusicPathButton;
	GtkFileChooserButton* whipMusicPathButton;
	GtkFileChooserButton* imagePathButton;
	GtkEntry* commandLineEntry;
	GtkEntry* whipTextEntry;
public:
	CSettingWindow(GladeXML* ui);
	GtkWidget* get_save_button();
	UserData get_user_data();
	void read_user_data(const UserData& data);

	static CSettingWindow* settingWindow;
};


#endif /* SETTING_WINDOW_H_ */
