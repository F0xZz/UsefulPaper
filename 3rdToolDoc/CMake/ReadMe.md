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

### 关键指令

关于系统环境设置测试

[CMake菜谱](https://www.bookstack.cn/read/CMake-Cookbook/content-chapter2-2.1-chinese.md)

CMakeLists提供了CMAKE_SYSTEM_NAME可供于查找

```
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring on/for Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring on/for macOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Configuring on/for Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    message(STATUS "Configuring on/for IBM AIX")
else()
    message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()
```



关于很多情况的gdb debug

cmake 中需要设置

```
add_compile_options("-g")
```

对于常用计算-Wall覆盖了尽可能的选项警告

-Ofast 官方文档

-Ofast  Disregard    strict    standards    compliance.‘-Ofast’    enables    all    ‘-O3’optimizations.  It also enables optimizations that are not valid for all standard- GCC Command Options109compliant  programs.It  turns  on  ‘-ffast-math’  and  the  Fortran-specific‘-fno-protect-parens’ and ‘-fstack-arrays’

在-O3的基础上，开启了近乎快速的数组计算

-mfpmath

For the x86-32 compiler, you must use ‘-march=cpu-type’, ‘-msse’or ‘-msse2’ switches to enable SSE extensions and make this optioneffective.  For the x86-64 compiler, these extensions are enabled bydefault

Generate floating-point arithmetic for selected unitunit.  The choices forunit

```
add_compile_options(-Wall -Ofast -msse2)
```

对于Header only

相比于普通的添加静态库目标， header-only 库目标的创建，需要添加 `INTERFACE` 关键字：

version 3.17 以上才可以使用

```
add_library(array INTERFACE
    array.hpp
)
```

常用指令

**set([NAME] [VALUE])设定变量**

**option([NAME] [DESCRIPTION] [DEFAULT_VALUE])设置自定义选项**

**if([表达式])…elseif([表达式])…else()…endif() 条件分支**

**aux_source_directory([DIR] [NAME]) 查找某个目录中所有源文件**

**target_link_libraries([OUT] [LIBS])链接库文件**



