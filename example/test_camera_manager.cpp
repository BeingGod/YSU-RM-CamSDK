/*
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-30 11:24:29
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-08-08 18:53:01
 * @Description: 
 */

#include <camera/camera_manager.h>

int main(int argc,char** argv)
{
    CameraManager cam_mana;
    cam_mana.readCameraParam("./camera.xml");
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