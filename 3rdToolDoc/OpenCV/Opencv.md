#  Opencv的安装

##  源码编译（Version:3.4.12）

### 官方网站下载

下载地址：[地址](https://opencv.org/releases/)

- 安装依赖项：

  ```shell
  sudo apt-get install build-essential
  sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
  sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev #optional
  ```

- [Opencv contrib](https://github.com/opencv/opencv_contrib)拓展包

- https://codechina.csdn.net/mirrors/itseez/opencv_contrib/-/tree/3.4.6

- 使用Cmake 进行编译

  ```shell
  cd ~/opencv
  mkdir build
  cd build
  cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/ubuntu/3rdpart/opencv-3.4.12/opencv_contrib-3.4/modules ..
  ```
 cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/ubuntu/3rdpart/opencv_contrib-3.4/modules ..
  ```shell
  make -j4
  #optional 创建文档
  #cd ~/opencv/build/doc/
  #make -j7 doxygen
  ```

  ```shell
  sudo make install #安装
  ```

  ![end](/home/ubuntu/Pictures/end.png)

  安装完成测试

  ```shell
   pkg-config opencv --modversion
  ```

  ![version](/home/ubuntu/Pictures/version.png)

- 测试用例

  ```shell
  git clone https://github.com/opencv/opencv_extra.git
  ```

  - set OPENCV_TEST_DATA_PATH environment variable to <path to opencv_extra/testdata>.
  - execute tests from build directory.

  ```shell
  <cmake_build_dir>/bin/opencv_test_core
  ```

  - 环境配置
  
  ```shell
  sudo gedit /etc/ld.so.conf
  ```
  
  添加opencv路径
  
  ```
  /usr/local/lib
  ```
  
  ```shell
  sudo ldconfig #生效
  ```
  
  ```shell
  sudo gedit /etc/bash.bashrc
  #在末尾添加
  ##
  PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
  export PKG_CONFIG_PATH
  ##
  source /etc/bash.bashrc
  ```
  
  

##  python-opencv

## YOLO

1. 下载darknet:[AlexyAB](https://github.com/AlexeyAB/darknet)

2. 下载权重

3. 进入下载好的darknet的文件

4. 找到Makefile修改以下内容（前提配置好Opencv Cuda ）

   ![yolo](/home/ubuntu/Pictures/yolo.png)

5. 直接使用

   ```shell
   make
   #如若需要重新编译
   make clean
   ```

6. 对编译好的进行测试

   ```shell
   ./darknet detect cfg/yolov3-tiny.cfg yolov3-tiny.weights data/dog.jpg
   #注意cfg文件与weights文件路径
   ```

## 问题

#### 卸载

```shell
sudo make uninstall
```

#### 报错
opencv/modules/stitching/CMakeLists.txt文件中加入一条语句使其include opencv_contrib/modules/xfeatures2d/include，可以是绝对路径，如

INCLUDE_DIRECTORIES("/home/spring/Soft/opencv3.4.2/opencv_contrib/modules/xfeatures2d/include"


-  fatal error: boostdesc_bgm.i: No such file or directory

  文件缺失直接第三方下载：[链接](https://pan.baidu.com/s/1BeYF8kqEZLAJYQj-MvxpmA)    密码：e1wc

  放入目录：(homelocations)/opencv-3.4.12/opencv_contrib-3.4/modules/xfeatures2d/src cmake时候设置的 contrib modules位置

- fatal error: opencv2/xfeatures2d/cuda.hpp: No such file or directory  #  include "opencv2/xfeatures2d/cuda.hpp"

解决方法1：找到文件，根据文件所丢失的文件进行绝对路径指引

解决办法2：找到CmakeLists 路径为：opencv/modules/stitching/CMakeLists.txt

添加

```txt
INCLUDE_DIRECTORIES("×[路径]×××/opencv_contrib/modules/xfeatures2d/include")
```

然后重新编译

```shell
make clean 
make -j4
```

- locate 

```shell
locate  libopencv_highgui.so.3.4 #不存在
# 环境配置问题
error while loading shared libraries:
libcuda.so.1: cannot open shared object file: No such file or directory


这个文件在 usr/local/nvidia/lib64/(不一定)直接locate

下面，把路径加入到 ld.so.conf.d/cuda.conf中就行了

不过要用一下ldconfig命令,重新链接一下。

mark下，自己再碰到别忘了奋斗。
```

/home/file/opencv/opencv_contrib-3.4/modules/xfeatures2d/src

/home/file/opencv/opencv_contrib-3.4/modules/xfeatures2d/include

cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/file/opencv/opencv_contrib-3.4/modules ..

echo 

sed -i
