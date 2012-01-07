/*
 * setting-window.cpp
 *
 *  Created on: 2012-1-6
 *      Author: wjp
 */
#include "../../head/ui_views/setting-window.h"
#include <glade/glade.h>
#include <stdlib.h>
#include <string>

CSettingWindow::CSettingWindow(GladeXML* ui)
{
	GtkWidget* settingWindow = glade_xml_get_widget(ui, "settingWindow");
	gtk_widget_show_all(settingWindow);
	levelHScrollbar = (GtkHScrollbar*) glade_xml_get_widget(ui,
			"levelHScrollbar");
	saveButton = glade_xml_get_widget(ui, "saveButton");
	intervalEntry = (GtkEntry*) glade_xml_get_widget(ui, "intervalEntry");
	lastEntry = (GtkEntry*) glade_xml_get_widget(ui, "lastEntry");
	pauseEntry = (GtkEntry*) glade_xml_get_widget(ui, "pauseEntry");
	delayEntry = (GtkEntry*) glade_xml_get_widget(ui, "delayEntry");
}

GtkWidget* CSettingWindow::get_save_button()
{
	return saveButton;
}

UserData CSettingWindow::get_user_data()
{
	gdouble value = gtk_range_get_value(&(levelHScrollbar->scrollbar.range));
	if (value == 0)
	{
		m_data.userLevel = remind;
	}
	else if (value == 100)
	{
		m_data.userLevel = force;
	}
	else
	{
		m_data.userLevel = remind_force;
	}

	string intervalEntryText = gtk_entry_get_text(intervalEntry);
	int intervalTime = atoi(intervalEntryText.c_str());
	m_data.intervalTime = intervalTime;

	string lastEntryText = gtk_entry_get_text(lastEntry);
	int lockTime = atoi(lastEntryText.c_str());
	m_data.lockTime = lockTime;

	string pauseEntryText = gtk_entry_get_text(pauseEntry);
	int unLockTime = atoi(pauseEntryText.c_str());
	m_data.unLockTime = unLockTime;

	string delayEntryText = gtk_entry_get_text(delayEntry);
	int delayTime = atoi(delayEntryText.c_str());
	m_data.delayTime = delayTime;

	return m_data;
}
