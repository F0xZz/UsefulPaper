## 阅读

参考文档[Cuda Runtime API](https://docs.nvidia.com/cuda/archive/11.2.2/cuda-runtime-api/group__CUDART__DEVICE.html#group__CUDART__DEVICE)该文档为11.2由于Cuda基础接口保持统一，具体还需要靠查询Cuda文档

对于运行的过程拆分为设备和运行API，一般直接使用RuntimeAPI即可

关于设备运行的查询

参考谭升blog的文档chapter 02-04

```
  int deviceCount = 0;
  cudaError_t error_id = cudaGetDeviceCount(&deviceCount);//该函数用于获得设备个数
  //error_id的返回信息需要用__host__ ​ __device__ ​const char* cudaGetErrorName ( cudaError_t error ) 返回可阅读字符串
```



GPU中每个SM都能支持数百个线程并发执行，每个GPU通常有多个SM，当一个核函数的网格被启动的时候，多个block会被同时分配给可用的SM上执行。

总结起来一句话，想优化速度，先学好怎么用性能分析工具。

- nvvp
- nvprof

这里要讲一下CPU了，当我们的程序包含大量的分支判断时，从程序角度来说，程序的逻辑是很复杂的，因为一个分支就会有两条路可以走，如果有10个分支，那么一共有1024条路走，CPU采用流水线话作业，如果每次等到分支执行完再执行下面的指令会造成很大的延迟，所以现在处理器都采用分支预测技术，而CPU的这项技术相对于gpu来说高级了不止一点点，而这也是GPU与CPU的不同，设计初衷就是为了解决不同的问题。CPU适合逻辑复杂计算量不大的程序，比如操作系统，控制系统，GPU适合大量计算简单逻辑的任务，所以被用来算数。

```
if(con){
//do something
}else{
//do something
}
```

假设这段代码是核函数的一部分，那么当一个线程束的32个线程执行这段代码的时候，如果其中16个执行if中的代码段，而另外16个执行else中的代码块，同一个线程束中的线程，执行不同的指令，这叫做线程束的分化。
我们知道在每个指令周期，线程束中的所有线程执行相同的指令，但是线程束又是分化的，所以这似乎是相悖的，但是事实上这两个可以不矛盾。

解决矛盾的办法就是每个线程都执行所有的if和else部分，当一部分con成立的时候，执行if块内的代码，有一部分线程con不成立，那么他们怎么办？继续执行else？不可能的，因为分配命令的调度器就一个，所以这些con不成立的线程等待，就像分水果，你不爱吃，那你就只能看着别人吃，等大家都吃完了，再进行下一轮（也就是下一个指令）线程束分化会产生严重的性能下降。条件分支越多，并行性削弱越严重。
注意线程束分化研究的是一个线程束中的线程，不同线程束中的分支互不影响。

因为线程束分化导致的性能下降就应该用线程束的方法解决，根本思路是避免同一个线程束内的线程分化，而让我们能控制线程束内线程行为的原因是线程块中线程分配到线程束是有规律的而不是随机的。这就使得我们根据线程编号来设计分支是可以的，补充说明下，当一个线程束中所有的线程都执行if或者，都执行else时，不存在性能下降；只有当线程束内有分歧产生分支的时候，性能才会急剧下降。
线程束内的线程是可以被我们控制的，那么我们就把都执行if的线程塞到一个线程束中，或者让一个线程束中的线程都执行if，另外线程都执行else的这种方式可以将效率提高很多。

看实现代码两种形式

```
__global__ void mathKernel1(float *c)
{
	int tid = blockIdx.x* blockDim.x + threadIdx.x;

	float a = 0.0;
	float b = 0.0;
	if (tid % 2 == 0)
	{
		a = 100.0f;
	}
	else
	{
		b = 200.0f;
	}
	c[tid] = a + b;
}
```



```
__global__ void mathKernel2(float *c)
{
	int tid = blockIdx.x* blockDim.x + threadIdx.x;
	float a = 0.0;
	float b = 0.0;
	if ((tid/warpSize) % 2 == 0)
	{
		a = 100.0f;
	}
	else
	{
		b = 200.0f;
	}
	c[tid] = a + b;
}
```

明显第二种的结果通道是顺序混乱的，但是可以后期错位调整，运算速度是可以得到提升的

![img](https://face2ai.com/CUDA-F-3-2-%E7%90%86%E8%A7%A3%E7%BA%BF%E7%A8%8B%E6%9D%9F%E6%89%A7%E8%A1%8C%E7%9A%84%E6%9C%AC%E8%B4%A8-P1/3_12.png)

这个时候就要拿出

需要有矩阵优化基础Gemm 和Blas方向 im2col等

[Vulkan目的性上手](https://zhuanlan.zhihu.com/p/487583258)

[gemm Cuda](https://zhuanlan.zhihu.com/p/441146275)

```
nvprof --metrics achieved_occupancy ./simple_sum_matrix
nvprof --metrics gld_throughput ./simple_sum_matrix
```

