/*
 * setting-window.cpp
 *
 *  Created on: 2012-1-6
 *      Author: wjp
 */
#include "../../head/ui_views/setting-window.h"
#include <glade/glade.h>

CSettingWindow::CSettingWindow(GladeXML* ui)
{
	GtkWidget* settingWindow = glade_xml_get_widget(ui, "settingWindow");
	gtk_widget_show_all(settingWindow);
	levelHScrollbar = (GtkHScrollbar*) glade_xml_get_widget(ui,
			"levelHScrollbar");
	saveButton = glade_xml_get_widget(ui, "saveButton");
}

GtkWidget* CSettingWindow::get_save_button()
{
	return saveButton;
}

UserData CSettingWindow::get_user_data()
{
	gdouble value = gtk_range_get_value(&(levelHScrollbar->scrollbar.range));
	if(value == 0)
	{
		m_data.userLevel = remind;
	}
	else if(value == 100)
	{
		m_data.userLevel = force;
	}
	else
	{
		m_data.userLevel = remind_force;
	}
	return m_data;
}
