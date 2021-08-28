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

## Reference

使用doxygen在reference目录下生成APIs文档
```
doxygen doc.dot
```

## TODO

3. LUT参数调整
4. ROS模块开发
5. 相机使能信息


## 反馈

邮箱：1085779572@qq.com

