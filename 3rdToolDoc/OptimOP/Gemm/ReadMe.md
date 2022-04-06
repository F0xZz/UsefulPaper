#

0x0 引言

最近翻阅知乎，了解到大佬分享的干货，以便于后续的矩阵运算理解

[zz佬](https://www.zhihu.com/people/zz-chris)

[白牛佬的vulkan](https://zhuanlan.zhihu.com/p/487583258)

[白牛佬gemm](https://zhuanlan.zhihu.com/p/65436463)

1x0 OpenBlas

blislab 用于矩阵运算

[reference code](https://github.com/flame/blislab)

[reference doc](https://raw.githubusercontent.com/flame/blislab/master/tutorial.pdf)

官网的内容中需要良好的英文和计算机基础还需要计算机原理

这里

1x1 start

参考代码带更新后 

https://github.com/tpoisonooo/how-to-optimize-gemm

关于开始，拉取分支模块

```shell
git clone https://github.com/tpoisonooo/how-to-optimize-gemm
cd how-to-optimize-gemm
git submodule  update --init
```

首先看到代码的

基础矩阵 C =AxB+C 

传统实现方式

```c++
for ( i = 0; i < m; i ++ ) {
    for ( j = 0; j < n; j ++ ) {
        for ( p = 0; p < k; p ++ ) {
            XC[ j * ldc + i ] += XA[ p * lda + i ] * XB[ j * ldb + p ];
        }
    }
}
```





看着看着

[量化zhihu](https://www.zhihu.com/question/338474672)

[量化文档](https://intellabs.github.io/distiller/algo_quantization.html)

[量化原理](https://arleyzhang.github.io/articles/923e2c40/)

[量化算法](https://jackwish.net/blog/2019/neural-network-quantization-introduction-chn.html)

