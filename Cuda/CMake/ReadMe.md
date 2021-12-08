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

