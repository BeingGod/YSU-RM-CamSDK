/*
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-08 17:15:13
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-10 23:07:45
 * @Description: 
 */

#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

typedef double Systime; // 时间戳

void getSystime(Systime &t);
double getTimeIntervalms(const Systime &now, const Systime &last);

#if defined(LINUX)
#include <sys/time.h>

#elif defined(WINDOWS)
#include <Windows.h>

#else
#error "No supported platform"

#endif

#endif