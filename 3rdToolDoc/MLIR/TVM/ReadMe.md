# TVM

[reference giantpandacv](http://www.giantpandacv.com/project/%E9%83%A8%E7%BD%B2%E4%BC%98%E5%8C%96/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8/%E3%80%90%E4%BB%8E%E9%9B%B6%E5%BC%80%E5%A7%8B%E5%AD%A6%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8%E3%80%91%E4%B8%80%EF%BC%8C%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8%E5%8F%8ATVM%E4%BB%8B%E7%BB%8D/)

简单来说，TVM的作用包含了LLVM后期也被叫做Relay然后MLIR后期被融入了LLVM中，具体时间点，需要参考其余文章。

例如 Intel CPU/Nvidia GPU/Intel GPU/Arm CPU/Arm GPU/FPGA/NPU(华为海思)/BPU(地平线)/MLU(寒武纪)，如果我们要手写一个用于推理的框架在**所有**可能部署的设备上都达到**良好的性能并且易于使用**是一件非常困难的事。

于是产生了TVM，具体图参考giantpandacv里的引用蓝色（知乎）的图

![编译器抽象](https://img-blog.csdnimg.cn/20210316202036555.png)

![深度学习编译器抽象](https://img-blog.csdnimg.cn/20210316203002990.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2p1c3Rfc29ydA==,size_16,color_FFFFFF,t_70)

## 环境配置

参考

[官方文档TVM](https://tvm.apache.org/docs/install/index.html)

[giantpandcv](http://www.giantpandacv.com/project/%E9%83%A8%E7%BD%B2%E4%BC%98%E5%8C%96/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8/%E3%80%90%E4%BB%8E%E9%9B%B6%E5%BC%80%E5%A7%8B%E5%AD%A6%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8%E3%80%91%E4%B8%80%EF%BC%8C%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8%E5%8F%8ATVM%E4%BB%8B%E7%BB%8D/)

直接拉取docker镜像的方式

直接拉取

```
docker pull tvmai/demo-gpu
nvidia-docker run --rm -it tvmai/demo-gpu bash
```

官方

```
git clone --recursive https://github.com/apache/tvm tvm
/path/to/tvm/docker/build.sh tvmai/demo-gpu
```

下面的方式会将你git的目录映射到docker环境中，方便使用

环境配置

```
git clone --recursive https://github.com/apache/tvm tvm
cd tvm
mkdir build
cp cmake/config.cmake build
cd build
cmake ..
make -j4
export TVM_HOME=/path/to/tvm
export PYTHONPATH=$TVM_HOME/python:${PYTHONPATH}
```

踩坑：

python3 

**from tvm import te 出现动态库和目录error，环境问题**

开始学习过程主要处于，文档和博客来回跳跃，毕竟giantpandacv提供的博客，还是不是很好能解释代码，故会查看代码。

## scheduler 

对于模型有计算图，即Tensor在Con和pool的过程，但是在不同的平台上，需要转换为Graph IR（Relay LLVM）,通过TVM提供的指令翻译成特定的硬件下的代码，具体图计算优化的方式可以参考**矩阵运算优化**的方式，giantpandacv里也有，对于**im2col，特殊层的汇编优化**等方式。

[接口reference](https://tvm.apache.org/docs/reference/api/python/te.html#tvm.te.compute)

[查看giantpandacv的使用](http://www.giantpandacv.com/project/%E9%83%A8%E7%BD%B2%E4%BC%98%E5%8C%96/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8/%E3%80%90%E4%BB%8E%E9%9B%B6%E5%BC%80%E5%A7%8B%E5%AD%A6%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E7%BC%96%E8%AF%91%E5%99%A8%E3%80%91%E4%BA%8C%EF%BC%8CTVM%E4%B8%AD%E7%9A%84scheduler/)

0x02节中，关于如何将一个卷积运算转换为GPU代码

```
import tvm
import tvm.testing
from tvm import te
import numpy as np

# 全局环境定义

tgt_host = "llvm"
# 如果启用了GPU，则将其更改为相应的GPU，例如：cuda、opencl、rocm
tgt = "cuda"

n = te.var("n") #Create a new variable with specified name and dtype
A = te.placeholder((n,), name="A")#Construct an empty tensor object.
B = te.placeholder((n,), name="B")
C = te.compute(A.shape, lambda i: A[i] + B[i], name="C")#Construct a new tensor by computing over the shape domain.
print(type(C))

```

到了如上步骤，进行了如下运算

```
for (int i = 0; i < n; ++i) {
  C[i] = A[i] + B[i];
}
```

C这个为Tensor但是为class 'tvm.te.tensor.Tensor'

C.op 指代The symbolic description of the intrinsic operation

te.create_schedule  指代Specialized condition to enable op specialization. 返回一个*class* tvm.te.Schedule[¶](https://tvm.apache.org/docs/reference/api/python/te.html#tvm.te.Schedule)

这个类就调用split(*loop: tvm.tir.schedule.schedule.LoopRV*, *factors: List[Optional[Union[[int](https://docs.python.org/3/library/functions.html#int), [tvm.ir.expr.PrimExpr](https://tvm.apache.org/docs/reference/api/python/ir.html#tvm.ir.PrimExpr)]]]*) → List[tvm.tir.schedule.schedule.LoopRV][¶](https://tvm.apache.org/docs/reference/api/python/tir.html?highlight=split#tvm.tir.Schedule.split)官方文档里面有很好的解释

```
s = te.create_schedule(C.op)
bx, tx = s[C].split(C.op.axis[0], factor=64)
```

转换成如下形式

```
for (int bx = 0; bx < ceil(n / 64); ++bx) {
  for (int tx = 0; tx < 64; ++tx) {
    int i = bx * 64 + tx;
    if (i < n) {
      C[i] = A[i] + B[i];
    }
  }
}
```

最后绑定

```
if tgt == "cuda" or tgt == "rocm" or tgt.startswith("opencl"):
    s[C].bind(bx, te.thread_axis("blockIdx.x"))
    s[C].bind(tx, te.thread_axis("threadIdx.x"))
```

生成

```
fadd = tvm.build(s, [A, B, C], tgt, target_host=tgt_host, name="myadd")
```

## ONNX->TVM前端

最新代码git clone下

error：

RuntimeError: LLVM version is not available, please check if you built TVM with LLVM

solve：

关于编译的过程中，需要将LLVM进行编译，参考官方文档

[llvm-buildmethod](https://tvm.apache.org/docs/install/from_source.html#tvm-package)

[llvm-install](https://releases.llvm.org/13.0.1/docs/GettingStarted.html)

但是docker容器没有这个就比较奇怪，暂时未解决

参考[TVM文档](https://tvm.apache.org/docs/install/from_source.html#building-with-a-conda-environment)中编译的过程设置了llvm，docker镜像里设置了llvm-config-9等配置仍然存在未引用
