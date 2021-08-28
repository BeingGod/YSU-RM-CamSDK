/**
 * @file test_mindvision_camera.cpp
 * @author BeingGod (1085779572@qq.com)
 * @brief MindVisionCamera测试程序
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <camera/camera_wrapper.h>

using namespace cam;

int main()
{
    MindVisionCamera camera(1, "049032410170", 0);

    Systime t1, t2;
    
    PackedFrame pf;
    if (!camera.init())
        exit(-1);

    // if (camera.setGroupProp(0, "/home/beinggod/workspace/camera/Camera/Configs/MV-SUA133GC-Group0.config"))
    // {// 更新参数组
    //     LOGM("UPDATE SUCCESS");
    // }
    // else
    // {
    //     LOGM("UPDATE FAILED");
    // }

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