/*
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-10 22:40:59
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-16 00:06:34
 * @Description: 
 */

#include <camera/camera_wrapper.h>
#include <iostream>
#include <unistd.h>

using namespace std;

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
