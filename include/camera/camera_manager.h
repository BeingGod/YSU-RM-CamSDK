/*
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-01-28 17:01:42
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-30 11:32:41
 * @Description: 相机管理类
 */

#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

#include <camera/camera_wrapper.h>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <string>

typedef std::pair<Camera *, std::string> CameraResource;
typedef std::unordered_map<CameraId, CameraResource> CameraResourceList; // ID2相机

/**
 * @brief 相机管理类
 * 
 * @todo 待实现
 */
class CameraManager
{
public:
    /**
     * @brief Destroy the Camera Manager object
     * 
     */
    ~CameraManager();

    /**
     * @brief 读取相机XML文件
     * 
     * @param path XML文件路径
     * @return true 
     * @return false 
     */
    bool readCameraParam(std::string path);

    /**
     * @brief 添加相机设备
     * 
     * @param ptr
     * @param des 设备描述
     * @return true 
     * @return false 
     */
    bool addCameraDevice(Camera *ptr,std::string des="");

    /**
     * @brief 移除相机设备
     * 
     * @param id 设备ID
     * @return true 
     * @return false 
     */
    bool removeCameraDevice(CameraId id);

    /**
     * @brief 打印列表中所有相机设备
     * 
     * 输出格式:
     * {
     *     {
     *         设备ID:
     *         设备类型: 
     *         设备描述:
     *     },
     * 
     *     {
     *         设备ID:
     *         设备类型:
     *         设备描述: 
     *     },
     * }
     * 
     */
    void printAllCameraDevice();

    /**
     * @brief 获取对应ID的相机设备指针
     * 
     * @param id 
     * @return Camera* 
     */
    Camera *getCameraDevice(CameraId id);

    /**
     * @brief 重载[]
     * 
     * @param id 
     * @return Camera* 
     */
    Camera *operator[](CameraId id);

private:
    /**
     * @brief 判断该设备是否已存在
     * 
     * @param id 设备ID
     * @return true 
     * @return false 
     */
    bool isCameraDeviceExist(CameraId id);

private:
    CameraResourceList m_camera_list_; // 相机设备资源列表
};

#endif