/*
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-15 23:44:30
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-18 20:54:47
 * @Description: 
 */

#include <camera/camera_wrapper.h>

int main()
{
    MindVisionCamera camera(1, "049032410170", 0);

    Systime t1, t2;
    
    PackedFrame pf;
    if (!camera.init())
        exit(-1);

    if (camera.setGroupProp(0, "/home/beinggod/workspace/camera/Camera/Configs/MV-SUA133GC-Group0.config"))
    {// 更新参数组
        LOGM("UPDATE SUCCESS");
    }
    else
    {
        LOGM("UPDATE FAILED");
    }

    int display = 10000;

    getSystime(t1);
    while (display)
    {
        PackedFrame pf = camera.getFrameFromBuffer();

        cv::imshow("source", pf.frame);
        LOGM("Time Stamp: %.2fms", pf.time_stamp);
        int key = cv::waitKey(1);

        if (key == 27)
            break;

        display--;
    }
    getSystime(t2);

    std::cout << getTimeIntervalms(t2, t1) << "ms" << std::endl;

    return 0;
}