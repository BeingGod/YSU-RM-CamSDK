/**
 * @file camera_manager.cpp
 * @author BeingGod (1085779572@qq.com)
 * @brief 相机管理类实现
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <camera/camera_manager.h>

using namespace std;
using namespace cam;
using namespace cv;

CameraManager::~CameraManager()
{
    for (auto it = m_camera_list_.begin();
         it != m_camera_list_.end(); it++)
    {// 释放所有设备
    // 避免内存泄漏
        CameraResource cam_res = it->second;
        cam_res.first->stopReadImage();
        delete cam_res.first;
    }

    m_camera_list_.clear();
}

bool CameraManager::addCameraDevice(Camera *ptr,string des)
{
    CameraId id = ptr->getCamId();
    if (isCameraDeviceExist(id))
        return false;

    CameraResource cam_res = pair<Camera *, string>(ptr, des);
    m_camera_list_.insert(pair<CameraId, CameraResource>(id, cam_res)); // 添加设备

    return true;
}

bool CameraManager::isCameraDeviceExist(CameraId id)
{
    if (m_camera_list_.find(id) != m_camera_list_.end())
    {// 该设备已存在
        LOGE("Camera ID:%u is already exists!", id);
        
        return true;
    }

    return false;
}

bool CameraManager::readCameraParam(string path)
{
    FileStorage fs_read;
    fs_read.open(path, FileStorage::READ);
    if (!fs_read.isOpened())
    {
        LOGE("File is not found!");
        return false;
    }
    
    for (int i = 1; i <= 3;i++)
    {
        stringstream node_name;
        node_name << "camera" << i;
        FileNode cam_node = fs_read[node_name.str()];
        FileNodeIterator cam_node_it = cam_node.begin();

        string type;
        (*cam_node_it) >> type;
        cam_node_it++;
        if (type == "USB")
        {
            int video_index;
            string des;
            int width;
            int height;
            int fps;
            (*cam_node_it) >> video_index;
            cam_node_it++;
            (*cam_node_it) >> des;
            cam_node_it++;
            FileNode param = *cam_node_it;
            FileNodeIterator param_it = param.begin();
            (*param_it) >> width;
            param_it++;
            (*param_it) >> height;
            param_it++;
            (*param_it) >> fps;
            param_it++;

            Camera *ptr = new USBCamera(i, video_index, width, height, fps);
            addCameraDevice(ptr, des);
        }
        else if (type == "MindVision")
        {
            string sn;
            string des;
            int group_id;
            (*cam_node_it) >> sn;
            cam_node_it++;
            (*cam_node_it) >> des;
            cam_node_it++;
            FileNode param = *cam_node_it;
            FileNodeIterator param_it = param.begin();
            (*param_it) >> group_id;
            param_it++;

            Camera *ptr = new MindVisionCamera(i, sn, group_id);
            addCameraDevice(ptr, des);
        }
        else if (type == "null")
        {
            continue;
        }
        else
        {
            LOGE("Not support Camera Type!");
            return false;
        }
    }

    return true;
}

bool CameraManager::removeCameraDevice(CameraId id)
{
    auto it = m_camera_list_.find(id);

    if (it == m_camera_list_.end())
    {// 该设备不存在
        LOGE("Camera ID:%u is not exists!", id);
        return false;
    }

    CameraResource cam_res = it->second;  // 释放设备
    delete cam_res.first;
    m_camera_list_.erase(id); // 移除设备

    return true;
}

Camera *CameraManager::getCameraDevice(CameraId id)
{
    auto it = m_camera_list_.find(id);
    if (it == m_camera_list_.end())
    {
        LOGE("Camera ID:%u is not exists!", id);
        return NULL;
    }
    CameraResource cam_res = it->second;

    return cam_res.first;
}

void CameraManager::printAllCameraDevice()
{
    stringstream sstr;
    sstr << "{\n";
    
    auto it = m_camera_list_.begin();
    for (; it != m_camera_list_.end();it++)
    {
        sstr << "\t{\n";
        
        CameraId id = it->first;
        CameraResource cam_res = it->second;
        int type = cam_res.first->getCamType();
        string des = cam_res.second;
        string type_des;
        if (type == CAM_TYPE_MINDVISION)
        {
            type_des = "迈德威视相机";
        }
        else if (type == CAM_TYPE_USB)
        {
            type_des = "USB相机";
        }
        else
        {
            type_des = "未知相机";
        }
        sstr << "\t\t设备ID: " << id << "\n";
        sstr << "\t\t设备类型: " << type_des << "\n";
        sstr << "\t\t设备描述: " << des << "\n";
        sstr << "\t},";
        sstr << "\n";
    }
    
    sstr << "}";

    cout << sstr.str() << endl;
}

Camera *CameraManager::operator[](CameraId id)
{
    return getCameraDevice(id);
}