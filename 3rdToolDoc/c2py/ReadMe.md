# C++ bind with Python

## Boost库

- boost作为官方c++ 支持的标准库，提供了各种开放和跨平台可用接口。

- 其中大部分可采用头文件的形式即可调用，而少部分可采用编译需使用链接动态库。

- 提供了import boost::python，方便了python与c++交互的接口
- 整体Boost较为庞大，不是很推荐使用，故而暂时没有使用

## Pybind11库

最近又看到一个pybind11被极力推荐，NCNN采用Pybind11进行了封装python接口

[pybind11官网](https://pybind11.readthedocs.io/en/stable/)

具体内容参考Pybind文件夹下内容

---

### 出现的问题

- Issues Boost

1.6.5编译过程中出现pyconfig.h cannot find file

原因为boost的原生错误,

需要修改安装脚本python.jam

1.65.1的位置在/boost_1_65_1/tools/build/src/tools/python.jam的547行，

改为

    else
    {undefined
        includes ?= $(prefix)/include/python$(version)m ;
    
        local lib = $(exec-prefix)/lib ;
        libraries ?= $(lib)/python$(version)/config $(lib) ;
    }
