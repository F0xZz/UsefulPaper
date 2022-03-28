# Vscode C++ debug配置

## 0x1基础

CMake基础，C++基础

## 0x2 环境安装

主机电脑,至少需要佩戴gcc，g++，cmake

## 0x3 g++编译

参考文档

vscode download link:https://code.visualstudio.com/

vscode env c++ doc:https://code.visualstudio.com/docs/cpp/config-msvc

控制原理和编译debug原理

官方文档解释:

```
tasks.json (build instructions)
launch.json (debugger settings)
c_cpp_properties.json (compiler path and IntelliSense settings)
```

task.json和launch.json

task.json利用gcc或者g++构建的

参考文档关于[task.json](https://blog.csdn.net/Taynpg/article/details/109072226)

## 0x4 CMake tool

g++手写  task.json方式是非常不方便的一种方式

接下来参考官方文档中[cmake tool in vscode](https://code.visualstudio.com/docs/cpp/cmake-linux) ，具体详解了配置方式，也及其方便，安装如下内容

![C/C++ extension](https://code.visualstudio.com/assets/docs/cpp/cpp/cpp-extension.png)

![CMake tools extension](https://code.visualstudio.com/assets/docs/cpp/cpp/cmake-extension.png)

确认清楚自己cmake 版本和gcc版本，对应不同环境下的编译过程。

## 0x5 vscode中cmake使用

使用方式采用

`ctrl+shitf+p`的方式打开后，进行cmake quick start ，如下的方式，接下来配置基础的cmakelists文件内容，对库和文件进行设定。

![Create CMake quickstart](https://code.visualstudio.com/assets/docs/cpp/cpp/cmake-quickstart-command-palette.png)

编译采用`ctrl+shift+p`中输入如下可看到对应的方式。

`cmake build`

`cmake  debug`

可以选择,debug,release

参考文档 [Select a variant#](https://code.visualstudio.com/docs/cpp/cmake-linux#_select-a-variant)

接下来即可调试
