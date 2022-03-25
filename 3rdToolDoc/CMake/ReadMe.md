# CMake构建

参考文档:

https://www.bookstack.cn/read/CMake-Cookbook/content-chapter2-2.1-chinese.md

书栈网:https://www.bookstack.cn/explore?cid=55&tab=popular

CMake 默认编程语言采用C++可在

编译完后再进行链接，链接保证了程序内部申明内容不出错

```
project(recipe-01 LANGUAGES CXX)
```

编译成静态库

```
add_library(message-objs
  OBJECT
    Message.hpp
    Message.cpp
  )
```

```
add_library(message
  STATIC
    Message.hpp
    Message.cpp
  )
```

条件语句

添加变量Message.hpp/Message.cpp 到 _sources

```
list(APPEND _sources Message.hpp Message.cpp)
```

### 系统自动链接的库

gcc和g++会自动检索 

/etc/ld.so.conf 文件中的内容

在内容中如果存在

```
include /etc/ld.so.conf.d/*.conf
```

关于该目录底下存在如下文件:

```
aarch64-linux-gnu.conf      fakechroot-aarch64-linux-gnu.conf
aarch64-linux-gnu_EGL.conf  fakeroot-aarch64-linux-gnu.conf
aarch64-linux-gnu_GL.conf   libc.conf
cuda-10-2.conf              nvidia-tegra.conf
```

任何一个文件打开，例如cuda-10-2.conf，指向了如下目录。

```
/usr/local/cuda-10.2/targets/aarch64-linux/lib
```

