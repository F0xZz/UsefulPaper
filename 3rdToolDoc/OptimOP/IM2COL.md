# 

算子方向的im2col

reference:

[giantpandacv im2col理解](http://www.giantpandacv.com/project/%E9%83%A8%E7%BD%B2%E4%BC%98%E5%8C%96/AI%20%E7%A7%BB%E5%8A%A8%E7%AB%AF%E7%AE%97%E6%B3%95%E4%BC%98%E5%8C%96/%E3%80%90%E8%AF%A6%E7%BB%86%E5%9B%BE%E8%A7%A3%E3%80%91%E5%86%8D%E6%AC%A1%E7%90%86%E8%A7%A3im2col/)

首先im2col的目的是为了减少gemm的调用，gemm也可称之为矩阵乘法，gemm调用减少，代码循环就少，在某种程度上可以增加一定运算速度，利用汇编Neon和指令集等操作配合多行运算达到加速的目的。

通俗解释参考：例子是一个 [1, 6, 6] 的输入，卷积核是[1, 3, 3]，stride 等于 1，padding 等于 0。那么卷积的过程可视化如下图，一共需要做 16 次卷积计算，每次卷积计算有 9 次乘法和 8 次加法。
