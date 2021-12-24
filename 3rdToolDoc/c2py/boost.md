#boost

boost作为官方c++ 支持的标准库，提供了各种开放和跨平台可用接口。

其中大部分可采用头文件的形式即可调用，而少部分可采用编译需使用链接动态库。

对于较为import boost::python 库，提供了方便python与c++交互的接口

最近又看到一个pybind11被极力推荐

[pybind11](https://pybind11.readthedocs.io/en/stable/)

---

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
#vulkan

vulkan提供了较为通用API接口关于设备调用

gpuinfo: vulkan.gpuinfo.org 搜索对应最大API

#CUDA

多面向https://www.nvidia.cn/training/instructor-led-workshops/#public-workshop

其中关于某一个程序的测试中利用Profile测试

```
gcc -O2 -g -pg myprog.c 
```

生成可执行文件，通过-O2优化

如果进行分析指令，将会将结果重定向 profile.txt

```
gprof ./a.out >profile.txt
```

通过定位分析得到某个函数or某个步骤所产生损耗占用比，就需要开始考虑是否要进行CUDA并行计算进行优化

对于CUDA debug 采用 GDB method

在计算Host和Device 设备中的代码，如果采用float 

```
float a;
a=a*1.02;
host会将单精度转为双精度
device 端不会进行转换而采用单精度形式
```

关于

设备内存中并行块存在的位置，采用Block 块和Thread配合

```

__global__ void printThreadIndex(float *A,const int nx,const int ny)
{
  int ix=threadIdx.x+blockIdx.x*blockDim.x;
  int iy=threadIdx.y+blockIdx.y*blockDim.y;
  unsigned int idx=iy*nx+ix;

A[idx] = idx;
//printf("%d\n",idx);

  printf("thread_id(%d,%d) block_id(%d,%d) coordinate(%d,%d)"
          "global index %2d ival %2d\n",threadIdx.x,threadIdx.y,
          blockIdx.x,blockIdx.y,ix,iy,idx,A[idx]);
}
```

如果设置为三维矩阵通道对应的位置

```
tid = threadIdx.x + threadIdx.y*blockDim.x + threadIdx.z*blockDim.x*blockDim.y
```

对于GPU运行过程中，并行规约可一定程度上提升其运算并行度

CUDA运行的过程中

传参时候GPU<<<grid,block>>>

类似为grid一个大SM线程块，block将线程块进行划分。

如何对于线性运算的双线性插值等进行Cuda加速

