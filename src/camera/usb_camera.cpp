/**
 * @file usb_camera.cpp
 * @author BeingGod (1085779572@qq.com)
 * @brief 普通相机类实现
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <camera/camera_wrapper.h>
#include <unistd.h>
#include <thread>

using namespace cv;
using namespace cam;
using namespace std;

USBCamera::USBCamera(int id, int video_index, int width, int height, int fps)
    : Camera(id, CAM_TYPE_USB), m_width_(width), m_height_(height),
      m_fps_(fps), m_video_index_(video_index)
{
#if defined(LINUX)
// 若为Linux系统,则使用V4L2框架打开相机
    #define OPEN_CALL m_cap_.open(m_video_index_, CV_CAP_V4L2)
#else
    #define OPEN_CALL m_cap_.open(m_video_index_)
#endif
    int try_times = 1;
    while (!OPEN_CALL)
    {
        if (try_times > 5)
        {
            LOGE("USB camera init failed, exiting...");
            exit(CAMERA_ERROR);
        }
        LOGE("Initing USB camera，try %d times...", try_times);
        try_times++;
        usleep(1000000); // 间隔一秒
    }
}

USBCamera::~USBCamera()
{
    m_cap_.release();
    LOGW("USB Camera device:%u is released!", getCamId());
}

bool USBCamera::read()
{
    if (!isInited())
    {
        LOGW("Please init camera first!");
        return false;
    }

    Mat frame;
    Systime time_stamp;
    PackedFrame packed_frame;

    while (m_thread_active)
    {// 循环读取图像
        if (m_cap_.read(frame))
        {// 读取图像成功
            getSystime(time_stamp);
            packed_frame.frame = frame;
            packed_frame.time_stamp = time_stamp;

            m_buffer_.push(packed_frame); // 入队
        }
    }
    
    return true;
}

bool USBCamera::setCamPropValue(CamProp prop, double value)
{
    if (!isInited())
    {
        LOGW("Please init camera first!");
        return false;
    }

    switch (prop)
    {
    case 0:
        return m_cap_.set(CV_CAP_PROP_FRAME_WIDTH, m_width_);
        
    case 1:
        return m_cap_.set(CV_CAP_PROP_FRAME_HEIGHT, m_height_);

    case 2:
        return m_cap_.set(CV_CAP_PROP_FPS, m_fps_);;
    
    case 3:
    case 4:
        LOGW("USB Camera device not support property!");

    case 5:
        if (m_cap_.set(CAP_PROP_EXPOSURE,value))
            return true;
        else
        {
            LOGW("USB Camera device not support property!");
            return false;
        }

    case 6:
        if (m_cap_.set(CAP_PROP_CONTRAST, value))
            return true;
        else
        {
            LOGW("USB Camera device not support property!");
            return false;
        }
    
    case 7:
        if (m_cap_.set(CAP_PROP_WHITE_BALANCE_BLUE_U, value))
            return true;
        else
        {
            LOGW("USB Camera device not support property!");
            return false;
        }

    case 8:
        if (m_cap_.set(CAP_PROP_GAIN, value))
            return true;
        else
        {
            LOGW("USB Camera device not support property!");
            return false;
        }
    
    case 9:
        if (m_cap_.set(CAP_PROP_SATURATION, value))
            return true;
        else
        {
            LOGW("USB Camera device not support property!");
            return false;
        }
    
    case 10:
        if (m_cap_.set(CAM_PROP_BRIGHTNESS, value))
            return true;
        else
        {
            LOGW("USB Camera device not support property!");
            return false;
        }
    
    default:
        LOGW("USB Camera device not support property!");
        
        return false;
    }
}

bool USBCamera::init()
{
    m_cap_.set(CV_CAP_PROP_FRAME_WIDTH, m_width_);
    m_cap_.set(CV_CAP_PROP_FRAME_HEIGHT, m_height_);
    m_cap_.set(CV_CAP_PROP_FPS, m_fps_);

    if (m_cap_.isOpened())
    {// 相机初始化成功
        m_status_ = true;

        // 启动图像采集线程
        std::thread t(&USBCamera::read, this);
        t.detach();
        sleep(3); // 让采集线程先填充图像队列

        LOGM("USB camera device:%u init success!", getCamId());

        return true;
    }
    else 
    {
        return false;
    }

}