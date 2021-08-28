/**
 * @file systime.cpp
 * @author BeingGod (1085779572@qq.com)
 * @brief 统一跨平台毫秒级计时函数
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <utils/systime.h>

#if defined(LINUX)

/**
 * @brief 获取当前系统时间
 * 
 * @return systime 系统时间
 */
static Systime getSystime()
{
    timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_usec / 1000.0 + tv.tv_sec * 1000.0;
}

/**
 * @brief 获取当前系统时间 Linux
 * 
 * @param t 系统时间
 */
void getSystime(Systime &t)
{
    static Systime time_base = getSystime();
    timeval tv;
    gettimeofday(&tv, nullptr);
    t = tv.tv_usec / 1000.0 + tv.tv_sec * 1000.0 - time_base;
}

#elif defined(WINDOWS)

/**
 * @brief 获取当前系统时间 Windows
 * 
 * @param t 系统时间
 */
void getSystime(Systime &t)
{
    SYSTEMTIME tv;
    GetLocalTime(&tv);
    t = tv.wMilliseconds + tv.wSecond * 1000.0;
}

#else

#error "No supported platform"

#endif

/**
 * @brief 计算时间间隔
 * 
 * @param now 系统时间
 * @param last 系统时间
 * @return double 时间差
 */
double getTimeIntervalms(const Systime &now, const Systime &last)
{
    return now - last;
}