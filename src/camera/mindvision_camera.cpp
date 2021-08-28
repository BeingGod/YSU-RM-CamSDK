/**
 * @file mindvision_camera.cpp
 * @author BeingGod (1085779572@qq.com)
 * @brief 迈得威视工业相机类实现
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <camera/camera_wrapper.h>
#include <sstream>
#include <unistd.h>

using namespace std;
using namespace cam;
using namespace cv;

static bool check(int status)
{
	if (status != CAMERA_STATUS_SUCCESS)
	{
		LOGE("Error status code: %d", status);
		return false;
	}
	else
	{
		return true;
	}
}

MindVisionCamera::MindVisionCamera(int id, string sn, int group_id)
	: Camera(id, CAM_TYPE_MINDVISION)
{
	int try_times = 1;
	do 
	{// 初始化相机SDK
		if (try_times > 5)
		{
			LOGE("Camera SDK init failed, exiting...");
			exit(CAMERA_ERROR);
		}
		LOGW("Initing camera SDK，try %d times...", try_times);
		try_times++;
		usleep(1000000); // 间隔一秒
	} while (SDK_UNSUCCESS(CameraSdkInit(1)));

	try_times = 1;
	do 
	{
		if (try_times > 5)
		{
			LOGE("Not connect camera device, exiting...");
			exit(CAMERA_ERROR);
		}
		LOGW("Connecting camera device, try %d times...",try_times);
		try_times++;
		usleep(500000); // 间隔0.5s
	} while ((CameraEnumerateDeviceEx() == 0));

	int camera_count = CameraEnumerateDeviceEx();
	tSdkCameraDevInfo camera_dev_list[camera_count];

	CameraEnumerateDevice(camera_dev_list, &camera_count);

	bool exist_device = false;
	for (int i = 0; i < camera_count; i++)
	{// 遍历设备列表
		for (int j = 0; j < 32; j++)
		{
			if (sn[j] != camera_dev_list[i].acSn[j])
			{
				break;
			}

			exist_device = true;
		}

		m_dev_info_ = camera_dev_list[i];
	}

	if (!exist_device)
	{
		LOGE("Not exist device. Please check SN code!");
		exit(CAMERA_ERROR);
	}

	memset(&m_image_size_, 0, sizeof(tSdkImageResolution));
	m_image_size_.iIndex = 0xff; // 自定义分辨率
	m_image_size_.iHOffsetFOV = 0;
	m_image_size_.iVOffsetFOV = 0;
	m_image_size_.iWidthFOV = 1280;
	m_image_size_.iHeightFOV = 1024;
	m_image_size_.iWidth = 1280;
	m_image_size_.iHeight = 1024;
}

MindVisionCamera::~MindVisionCamera()
{
	CameraUnInit(m_handle_); // 反初始化
	if (m_rgb_buffer_)
		free(m_rgb_buffer_);
		
	LOGW("MindVision Camera device:%u is released!", getCamId());
}

bool MindVisionCamera::init()
{
	int try_times = 1;
	do 
	{// 初始化相机
		if (try_times > 5)
		{
			LOGE("Camera init failed...");
			return false;
		}
		LOGW("Initing camera device, try %d times...", try_times);
		try_times++;
		usleep(500000); // 间隔0.5s
	} while (SDK_UNSUCCESS(CameraInit(&m_dev_info_, PARAM_MODE_BY_MODEL,
	                                  -1, &m_handle_)));

	// CameraRstTimeStamp(m_handle_);

	// 获取相机使能信息
	if SDK_UNSUCCESS (CameraGetCapability(m_handle_, &m_capbility_)) return false;

	m_rgb_buffer_ = (unsigned char *)malloc(m_capbility_.sResolutionRange.iHeightMax *
											m_capbility_.sResolutionRange.iWidthMax * 3);
	
	if (m_capbility_.sIspCapacity.bMonoSensor)
	{
		CameraSetIspOutFormat(m_handle_, CAMERA_MEDIA_TYPE_MONO8);
	}
	else
	{
		CameraSetIspOutFormat(m_handle_, CAMERA_MEDIA_TYPE_BGR8);
	}

	m_status_ = true; // 初始化成功
	
	if (CameraPlay(m_handle_) == CAMERA_STATUS_SUCCESS)
	{ // 相机初始化成功
		m_status_ = true;

		// 启动图像采集线程
		std::thread t(&MindVisionCamera::read, this);
		t.detach();

		sleep(3); // 让采集线程先填充图像队列

		LOGM("MindVision camera device:%u init success!", getCamId());

		return true;
	}
	else
	{
		return false;
	}

}

bool MindVisionCamera::setCamPropValue(CamProp prop, double value)
{
	int status = CAMERA_STATUS_SUCCESS;
	if (!isInited())
	{
		LOGW("Please init camera first!");
		return false;
	}
	
	switch (prop)
	{
	case 0:
	{// 设置图像宽度
		int max_width = m_capbility_.sResolutionRange.iWidthMax;
		int min_width = m_capbility_.sResolutionRange.iWidthMin;

		m_image_size_.iWidthFOV = value;
		m_image_size_.iWidth = value;
		m_image_size_.iHOffsetFOV = (max_width - value) * 0.5; // 水平偏移量

		return check(CameraSetImageResolution(m_handle_, &m_image_size_));
	}

	case 1:
	{// 设置图像高度
		int max_height = m_capbility_.sResolutionRange.iHeightMax;
		int min_height = m_capbility_.sResolutionRange.iHeightMin;

		m_image_size_.iHeightFOV = value;
		m_image_size_.iHeight = value;
		m_image_size_.iVOffsetFOV = (max_height - value) * 0.5; // 水平偏移量

		return check(CameraSetImageResolution(m_handle_, &m_image_size_));
	}

	case 2:
	{// 设置帧率模式(低速 50%|普通 75%|高速 接近满帧)
	// 注意: 帧率越高，带宽需求越大!
		int speed_mode = static_cast<int>(value);
		
		return check(CameraSetFrameSpeed(m_handle_, speed_mode));
	}
			
	case 3:
	{// 设置水平方向偏移量
		m_image_size_.iHOffsetFOV = static_cast<int>(value);

		return check(CameraSetImageResolution(m_handle_, &m_image_size_));
	}

	case 4:
	{// 设置垂直方向偏移量
		m_image_size_.iVOffsetFOV = static_cast<int>(value);

		return check(CameraSetImageResolution(m_handle_, &m_image_size_));
	}

	case 5:
	{// 设置曝光时间 单位：微妙
		UINT exposure_time = static_cast<unsigned int>(value);
		CameraSetAeState(m_handle_, false);

		return check(CameraSetExposureTime(m_handle_, exposure_time));
	}

	case 6:
	{// 设置对比度
		int constrast = static_cast<int>(value);

		return check(CameraSetContrast(m_handle_, constrast));
	}

	case 7:
	{// 设置白平衡
		return check(CameraSetOnceWB(m_handle_));
	}

	case 8:
	{// 设置模拟增益
		float gain = static_cast<float>(value);

		return check(CameraSetAnalogGainX(m_handle_, gain));
	}

	case 9:
	{// 设置饱和度
		int saturation = static_cast<int>(value);

		return check(CameraSetSaturation(m_handle_, saturation));
	}

	case 10:
	{ // 设置模拟增益(亮度)
		float gain = static_cast<float>(value);

		return check(CameraSetAnalogGainX(m_handle_, gain));
	}

	default:
		LOGW("Mindvision camera not support property!");

		return false;
	}
}

bool MindVisionCamera::setDigitalGainPropValue(float r, float g, float b)
{
	int rgain = r * 100;
	int ggain = g * 100;
	int bgain = b * 100;

	return check(CameraSetGain(m_handle_, rgain, ggain, bgain));
}

bool MindVisionCamera::setGroupProp(int group_id, string file_path)
{
	if (group_id < 0 || group_id > 3)
	{
		LOGW("Not support param group!");
		
		return false;
	}

	int size = file_path.size();
	int file_id = (int)file_path.at(size-8) - 48;

	if (file_id != group_id)
	{
		LOGW("File path not match group id!");

		return false;
	}

	char temp[100];
	strcpy(temp, file_path.c_str());

	return check(CameraReadParameterFromFile(m_handle_, temp)) && 
	       check(CameraLoadParameter(m_handle_, group_id));
}

bool MindVisionCamera::read()
{
	if (!isInited())
	{
		LOGW("Please init camera first!");
		return false;
	}

	PackedFrame packed_frame;
	tSdkFrameHead frame_info;

	while (true)
	{
		if (CameraGetImageBuffer(m_handle_, &frame_info, &m_byte_buffer_, 1000) == CAMERA_STATUS_SUCCESS)
		{
			CameraImageProcess(m_handle_, m_byte_buffer_, m_rgb_buffer_, &frame_info);

			getSystime(packed_frame.time_stamp);
			packed_frame.frame = Mat(
				cvSize(frame_info.iWidth, frame_info.iHeight),
				frame_info.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
				m_rgb_buffer_);

			// packed_frame.time_stamp = static_cast<double>(frame_info.uiTimeStamp);

			m_buffer_.push(packed_frame);
			CameraReleaseImageBuffer(m_handle_, m_byte_buffer_);
		}
	}

	return true;
}
