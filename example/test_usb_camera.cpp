/**
 * @file test_usb_camera.cpp
 * @author BeingGod (1085779572@qq.com)
 * @brief USBCamera测试程序
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <camera/camera_wrapper.h>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace cam;

int main()
{
    USBCamera camera(0,0);

    if (!camera.init())
        exit(-1);
        
    while (true)
    {
        PackedFrame pf = camera.getFrameFromBuffer();
        
        cv::imshow("source", pf.frame);
        LOGM("Time Stamp: %.2fms", pf.time_stamp);
        if (cv::waitKey(30) == 27)
            break;
    }

    return 0;
}
