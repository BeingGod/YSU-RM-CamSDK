/*
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-01-24 00:40:44
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-08-08 18:24:52
 * @Description: 抽象相机类,普通相机类,工业相机类声明
 * 
 * 添加宏定义CAMERA_MULTI可在不改变程序基础上使用生产者-消费者的多线程模式
 * 由于测试程序的局限，目前发现使用多线程并未起到明显的优化效果
 * 
 */

#ifndef _CAMERA_WRAPPER_H_
#define _CAMERA_WRAPPER_H_

#include <opencv2/opencv.hpp>
#include <MVSDK/CameraApi.h>
#include <utils/round_queue.h>
#include <utils/systime.h>
#include <utils/log.h>
#include <status_code.h>
#include <thread>


typedef unsigned int CameraId; // 设备ID

// 相机类型
enum CameraType
{
    CAM_TYPE_UNDEFINED = 0, // 未定义相机
    CAM_TYPE_MINDVISION,    // 迈德威视相机
    CAM_TYPE_USB,           // USB相机
};

// 定义相机属性
enum CamProp
{
    CAM_PROP_WIDTH = 0,     // 宽度
    CAM_PROP_HEIGHT,        // 高度
    CAM_PROP_FPS,           // 帧率
    CAM_PROP_HOFF,          // 水平偏移
    CAM_PROP_VOFF,          // 垂直偏移
    CAM_PROP_EXPOSURE_TIME, // 曝光时间
    CAM_PROP_CONTRAST,      // 对比度
    CAM_PROP_WHITE_BALANCE, // 白平衡
    CAM_PROP_GAIN,          // 增益
    CAM_PROP_SATURATION,    // 饱和度
    CAM_PROP_BRIGHTNESS,    // 亮度
};

// 包装图像帧
struct PackedFrame
{
    Systime time_stamp; // 时间戳
    cv::Mat frame;      // 图像帧
};

/**
 * @brief 相机类
 * 
 */
class Camera
{
public:
    /**
     * @brief Construct a new Camera object
     * 
     * @param id 相机ID
     * @param type 相机类型
     */
    Camera(CameraId id, int type)
        : m_cam_id_(id), m_cam_type_(type), m_buffer_(3) {}

    /**
     * @brief Destroy the Camera object
     * 
     */
    virtual ~Camera() {}

    /**
     * @brief Get the Cam Id object
     * 
     * @return CameraId 
     */
    CameraId getCamId() { return m_cam_id_; }

    /**
     * @brief Get the Cam Type object
     * 
     * @return int 
     */
    int getCamType() { return m_cam_type_; }

    /**
     * @brief 相机是否初始化
     * 
     * @return true 
     * @return false 
     */
    bool isInited() { return m_status_; }
    
    /**
     * @brief 设置相机属性
     * 
     * @param prop 属性
     * @param value 属性值
     * @return true 
     * @return false 
     */
    virtual bool setCamPropValue(CamProp prop, double value) = 0;
    
    /**
     * @brief 相机初始化
     * 
     * @return true 
     * @return false 
     */
    virtual bool init() = 0;

    /**
     * @brief 从缓冲区中获取一帧图像
     * 当缓冲区中没有图像的时候会报错，子类有必要需要覆盖该方法！
     * 
     * @return PackedFrame
     */
    PackedFrame getFrameFromBuffer() { return m_buffer_.front(); }

    /**
     * @brief 打印相机能力信息
     * @todo 待实现
     */
    virtual void printCamCapbility() { LOGM("Virtual Camera!"); }

    /**
     * @brief 停止图像采集
     * 注意：调用此函数后，程序中相机无法再次采集图像！
     * 
     */
    void stopReadImage() { m_thread_active = false; };

protected:
    /**
     * @brief 采集一帧图像
     * 
     * @return true 
     * @return false 
     */
    virtual bool read() = 0;

protected:
    bool m_status_ = false;             // 初始化状态
    RoundQueue<PackedFrame> m_buffer_;  // 图像缓冲区
    bool m_thread_active = true;        // 是否激活读图线程

private:
    CameraId m_cam_id_;                 // 相机ID
    int m_cam_type_;                    // 相机类型
};

