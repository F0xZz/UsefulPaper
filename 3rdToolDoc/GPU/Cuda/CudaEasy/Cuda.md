# 阅读

Cuda和CPU，实际上某种程度上Cuda或者显卡对于大数据量是有一定加速的，但并不是绝对~！！！！，不可乱用Cuda

但！！要考虑中间数据交换层如果因为数据拷贝从内存到显存->开始多并行计算加速->显存到内存，其实大部分时间都是在拷贝的时间上，占用了大部分的时间。

官方文档https://docs.nvidia.com/cuda/index.html

参考学习内容：https://face2ai.com/program-blog/

书籍:[Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA ](https://github.com/PacktPublishing/Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA)

x86 和 GPU 即最常见的异构计算方式

当然也有 arm GPU 

FPGA等等因素

显卡贵的原因还有一点高频显存多，而CPU做交互的

L3 L2 L1缓存 200ns 2ns 0.05ns对应，由于数据缓存和内存交互的因素，导致在大数据量的情况下时间数据交互时间直线提升，GPU能一下子读取更多的数据量，并且并行数高于CPU虽然主频不一定高于CPU但是总体速度得到了大量的提高。

## Basic Line

Cuda针对Nvidia

OpenGL,vulkan针对all more complex

自顶而下Nvidia官方宣称结构如下：

Nvidia

Streamimg multiprocessors(SMs)

Threads

但是实际上

### 使用方式

获得Cuda

离线方式

```shell
sudo dpkg -i cuda-repo-<distro>_<version>_<architecture>.deb //offline install 
```

在线方式

```shell
2.Install the CUDA public GPG key using the following command:
sudo apt-key add /var/cuda-repo-<version>/7fa2af80.pub
3. Then, update the apt repository cache using the following command:
sudo apt-get update
4. Then you can install CUDA using the following command:
sudo apt-get install cuda
```



```shell
5. Include the CUDA installation path in the PATH environment variable using
the following command:
If you have not installed CUDA at default locations, you need to change the path to point at
your installation location.
export PATH=/usr/local/cuda-x.x/bin${PATH:+:${PATH}}
6. Set the LD_LIBRARY_PATH environment variable:
export LD_LIBRARY_PATH=/usr/local/cuda-x.x/lib64\
${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
```

获得带Cuda的Opencv 编译的指令如下

参考指令

```
mkdir build-cuda 
cd build-cuda
cmake  -D CMAKE_BUILD_TYPE=RELEASE                            \
           -D CMAKE_INSTALL_PREFIX=/usr/local              \
           -D WITH_CUDA=ON                                        \
           -D CUDA_GENERATION=Pascal                              \
           -D ENABLE_FAST_MATH=1                                  \
           -D CUDA_FAST_MATH=1                                    \
           -D WITH_CUBLAS=1                                       \
           -D WITH_CUDNN=ON                                       \
           -D WITH_OPENCL=ON                                      \
           -D INSTALL_PYTHON_EXAMPLES=ON                          \
           -D BUILD_EXAMPLES=OFF ..                                   

```

实际指令如下

```
mkdir build 
cd build 
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_CUDA=ON -D CUDA_GENERATION=Turing -D WITH_CUDNN=ON ..
```

目前参考

https://blog.csdn.net/qq_30263737/article/details/80581071

https://blog.csdn.net/qq_30263737/article/details/80581037?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163531600016780262511900%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=163531600016780262511900&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_v2~rank_v29-3-80581037.pc_v2_rank_blog_default&utm_term=cuda&spm=1018.2226.3001.4450

测试算力 算力值查询

https://developer.nvidia.com/zh-cn/cuda-gpus#compute

## 使用Profile进行优化（Profile-Driven Optimization）

中文翻译的这个标题是配置文件驱动优化，驱动这个词在这里应该是个动词，或者翻译的人直接按照字面意思翻译的，其实看完内容以后的意思是根据profile这个文件内的信息对程序进行优化。
性能分析通过以下方法来进行：

1. 应用程序代码的空间(内存)或时间复杂度
2. 特殊指令的使用
3. 函数调用的频率和持续时间

程序优化是建立在对硬件和算法过程理解的基础上的，如果都不了解，靠试验，那么这个结果可想而知。理解平台的执行模型也就是硬件特点，是优化性能的基础。
开发高性能计算程序的两步：

1. 保证结果正确，和程序健壮性
2. 优化速度

Profile可以帮助我们观察程序内部。

- 一个原生的内核应用一般不会产生最佳效果，也就是我们基本不能一下子就写出最好最快的内核，需要通过性能分析工具分析性能。找出性能瓶颈
- CUDA将SM中的计算资源在该SM中的多个常驻线程块之间进行分配，这种分配方式可能导致一些资源成为性能限制因素，性能分析工具可以帮我们找出来这些资源是如何被使用的
- CUDA提供了一个硬件架构的抽象。它能够让用户控制线程并发。性能分析工具可以检测和优化，并肩优化可视化

总结起来一句话，想优化速度，先学好怎么用性能分析工具。

- nvvp
- nvprof

限制内核性能的主要包括但不限于以下因素

- 存储带宽
- 计算资源
- 指令和内存延迟



目前所阅版本 V10.2

Cuda官方文档虽然从架构到机理开始介绍到解释。

但是就编程而言，难点在于如何调教优秀的并行代码而不是如何优先的让他跑起来，大型的矩阵运算，需要并行的计算，而不是简单的串行进行计算。

从文档Figure6中

Grid将其分成了多个Block，而多个Block里涵盖了多个Thread，

截止上次看Cuda是9月份 ，这次10月份再看，好歹能看懂初步内容。关键复杂矩阵的内存地址交换问题还是存在一点问题，果然

C++基础和数据存储方式需要更好的理解和加深。



## 2022.04 chapter02

这次是2022.04月份，我觉得每次回头看知识都会获得新的内容

重头开始看blog内容：这个图对于编程模型和编译器具有一个Communication Abstraction

![img](https://face2ai.com/CUDA-F-2-0-CUDA%E7%BC%96%E7%A8%8B%E6%A8%A1%E5%9E%8B%E6%A6%82%E8%BF%B01/1.png)

编程模型可以理解为，我们要用到的语法，内存结构，线程结构等这些我们写程序时我们自己控制的部分，这些部分控制了异构计算设备的工作模式，都是属于编程模型。（参考谭升blog）

两个内存从硬件到软件都是隔离的（CUDA6.0 以后支持统一寻址）

- cuda和c接口

| 标准C函数 | CUDA C 函数 |   说明   |
| :-------: | :---------: | :------: |
|  malloc   | cudaMalloc  | 内存分配 |
|  memcpy   | cudaMemcpy  | 内存复制 |
|  memset   | cudaMemset  | 内存设置 |
|   free    |  cudaFree   | 释放内存 |

- cudaMemcpyHostToHost
- cudaMemcpyHostToDevice
- cudaMemcpyDeviceToHost
- cudaMemcpyDeviceToDevice

看了第二章较为关键的核函数的控制流，对于grid，block，thread的控制，这张图真好

![img](https://face2ai.com/CUDA-F-2-0-CUDA%E7%BC%96%E7%A8%8B%E6%A8%A1%E5%9E%8B%E6%A6%82%E8%BF%B01/4.png)

其中在初始化核函数进去的时候grid已经定义了

在核函数运行的时候gridDim为一开始

对应dim3 grid 可通过在核函数内部GridDim.x GridDim.y GridDim.z

dim3 block也同理 

```
<<<grid,block>>>
```

参考谭升的代码1_check_dimension

```
#include <cuda_runtime.h>
#include <stdio.h>
__global__ void checkIndex(void)
{
  printf("threadIdx:(%d,%d,%d) blockIdx:(%d,%d,%d) blockDim:(%d,%d,%d)\
  gridDim(%d,%d,%d)\n",threadIdx.x,threadIdx.y,threadIdx.z,
  blockIdx.x,blockIdx.y,blockIdx.z,blockDim.x,blockDim.y,blockDim.z,
  gridDim.x,gridDim.y,gridDim.z);
}
int main(int argc,char **argv)
{
  int nElem=6;
  dim3 block(3);
  dim3 grid((nElem+block.x-1)/block.x);
  printf("grid.x %d grid.y %d grid.z %d\n",grid.x,grid.y,grid.z);
  printf("block.x %d block.y %d block.z %d\n",block.x,block.y,block.z);
  checkIndex<<<grid,block>>>();
  cudaDeviceReset();
  return 0;
}
```

output

```
grid.x 2 grid.y 1 grid.z 1 
//-> dim3 grid(2) same as dim3 grid(2,1,1)
block.x 3 block.y 1 block.z 1 
//-> dim3 block(3) same as dim3 block(3,1,1)
// 这边后续blockDim girdDim比较好理解
// thread block 该部分就好从上chapter中得到，一个核函数为一个grid 所以grid设置好了进行划分了
// checkIndex<<<grid,block>>>();
// 在核函数创建的过程中，会开辟一个网格grid，该网格会先开辟一个block区域，block再被细化为thread
// 最细化为thread->block在网格块中，在网格块中block为gridDim维度对应
// threadIdx 应该是block参数对应
threadIdx:(0,0,0) blockIdx:(1,0,0) blockDim:(3,1,1)  gridDim(2,1,1)
threadIdx:(1,0,0) blockIdx:(1,0,0) blockDim:(3,1,1)  gridDim(2,1,1)
threadIdx:(2,0,0) blockIdx:(1,0,0) blockDim:(3,1,1)  gridDim(2,1,1)
threadIdx:(0,0,0) blockIdx:(0,0,0) blockDim:(3,1,1)  gridDim(2,1,1)
threadIdx:(1,0,0) blockIdx:(0,0,0) blockDim:(3,1,1)  gridDim(2,1,1)
threadIdx:(2,0,0) blockIdx:(0,0,0) blockDim:(3,1,1)  gridDim(2,1,1)
```

由于该sumArraysGPU中已经计算好该一维向量为1<<14 =1<<4 x 1<<10 = 16*1024 

接下来去理解sumArraysGPU中的位置偏移量，由于a和b为一个一维向量，但是被grid切分为block的形式，

原本是在核函数传参<<<grid,block>>>  -------   <<<16,1024>>> 划分为16个block 对应gridDim(16,1,1)  

blockIdx:(0-15,0,0)，每个block被划分为1024个thread，然后就开始解析如下语句，需要有一定的**数据结构和地址理解**

那么再看16*1024个数据刚刚每个数据一个地址不需要重复计算，首先会将数据划分为16块

blockIdx.x该参数为block的第一维0-15变化，blockDim.x为1024其实就是地址偏移，thread.Idx为线程块，

由于一个块只计算一个值，所以底下仅仅跟一个，如果把改成一个线程两个相对应的block 要缩小一半，

如果不信可以进行测试

```
__global__ void sumArraysGPU(float*a,float*b,float*res)
{
  //int i=threadIdx.x;
// if(threadIdx.x==0 || threadIdx.x==1023){
 // printf("blockIdx.x,blockDim.x,threadIdx.x:(%d,%d,%d)\n",blockIdx.x,blockDim.x,threadIdx.x);
  //}
  int i=blockIdx.x*blockDim.x+threadIdx.x;
  res[i]=a[i]+b[i];
}
```

output中为会有，一维的角度来看更好理解了

```
blockIdx.x,blockDim.x,threadIdx.x:(15,1024,0)
blockIdx.x,blockDim.x,threadIdx.x:(9,1024,0)
blockIdx.x,blockDim.x,threadIdx.x:(3,1024,0)
blockIdx.x,blockDim.x,threadIdx.x:(13,1024,0)
blockIdx.x,blockDim.x,threadIdx.x:(1,1024,0)
```

