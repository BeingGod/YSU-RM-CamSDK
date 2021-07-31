<!--
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-31 11:31:12
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-31 11:42:03
 * @Description: 
-->
# 迈德威视相机工具



## 概述

​	迈德威视提供了一个可视化参数调整软件方便开发者调整工业相机的参数。



## 安装

1. 编译

   ```bash
   cd ./tools/GTK_Ddemo
   make
   ```

2. 连接工业相机并执行命令，输入对应的工业相机的实例下标

   ```bash
   ./GTK_Demo
   ```

3. 如果出现下面的画面说明测试成功

   ![gtk_demo](https://github.com/BeingGod/YSU-RM-CamSDK/blob/main/asset/gtk_demo.png)



## 使用

1. 调整参数，具体每个参数的含义以及影响建议参考[相机知识]()和[相机能力]()。

2. 当调整完参数后，点击`Save`，`GTK_Demo\Camera\Configs`目录下会出现1个`.config`文件，该文件即为当前参数组的参数文件。
3. 使用时导入其参数文件。



注意：GTK_Demo无法修改分辨率参数，同时修改了的分辨率参数也GTK_Demo中也无法生效。

