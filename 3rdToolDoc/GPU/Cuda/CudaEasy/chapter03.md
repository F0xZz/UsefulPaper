# 阅读

## 2022.04 chapter03

关于cuda计算时间

不可用如下形式，如下形式不可用并行计算的程序

```
clock_t start, finish;
start = clock();
// 要测试的部分
finish = clock();
duration = (double)(finish - start) / CLOCKS_PER_SEC;
```

采用如下形式

```
#include <sys/time.h>
double cpuSecond()
{
  struct timeval tp;
  gettimeofday(&tp,NULL);
  return((double)tp.tv_sec+(double)tp.tv_usec*1e-6);
}
```

使用方式如下

```
  double iStart,iElaps;
  iStart=cpuSecond();
  sumArraysGPU<<<grid,block>>>(a_d,b_d,res_d,nElem);
  cudaDeviceSynchronize();
  iElaps=cpuSecond()-iStart;
```

还有个工具是nvprof

```
nvprof [nvprof_args] <application>[application_args]
```

nvprof的使用参考文档