/**
 * @brief USB相机类
 * 
 */
class USBCamera : public Camera
{
public:
    /**
     * @brief Construct a new USBCamera object
     * 
     * @param id 相机ID
     * @param video_index 操作系统分配的相机设备下标 
     * @param width 宽度
     * @param height 高度
     * @param fps 帧率
     */
    USBCamera(int id, int video_index, int width = 640,
              int height = 480, int fps = 30);

    /**
     * @brief Destroy the USBCamera object
     * 
     */
    ~USBCamera();

    /**
     * @brief 设置USB相机属性
     * 注：具体属性是否支持与相机设备型号有关
     * 
     * @param prop 属性
     * @param value 属性值
     * @return true 
     * @return false 
     */
    bool setCamPropValue(CamProp prop, double value) override;

    /**
     * @brief USB相机初始化
     * 
     * @return true 
     * @return false 
     */
    bool init() override;
    
    /**
     * @brief 打印USB相机能力
     * @todo 待实现
     */
    void printCamCapbility() override {};

protected :
    /**
     * @brief 采集一帧图像
     * 注：底层使用了多线程实现，初始化的时候会启用图像读取线程
     * 
     * @return true 
     * @return false 
     */
    bool read() override;

private:
    cv::VideoCapture m_cap_; // 相机设备
    int m_width_;            // 宽度
    int m_height_;           // 高度
    int m_fps_;              // 帧率
    int m_video_index_;      // 操作系统分配的相机设备下标
};

/**
 * @brief 迈德威视工业相机类
 * 
 */
class MindVisionCamera : public Camera
{
public:
    /**
     * @brief Construct a new Mind Vision Camera object
     * 
     * @param id 相机ID
     * @param sn 设备唯一SN码
     * @param group_id 参数组ID 取值[0-3]
     */
    MindVisionCamera(int id, std::string sn, int group_id);

    /**
     * @brief Destroy the Mind Vision Camera object
     * 
     */
    ~MindVisionCamera();

    /**
     * @brief 设置迈德威视相机属性
     * 具体每个属性取值请参考手册！
     * 
     * @param prop 属性名
     * @param value 属性值
     * @return true 
     * @return false 
     */
    bool setCamPropValue(CamProp prop, double value) override;

    /**
     * @brief 迈德威视相机初始化
     * 
     * @return true 
     * @return false 
     */
    bool init() override;

    /**
     * @brief 更新参数组
     * 每次调用都会读取底层配置文件，不建议频繁调用！
     * 
     * @param group_id 
     * @param file_path
     * @return true 
     * @return false 
     */
    bool setGroupProp(int group_id, std::string file_path);

    /**
     * @brief 设置图像的数字增益值
     * 
     * @param r 红色通道的增益值
     * @param g 绿色通道的增益值
     * @param b 蓝色通道的增益值
     * @return true 
     * @return false 
     */
    bool setDigitalGainPropValue(float r, float g, float b);

    /**
     * @brief 打印迈德威视相机能力信息
     * @todo 待实现
     */
    void printCamCapbility() override {};

    /**
     * @brief Get the Camera Handle object
     * 返回设备句柄，用于实现更灵活的相机操作
     * 
     * @return CameraHandle& 相机设备句柄 
     */
    CameraHandle &getCameraHandle() { return m_handle_; }
    
    // PackedFrame getFrameFromBuffer();

protected:
    /**
     * @brief 采集一帧图像
     * 注：底层使用了多线程实现，初始化的时候会启用图像读取线程
     * 
     * @return true 
     * @return false 
     */
    bool read() override;
    
private:
    CameraHandle m_handle_;             // 相机句柄
    tSdkCameraCapbility m_capbility_;   // 相机能力
    tSdkCameraDevInfo m_dev_info_;      // 设备信息
    BYTE *m_byte_buffer_;               // 原始字节流缓冲区
    unsigned char *m_rgb_buffer_ = NULL;// rgb图像数据缓冲区
    tSdkImageResolution m_image_size_;  // 图像大小
};

#endif