# 关于MLIR编译器

deeplearning学习的框架，关联了计算图的问题

S0 量化 S1Init初始化 S2卷积 S3 ReLu

```
for(h=0;h<H;h++)
    for(w=0;w<W;w++)
        A[h][w] = Quant(A[h][w]) /*S0*/
for(h=0;h<H;h++)
    for(w=0;w<W;w++){
        C[h][w] = 0; /*S1*/
        for(kh=0;kh<kH;kh++)
            for(kh=0;kh<kH;kh++) 
                C[h][w] += A[h+kh][w+kw]*B[kh][kw]; /*S2*/ 这里就是在做一个卷积操作
    }
for(h=0;h<H;h++)
    for(w=0;w<W;w++)
        C[h][w] = ReLU(C[h][w]) /*S3*/ 这里在做ReLU激活

```

TVM开始

TVM 的编译器NNVM 已经更换为Relay

