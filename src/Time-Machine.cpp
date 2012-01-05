//============================================================================
// Name        : Time-Machine.cpp
// Author      : Andriy, 590, WJP
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "../head/logic/daemon_controller.h"

using namespace std;


int main(int argc, char** argv)
{
	CDaemonController daemon_controller;
	daemon_controller.init_daemon();

	return 0;
}

