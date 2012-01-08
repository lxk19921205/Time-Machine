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

CSettingWindow* CSettingWindow::settingWindow = NULL;

CSettingWindow::CSettingWindow(GladeXML* ui)
{
	GtkWidget* settingWindow = glade_xml_get_widget(ui, "settingWindow");
	gtk_widget_show_all(settingWindow);
	levelHScrollbar = (GtkHScrollbar*) glade_xml_get_widget(ui,
			"levelHScrollbar");
	saveButton = glade_xml_get_widget(ui, "saveButton");
	intervalEntry = (GtkEntry*) glade_xml_get_widget(ui, "intervalEntry");
	lastEntry = (GtkEntry*) glade_xml_get_widget(ui, "lastEntry");
	ifStartWithPowerCheck = (GtkCheckButton*) glade_xml_get_widget(ui,
			"ifStartWithPowerCheck");
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
	if (value >= 0 && value < 33)
	{
		m_data.userLevel = remind;
	}
	else if (value <= 100 && value > 66)
	{
		m_data.userLevel = force;
	}
	else
	{
		//[33, 66]
		m_data.userLevel = remind_force;
	}

	string intervalEntryText = gtk_entry_get_text(intervalEntry);
	int intervalTime = atoi(intervalEntryText.c_str());
	m_data.restIntervalTime = intervalTime;

	string lastEntryText = gtk_entry_get_text(lastEntry);
	int lockTime = atoi(lastEntryText.c_str());
	m_data.lockTime = lockTime;

	bool ifStartWithPower = gtk_toggle_button_get_active(
			&(ifStartWithPowerCheck->toggle_button));
	m_data.ifStartWithPower = ifStartWithPower;

	bool ifShutScreen = gtk_toggle_button_get_active(
			&(ifShutScreenCheck->toggle_button));
	m_data.ifCloseScreen = ifShutScreen;

	bool ifForceExit = gtk_toggle_button_get_active(
			&(ifForceExitCheck->toggle_button));
	m_data.canForceToExit = ifForceExit;

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

void CSettingWindow::read_user_data(const UserData& data)
{
	if (data.userLevel == remind)
	{
		gtk_range_set_value(&(levelHScrollbar->scrollbar.range), 0);
	}
	else if (data.userLevel == remind_force)
	{
		gtk_range_set_value(&(levelHScrollbar->scrollbar.range), 50);
	}
	else
	{
		gtk_range_set_value(&(levelHScrollbar->scrollbar.range), 100);
	}

	char buffer[16];
	sprintf(buffer, "%d", data.restIntervalTime);
	gtk_entry_set_text(intervalEntry, buffer);

	char buffer1[16];
	sprintf(buffer1, "%d", data.lockTime);
	gtk_entry_set_text(lastEntry, buffer1);

	gtk_toggle_button_set_active(&(ifStartWithPowerCheck->toggle_button),
			data.ifStartWithPower);
	gtk_toggle_button_set_active(&(ifShutScreenCheck->toggle_button),
			data.ifCloseScreen);
	gtk_toggle_button_set_active(&(ifForceExitCheck->toggle_button),
			data.canForceToExit);
	gtk_toggle_button_set_active(&(ifCanDelayCheck->toggle_button),
			data.canDelay);

	gtk_file_chooser_set_filename((GtkFileChooser*) restMusicPathButton,
			data.restMp3Path.c_str());

	gtk_file_chooser_set_filename((GtkFileChooser*) whipMusicPathButton,
			data.whipMp3Path.c_str());

	gtk_file_chooser_set_filename((GtkFileChooser*) imagePathButton,
			data.imagePath.c_str());

	gtk_entry_set_text(commandLineEntry, data.customCommand.c_str());
	gtk_entry_set_text(whipTextEntry, data.whipWord.c_str());

}
