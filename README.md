# Ray-Tracing-software-renderer
该仓库项目由于版本过老和为止bug较多**已经停止更新**，最新请移步：[BlokCAT/PacthTracing: A soft ray tracing renderer based on Monte Carlo integration path tracing, implemented using pure C++ and file output streams.使用纯C++和文件输出流实现的一个基于蒙德卡罗积分路径追踪的软光线追踪渲染器 (github.com)](https://github.com/BlokCAT/PacthTracing)

-----------------------------



*注意：本项目旨在学习和分享图形学基础和光线追踪路径追踪底层原理 ， **固该项目并未使用任何图形API！***

*使用纯C++和文件输出流实现的一个基于蒙德卡罗积分路径追踪的软光线追踪渲染器，实现了简易的BVH的加速结构，实现了透射折射材质 。欢迎大佬来提出优化建议和指出错误*



## 结果
![image](https://github.com/BlokCAT/Ray-Tracing-software-renderer/blob/main/showw.png?raw=true)

这图片尺寸500*500 ， 采样率spp300


透射图


![image](https://github.com/BlokCAT/Ray-Tracing-software-renderer/blob/main/1.png?raw=true)


## 实现

### 加速结构：
基于BVH加速树结构，划分区域三角形，树查找减少求交点遍历三角形或物体的个数


### 实现的材质：

  1.Diffuse材质（墙壁）

  2.微表面材质（绿球）

  3.全镜面反射材质（红球）

  4.折射透射材质



### 实现的物体：

  1.继承至`Object`的球体`Boll`

  2.继承至`Object`的平面`Plane`

  

全项目实现思路基本和games101课程介绍一至 ，链接：https://www.bilibili.com/video/BV1X7411F744?p=13&vd_source=1f1ca9dc40200ce4ab07e6e9da88e6b6
