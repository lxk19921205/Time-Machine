/*
 * music_controller.h
 *
 *  Created on: Jan 7, 2012
 *      Author: jolin
 */

#ifndef MUSIC_CONTROLLER_H_
#define MUSIC_CONTROLLER_H_

class MusicController
{
private:
	static GMainLoop* loop;
	static GstElement* pipeline;
public:
	//use for play music
	static void play_music(gchar* file);

	//use for end music
	static void end_music();
};


#endif /* MUSIC_CONTROLLER_H_ */
