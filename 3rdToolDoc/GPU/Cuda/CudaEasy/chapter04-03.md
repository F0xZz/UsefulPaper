## 阅读

阅读谭升blog

读书人懂得道：”格物，致知，诚意，正心，修身，齐家，治国，平天下”

格物，制知：欲诚其意者，先致其知，致知而格物，物格而后知至

物有本末，事有终始，知所先后，则近道矣

《大学》知止而后有定，定而后能静，静而后能安，安而后能虑，虑而后能得。物有本末，事有终始。知所先后，则近道矣。

知道应达到的境界才能够志向坚定；志向坚定才能够镇静不躁；镇静不躁才能够心安理得；心安理得才能够思虑周祥；思虑周祥才能够有所收获。每样东西都有根

本有枝末，每件事情都有开始有终结。明白了这本末始终的道理，就接近事物发展的规律了。

个人见解：截止2022.04 做了一年多的深度学习，神经网络，视觉相关的内容，但是仍然未曾好好的去理解，起初我私以为，深度学习和神经网络，会脱离计算机成为一套独立系统，完成自己一套神经模型，最后了解的始末，发现深度学习和神经网络，尚未脱离计算机系统这个体系，所以我认为这神经网络的仍处于中间阶段，类似互联网的一个阶段性爆发。仍然需要摒者计算机的知识，努力提升。

[知乎-李沐](了解/从事机器学习/深度学习系统相关的研究需要什么样的知识结构？ - 李沐的回答 - 知乎 https://www.zhihu.com/question/315611053/answer/2414104379)

而整个学科的本，我认为是数学，当如果哪一天证明，数学的整个体系在人工智能面前崩溃了，各种反公理，反定理的事件都在人工智能中发生了，那就证明我错了。但目前，本还是本。

内存和显存访问的过程中， 在显存中，L1代表一级缓存，每个SM都有自己的L1 catch 但是L2的是所有SM块共用的。

核函数运行的过程中，会从(DRAM)中读取数据，读取数据粒度只存在 128 / 32 粒度

对齐合并访问的状态是理想化的，也是最高速的访问方式，当线程束内的所有线程访问的数据在一个内存块，并且数据是从内存块的首地址开始被需要的，那么对齐合并访问出现了

​	一个线程束加载数据，使用一级缓存，并且这个事务所请求的所有数据在一个128字节的对齐的地址段上（对齐的地址段是我自己发明的名字，就是首地址是粒度的偶数倍，那么上面这句话的意思是，所有请求的数据在某个首地址是粒度偶数倍的后128个字节里），具体形式如下图，这里请求的数据是连续的，其实可以不连续，但是不要越界就好。

![4-6](https://face2ai.com/CUDA-F-4-3-%E5%86%85%E5%AD%98%E8%AE%BF%E9%97%AE%E6%A8%A1%E5%BC%8F/4-6.png)

这里总结一下内存事务的优化关键：用最少的事务次数满足最多的内存请求。事务数量和吞吐量的需求随设备的计算能力变化。

常规的路径是一级和二级缓存，需要使用常量和只读缓存的需要在代码中显式声明。但是提高性能，主要还是要取决于访问模式。
控制全局加载操作是否通过一级缓存可以通过编译选项来控制，当然比较老的设备可能就没有一级缓存。
编译器禁用一级缓存的选项是：

```
-Xptxas -dlcm=cg
```

编译器启用一级缓存的选项是：

```
-Xptxas -dlcm=ca
```

启用一级缓存后，当SM有全局加载请求会首先通过尝试一级缓存，如果一级缓存缺失，则尝试二级缓存，如果二级缓存也没有，那么直接DRAM。

感觉直接[reference](https://face2ai.com/CUDA-F-4-3-%E5%86%85%E5%AD%98%E8%AE%BF%E9%97%AE%E6%A8%A1%E5%BC%8F/)

