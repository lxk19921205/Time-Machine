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
	ifStartWithPowerCheck = (GtkCheckButton*) glade_xml_get_widget(ui,
			"ifStartWithPowerCheck");
	ifBeepCheck = (GtkCheckButton*) glade_xml_get_widget(ui, "ifBeepCheck");
	ifShutScreenCheck = (GtkCheckButton*) glade_xml_get_widget(ui,
			"ifShutScreenCheck");
	ifForceExitCheck = (GtkCheckButton*) glade_xml_get_widget(ui,
			"ifForceExitCheck");
	ifStopTimingCheck = (GtkCheckButton*) glade_xml_get_widget(ui,
			"ifStopTimingCheck");
	ifCanDelayCheck = (GtkCheckButton*) glade_xml_get_widget(ui,
			"ifCanDelayCheck");
	restMusicPathButton = (GtkFileChooserButton*) glade_xml_get_widget(ui,
			"restMusicPath");
	whipMusicPathButton = (GtkFileChooserButton*) glade_xml_get_widget(ui,
			"whipMusicPath");
	imagePathButton = (GtkFileChooserButton*) glade_xml_get_widget(ui,
			"imagePath");
	commandLineEntry = (GtkEntry*) glade_xml_get_widget(ui, "commandLineEntry");
	whipTextEntry = (GtkEntry*) glade_xml_get_widget(ui, "whipTextEntry");
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
	m_data.restIntervalTime = intervalTime;

	string lastEntryText = gtk_entry_get_text(lastEntry);
	int lockTime = atoi(lastEntryText.c_str());
	m_data.lockTime = lockTime;

	string pauseEntryText = gtk_entry_get_text(pauseEntry);
	int unLockTime = atoi(pauseEntryText.c_str());
	m_data.unLockTime = unLockTime;

	string delayEntryText = gtk_entry_get_text(delayEntry);
	int delayTime = atoi(delayEntryText.c_str());
//	m_data.delayTime = delayTime;

	bool ifStartWithPower = gtk_toggle_button_get_active(
			&(ifStartWithPowerCheck->toggle_button));
	m_data.ifStartWithPower = ifStartWithPower;

	bool ifBeep = gtk_toggle_button_get_active(&(ifBeepCheck->toggle_button));
//	m_data.ifBeep = ifBeep;

	bool ifShutScreen = gtk_toggle_button_get_active(
			&(ifShutScreenCheck->toggle_button));
	m_data.ifCloseScreen = ifShutScreen;

	bool ifForceExit = gtk_toggle_button_get_active(
			&(ifForceExitCheck->toggle_button));
	m_data.canForceToExit = ifForceExit;

	bool ifStopTiming = gtk_toggle_button_get_active(
			&(ifStopTimingCheck->toggle_button));
//	m_data.ifStopTiming = ifStopTiming;

	bool ifCanDelay = gtk_toggle_button_get_active(
			&(ifCanDelayCheck->toggle_button));
	m_data.canDelay = ifCanDelay;

	string restMusicPath = gtk_file_chooser_get_filename(
			(GtkFileChooser*) restMusicPathButton);
	m_data.restMp3Path = restMusicPath;

	string whipMusicPath = gtk_file_chooser_get_filename(
			(GtkFileChooser*) whipMusicPathButton);
	m_data.whipMp3Path = whipMusicPath;

	string imagePath = gtk_file_chooser_get_filename(
			(GtkFileChooser*) imagePathButton);
	m_data.imagePath = imagePath;

	string commandLine = gtk_entry_get_text(commandLineEntry);
	m_data.customCommand = commandLine;

	string whipText = gtk_entry_get_text(whipTextEntry);
	m_data.whipWord = whipText;

	return m_data;
}
