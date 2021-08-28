/**
 * @file test_camera_manager.cpp
 * @author BeingGod (1085779572@qq.com)
 * @brief CameraManager测试程序
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <camera/camera_manager.h>

using namespace cam;

int main(int argc,char** argv)
{
    CameraManager cam_mana;
    cam_mana.readCameraParam(argv[1]);
    cam_mana.printAllCameraDevice();
    if (!cam_mana[2]->init())
        exit(-1);
    if (!cam_mana[1]->init())
        exit(-1);

    PackedFrame pf1, pf2;
    while (true)
    {
        pf1 = cam_mana[1]->getFrameFromBuffer();
        pf2 = cam_mana[2]->getFrameFromBuffer();

        cv::imshow("MindVision Camera", pf1.frame);
        cv::imshow("USB Camera", pf2.frame);
        LOGM("mindvision timestamp %.2f", pf1.time_stamp);
        LOGM("usb timestamp %.2f", pf2.time_stamp);

        if (cv::waitKey(30) == 27)
        {
            break;
        }
    }
    return 0;
}