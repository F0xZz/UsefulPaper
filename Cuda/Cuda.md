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

