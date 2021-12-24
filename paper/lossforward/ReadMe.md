#LabelSmooth

论文:[When Does Label Smoothing Help?](https://arxiv.org/abs/1906.02629)

参考代码:[labelSmooth](https://github.com/kleyersoma/Keras_Label_Smoothing)

中间存在各种pytorch版本

参考了GiantPandaCV的文章

$ \Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,. $

logSoftMax-->其实就是softmax取对数

$ loss=-\sum_{i=1}^ny_ilog(x_i)$

**LabelSmooth** 

**作用**

主要降低SoftMax所带来的高confidence的时候，让模型略微关注到了低分辨率分布的权重。这样做的目的让模型可以更加关注低概率的分布的权重。这么做会导致置信度相对降低。

假设设定一个比例$\epsilon=0.1$

那么就有$y(i)={\epsilon\over\ n} $ 和目标不等的情况$y(i)=1-\epsilon+{\epsilon\over\ n}$和目标相等的时候

这里就权重生成可放在CELoss

代码中

```
import torch 
import torch.nn as nn
import torch.nn.functional as F
class LabelSmoothingCrossEntropy()
```

