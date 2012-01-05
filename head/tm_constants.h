/*
 * tm_constants.h
 *
 *  Created on: 2012-1-5
 *      Author: andriy
 */

#ifndef TM_CONSTANTS_H_
#define TM_CONSTANTS_H_


//=====用来判断此程序是否是唯一运行的程序的文件=====
#define TM_LOCK_FILE "/var/run/TimeMachine.pid"
#define TM_LOCK_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


#endif /* TM_CONSTANTS_H_ */
