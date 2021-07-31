<!--
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-31 11:31:12
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-31 11:57:17
 * @Description: 
-->
# RM相机模块



## 概述

​	RM相机模块封装了迈德威视工业相机，普通USB相机，双目USB相机等相机，并提供一套统一的调用接口方便上层程序访问并使用相机设备。其主要特点如下：

1. 高性能（底层使用多线程提高图像读取速度）;
2. 便捷性（所有相机提供统一接口）



## 模块架构

​	整体模块架构如下：

![UML](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/asset/UML.png)

1. CameraManager提供集成化访问各类相机设备的途径;
2. 具体相机类继承Camera抽象相机类



## 项目架构

```bash
.
├── asset // 资源
│   ├── mindvision_linuxSDK.zip
│   └── ...
├── Camera // 迈德威视相机文件
│   ├── Configs // 迈德威视相机参数文件
│   │   ├── MV-SUA133GC-Group0.config
│   │   └── ...
│   ├── Data
│   │   └── 049032410170.mvdat
│   └── log
├── camera.xml // 配置文件
├── docs // 文档
├── example // 示例代码
├── include
│   ├── camera
│   │   ├── camera_manager.h
│   │   └── camera_wrapper.h
│   ├── log.h
│   ├── round_queue.h
│   ├── status_code.h
│   └── systime.h
├── load_env.sh // 配置环境变量
├── Makefile 
│   ├── Makefile.mindvision
│   └── Makefile.usb
├── obj // 编译中间文件
├── lib // 生成静态库文件
├── src 
│   ├── camera
│   │   ├── camera_manager.cpp
│   │   ├── mindvision_camera.cpp
│   │   └── usb_camera.cpp
│   └── systime.cpp
├── tools // 工具包
│   └── GTK_Demo // 相机调参
│       └── ...
└── README.md

```



## 帮助

1. [快速上手](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/docs/install.md)
2. [相机能力（待补充）](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/docs/camera_capability.md)
3. [迈德威视相机工具](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/docs/mindvision_tools.md)
4. [注意事项](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/docs/some_traps.md)
5. [工业相机知识](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/docs/knowledge.md)



## TODO

3. LUT参数调整
4. ROS模块开发
5. 相机使能信息



## 反馈

邮箱：1085779572@qq.com

