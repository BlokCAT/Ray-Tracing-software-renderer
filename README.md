# Ray-Tracing-software-renderer
## 简介
使用纯C++和文件输出流实现的一个基于蒙德卡罗积分路径追踪的软光线追踪渲染器，结构清晰，体量中等，比较适合学习参考，非常欢迎大佬来提出优化建议和指出错误，目前未实现类似BVH的加速结构，后续更新火速填坑awa
## 结果
![image](https://github.com/BlokCAT/Ray-Tracing-software-renderer/blob/main/show.png?raw=true)

这图片尺寸500*500 ， 采样率spp300


## 实现

实现的加速结构：
后续补上

实现的材质：

  1.Diffuse材质（墙壁）

  2.微表面材质（绿球）

  3.全镜面反射材质（红球）


实现的物体：

  1.继承至`Object`的球体`Boll`

  2.继承至`Object`的平面`Plane`

全项目实现思路基本和games101课程介绍一至 ，链接：https://www.bilibili.com/video/BV1X7411F744?p=13&vd_source=1f1ca9dc40200ce4ab07e6e9da88e6b6
