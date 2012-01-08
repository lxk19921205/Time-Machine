/*
 * rest_controller.cpp
 *
 *  Created on: 2012-1-6
 *      Author: andriy
 */

#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <pthread.h>

#include <iostream>

#include "../../head/logic/rest_controller.h"
#include "../../head/Persistence/persistence_controller.h"
#include "../../head/logic/music_controller.h"
#include "../../head/ui_views/fullscreen-window.h"

using namespace std;

pid_t music_pid = -1;
CFullScreenWindow* fullScreenWindow = NULL;
long left_seconds = 0;

CRestController::CRestController()
	: CAbsProcController("/var/run/TimeMachineRest.pid")
{
}

CRestController::~CRestController()
{
}

void CRestController::init_process()
{
	int pid = fork();
	if (pid < 0)
	{
		//error
		syslog(LOG_ERR, "fork error in init_rest_child()");
		return;
	}
	else if (pid > 0)
	{
		//parent
		return;
	}

	openlog("-----Time Machine Rest d-----", LOG_CONS, LOG_DAEMON);

	//child

	if (this->lock_file() == false)
	{
		//=====不是唯一运行的=====
		syslog(LOG_ERR, "try to start up rest process again when there is one running!");
		exit(1);
	}

	this->read_setting();
	this->set_signal();
	this->start_waiting();

	exit(0);
}

static void signal_handler(int signum, siginfo_t *info, void* context)
{
	if (signum == SIGALRM)
	{
		CRestController restController;
		restController.read_setting();
		restController.do_rest();
	}
}


void CRestController::set_signal()
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGALRM, &sa, NULL);
}


void CRestController::start_waiting()
{
	while (true)
	{
		struct timeval interval;
		interval.tv_sec = setting.restIntervalTime * 60;
		interval.tv_usec = 0;

//		//TODO testing
//		interval.tv_sec = 10;

		//=====白天长一点，晚上短一点=====
		if (this->is_night())
		{
			interval.tv_sec *= 0.8;
		}

		//TODO 应该改为每隔1秒，醒过来一次，判断一下到了没有，这样可以知道还有多久
		//但是数据写在哪里呢？

		struct itimerval value;
		value.it_interval = interval;
		value.it_value = interval;

		if (setitimer(ITIMER_REAL, &value, NULL) == -1)
		{
			//error
			syslog(LOG_ERR, "rest timer error: %s", strerror(errno));
			exit(1);
		}
		pause();
	}
}

bool CRestController::is_night()
{
	//TODO
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
	return false;
}

/**
 * 返回一个代表此时间的string
 */
string parse_time(long seconds)
{
	string s;
	char buffer[16];
	if (seconds < 60)
	{
		if (seconds < 10)
		{
			sprintf(buffer, "00 : 0%d", seconds);
		}
		else
		{
			sprintf(buffer, "00 : %d", seconds);
		}
		return string(buffer);
	}

	long minutes = seconds / 60;
	seconds %= 60;
	if (seconds < 10)
	{
		if (minutes < 10)
		{
			sprintf(buffer, "0%d:0%d", minutes, seconds);
		}
		else
		{
			sprintf(buffer, "%d:0%d", minutes, seconds);
		}
	}
	else
	{
		if (minutes < 10)
		{
			sprintf(buffer, "0%d:%d", minutes, seconds);
		}
		else
		{
			sprintf(buffer, "%d:%d", minutes, seconds);
		}
	}
	return string(buffer);
}

/**
 * full screen的signal处理
 */
void full_signal_handler(int signum, siginfo_t *info, void* context)
{
	if (signum == SIGALRM && fullScreenWindow != NULL)
	{
		if (left_seconds <= 0)
		{
			gtk_main_quit();
			delete fullScreenWindow;
			fullScreenWindow = NULL;
			if (music_pid != -1)
			{
				kill(music_pid, SIGKILL);
				waitpid(music_pid, NULL, 0);
			}
			exit(0);
		}

		fullScreenWindow->show_rest_time(parse_time(left_seconds));
		left_seconds--;
	}
}


void CRestController::do_rest()
{
	if (setting.ifCloseScreen)
	{
		this->turn_off_screen();
	}
	this->set_alt_enable(false);

	pid_t pid = fork();
	if (pid < 0)
	{
		syslog(LOG_ERR, "fork error in do_rest()");
		exit(1);
	}
	else if (pid == 0)
	{
//		this->show_fullscreen(setting.whipWord, 10);
		this->show_fullscreen(setting.whipWord, setting.lockTime*60);
		exit(0);
	}

//	//[0]read  [1]write
//	int fds[2];
//	pipe(fds);
//	pid = fork();
//	if (pid < 0)
//	{
//		syslog(LOG_ERR, "fork error in do_rest()");
//		exit(1);
//	}
//	else if (pid == 0)
//	{
//		//BEEP
//		close(fds[0]);
//		pid_t musicid = getpid();
//		write(fds[1], &musicid, sizeof(pid_t));
//		syslog(LOG_INFO, "start to play music");
//		MusicController::play_music((char*) setting.restMp3Path.c_str());
//		exit(0);
//	}
//	close(fds[1]);
//	read(fds[0], &music_pid, sizeof(pid_t));
//	syslog(LOG_INFO, "music processid %d", music_pid);

	int status;
	wait(&status);
	this->set_alt_enable(true);//	MusicController::end_music();
}


void CRestController::show_fullscreen(string& whipWord, long total_time)
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_sigaction = full_signal_handler;
	sigaction(SIGALRM, &sa, NULL);

	fullScreenWindow = new CFullScreenWindow();
	left_seconds = total_time;
	fullScreenWindow->set_whip_word(whipWord);
	fullScreenWindow->show_fullscreen_window();

	struct timeval interval;
	interval.tv_sec = 1;
	interval.tv_usec = 0;

	struct itimerval value;
	value.it_interval = interval;
	value.it_value = interval;

	if (setitimer(ITIMER_REAL, &value, NULL) == -1)
	{
		//error
		syslog(LOG_ERR, "rest timer error: %s", strerror(errno));
		exit(1);
	}

	syslog(LOG_INFO, "prepare to gtk_main()");

	gtk_main();
}


void CRestController::turn_off_screen()
{
	sleep(1);
//	execlp("xset","", "dpms", "force", "off", NULL);
	system("xset dpms force off");
//	int pid = fork();
//	if (pid < 0)
//	{
//		//error
//		syslog(LOG_ERR, "fork error in do_rest()");
//		exit(1);
//	}
//	else if (pid == 0)
//	{
//		sleep(1);
//		execlp("xset","", "dpms", "force", "off", NULL);
//		exit(0);
//	}
}


void CRestController::set_alt_enable(bool enabled)
{
	if (enabled)
	{
		system("xmodmap -e 'keycode 64 = Alt_L Meta_L'");
		system("xmodmap -e 'keycode 108 = Alt_R Meta_R'");
		system("xmodmap -e 'add mod1 = Alt_L Alt_R Meta_L Meta_R'");

		//=====多一行，只是因为让add产生作用，不延迟，具体看文档～=====
		system("xmodmap -e 'keycode 64 = Alt_L Meta_L'");
	}
	else
	{
		system("xmodmap -e 'clear mod1'");
		system("xmodmap -e 'keycode 64 = a A'");
		system("xmodmap -e 'keycode 108 = b B'");
	}
}
