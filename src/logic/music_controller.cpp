/*
 * music_controller.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: jolin
 */

#include <iostream>
#include <gst/gst.h>

#include "../../head/logic/music_controller.h"
using namespace std;

GMainLoop* MusicController::loop = NULL;
GstElement* MusicController::pipeline = NULL;


void initPlayer(){
	const gchar *nano_str;
	guint major, minor, micro, nano;

	//initial gstreamer
	gst_init (NULL, NULL);
	gst_version(&major, &minor, &micro, &nano);

	if (nano == 1)
		nano_str = "(CVS)";
	else if (nano == 2)
		nano_str = "(Prerelease)";
	else
		nano_str = "";
	printf ("This program is linked against GStreamer %d.%d.%d %s\n",
			major, minor, micro, nano_str);
}

void MusicController::play_music(gchar* file)
{
 	initPlayer();

	//get the state of music
	//GstState state;
	//gst_element_get_state(pipeline, &state, NULL, -1);

	// define the element
	GstElement	*source,*decoder,*sink;
	GstBus *bus;

	//define the main loop
	loop = g_main_loop_new(NULL,FALSE);
	//establish the pipe and component
	pipeline = gst_pipeline_new("audio-player");

	//read the source file
	source = gst_element_factory_make("filesrc","file_source");

	decoder = gst_element_factory_make("mad","mad-decoder");

	//output the sound to sound card
	sink = gst_element_factory_make("autoaudiosink","audio-output");

	if(!pipeline || !source || !decoder || !sink)
	{
		g_printerr("One element could not be created!");
	}

	g_object_set(G_OBJECT(source), "location", file, NULL);

	//use bus to get the message from pipe
	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

	//add components to pipe
	gst_bin_add_many(GST_BIN(pipeline),source,decoder,sink,NULL);

	//link the element
	gst_element_link_many(source,decoder,sink,NULL);

	//set the pipe state playing
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	g_main_loop_run(loop);

}

void MusicController::end_music()
{
	//quit the music loop
	g_main_loop_quit(loop);

	//set the pipe state NULL
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(pipeline));

}

