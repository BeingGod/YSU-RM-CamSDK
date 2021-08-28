/**
 * @file systime.h
 * @author BeingGod (1085779572@qq.com)
 * @brief 统一跨平台毫秒级计时函数
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

typedef double Systime; //!< 时间戳

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