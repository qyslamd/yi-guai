Yiguai是一只狗，本名叫做“一 一”，因为性格古怪，所以又给它起了一个别名叫做“一怪”
![主界面](https://images.gitee.com/uploads/images/2021/0607/152608_64a155f5_5104064.png "屏幕截图.png")
 **一** 环境搭建
1.Qt环境及Visual studio环境
  从Qt官网：https://www.qt.io/ 上下载 Qt的安装器，下载完成后，安装Qt5.15.2，选择 MSVC2019 32bit 的 kit，当然也可以选择 MSVC2019 64bit的kit，那么cef就应该下载64位的二进制发布包。
由于 cef 的选择的版本是 86.0.21+g6a2c8e7+chromium-86.0.4240.183，由cef wiki推荐的编译器是 MSVC2019，故必须安装Visual Studio 2019。
2.CEF环境
CEF直接从： https://cef-builds.spotifycdn.com/index.html#windows32 上下载二进制包，下载下来的包是CMake构建的，因此需要安装CMake。
3.CEF依赖配置
本项目采用Qt的qmake进行构建，CEF的依赖使用qmake的语法添加。在第2步中使用cmake生成cef.sln后，可使用visual studio打开。然后将libcef_dll_wrapper项目改为MD和MDd编译，编译后得到libcef_dll_wrapper.lib这个静态库。此时只需要整理好依赖的目录结构即可。
目录结构是这样的：
cef_4240_chromium_86
----------> x64
--------------> bin
------------------> Debug
------------------> Release
--------------> include
--------------> libcef_dll_wrapper
------------------> MD
------------------> MDd
------------------> MT
------------------> MTd
--------------> manifests
--------------> Resources

----------> x86
--------------> bin
--------------> include
--------------> libcef_dll_wrapper
--------------> manifests
--------------> Resources
bin/Debug目录下是这样的：
![bin目录下文件（文件夹）结构](https://images.gitee.com/uploads/images/2021/0607/154222_0659350e_5104064.png "屏幕截图.png")
bin/Release目录和bin/Debug目录文件相似，均是从cef的二进制发布包中拷贝而来
include目录是从cef的二进制发布包中拷贝而来
libcef_dll_wrapper目录下放置libcef_dll_wrapper.lib 按照不同的C++运行时库连接规则对应的生成后放到对应目录下
Resources 目录从 cef二进制发布包中拷贝而来

 **然后将组织好的cef_4240_chromium_86文件夹放置到和工程的目录同级的cef_depends目录中，即cef_depends/cef_4240_chromium_86** 

如果不想下载cef二进制发布包，可以私信我 zhouyohu@163.com。看到一般都会回复你的。
或者百度网盘 链接：https://pan.baidu.com/s/1JRt6so9YGyu9WWa6-ifA_g 
提取码：8bw9 
