# vulkan

vulkan提供了较为通用API接口关于设备调用

gpuinfo: vulkan.gpuinfo.org 搜索对应最大API

# CUDA

多面向https://www.nvidia.cn/training/instructor-led-workshops/#public-workshop

[参考github:](https://github.com/Tony-Tan/CUDA_Freshman)

[博客tansheng](https://face2ai.com/program-blog/)

这些都是比较不错的入门方案

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

