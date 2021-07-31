<!--
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-31 11:31:12
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-31 11:58:49
 * @Description: 
-->
# 安装



## 环境

系统：Ubuntu1804 LTS

依赖库：OpenCV 3.4.9，MVSDK 2.1.0.26

编译器：clang 6.0.0



## 注意

1. 如未特别说明，以下所有命令都在项目根目录下执行；
2. 请使用USB 3.0连接迈德威视工业相机；
3. 如果出现USB相机无法打开，或者画面中断的问题，请尝试使用USB 3.0连接。



## USB相机

1. 编译

   ```bash
   cp ./example/test_usb_camera.cpp ./
   cp ./example/Makefile.usb ./Makefile
   source ./load_env.sh
   make
   ```

2. 测试

   ```bash
   ./test_usb_camera
   ```

3. 如下说明USB相机测试成功

   ![usb_test](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/asset/usb_test.png)



## 迈德威视工业相机

1. 安装驱动

   ```bash
   cd ./asset
   unzip ./mindvision_linuxSDK.zip
   sudo ./install.sh
   sudo reboot
   ```

2.  编译

   ```bash
   cp ./example/test_mindvision_camera.cpp ./
   cp ./example/Makefile.mindvision ./Makefile
   source ./load_env.sh
   make
   ```

3. 如下说明迈德威视相机测试成功

   ![mindvision_test](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/asset/mindvision_test.png)



## 快速上手

​	为了简化在上游程序调用`RmCamSDK`的函数接口，使用静态库来简化这一过程，具体步骤如下：

1. 在程序中包含头文件`camera_manager.h`

2. 调用对应的函数接口（详细请参考`demo.cpp`）

   ![demo](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/asset/demo.png)

3. 编译时链接静态库（详细请参考`example/Makefile`）

   ```bash
   g++ -std=c++14 `pkg-config --cflags opencv` -DLINUX ./demo.cpp -I./include -L./lib -lRmCamSDK -lpthread `pkg-config --libs opencv` -lMVSDK -o demo
   ```

   使用-L参数指定`libRmCamSDK.a`文件存放路径，使用-l参数链接`RmCamSDK`静态库文件。
   
   

## 参数文件说明

   本模块支持从xml文件中一次性配置至多三个相机设备，xml文件参数说明如下：

```bash
<?xml version="1.0"?>
<opencv_storage>
    <camera1> #1号相机设备
        <type>MindVision</type> # 相机类型，可选MindVision/USB/null
        <sn>"049032410170"</sn> # 如相机类型为MindVision，需要填写设备SN码
        <description>炮口云台</description> # 相机设备作述
        <param> # 相机设备参数
            <group_id>0</group_id> # 相机类型为MindVision，选填0/1/2/3分别对应四套参数
        </param>
    </camera1>
    <camera2> #2号相机设备
        <type>USB</type>
        <video_index>0</video_index> # 如相机类型为USB，需要填写操作系统分配的设备标志
        <description>底盘双目</description>
        <param>
            <width>640</width> # 如相机类型为USB，需要填写，拍摄图像宽度
            <height>480</height> # 如相机类型为USB，需要填写，拍摄图像高度
            <fps>30</fps> # 如相机类型为USB，需要填写，拍摄图像帧率
        </param>
    </camera2>
    <camera3> #3号相机设备
        <type>null</type> # 为null表示该设备不存在
    </camera3>
</opencv_storage>
```