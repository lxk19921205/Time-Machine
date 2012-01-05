//============================================================================
// Name        : Time-Machine.cpp
// Author      : Andriy, 590, WJP
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <iostream>
#include <gtk/gtk.h>
#include "./../head/ui_views/main-window.h"

using namespace std;

void init_daemon_process(const char* cmd);
void init_main_window();

int main(int argc, char* argv[]) {
	gtk_init(&argc, &argv);
	init_main_window();
	//init_daemon_process("----------Time Machine----------");
	return 0;
}

/**
 * 初始化主界面设置
 */

void init_main_window() {
	GtkWidget* window;
	CMainWindow mainWindow(window);
}

/**
 * 初始化daemon process，之后此进程就一直默默地工作着
 */
void init_daemon_process(const char* cmd) {
	//step 1
	umask(0);

	struct rlimit rl;
	getrlimit(RLIMIT_NOFILE, &rl);

	pid_t pid = fork();
	if (pid < 0) {
		//fork()失败了
		exit(1);
	} else if (pid > 0) {
		//parent
		exit(0);
	}

	//是child process了现在
	setsid();

	//确保将来的open不会分配控制着的tty
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGHUP, &sa, NULL);
	pid = fork();
	if (pid < 0) {
		//fork()失败了
		exit(1);
	} else if (pid > 0) {
		//parent process
		exit(0);
	}

	//改变当前目录到根目录
	chdir("/");

	//关闭所有打开的file descriptors
	if (rl.rlim_max == RLIM_INFINITY) {
		rl.rlim_max = 1024;
	}
	for (unsigned int i = 0; i < rl.rlim_max; i++) {
		close(i);
	}

	//把fd 0，1，2和/dev/null关联起来
	int fd0 = open("/dev/null", O_RDWR);
	int fd1 = dup(0);
	int fd2 = dup(0);

	//init Log
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

/**
 * 移除后台默默守候的进程
 */
void remove_daemon_process() {

}
