# **UsefulPaper**

目的是用于新手学习，和个人阅读的培养记录。

鄙人新手写的很一般，有感兴趣的欢迎PR自己的阅读paper，也欢迎和我联系Wx:9855828188

**关于目前自己能力**

1. 编程能力

   Python(入门)/C++(入门)/Cuda(了解)

   Python 看得懂网络层级结构和前后处理，可根据需求一定程度上更改模型和部分结构，pytorch框架居多

   使用python构建过多种脚本，半自动Reid标定脚本(非开源)，转换脚本，数据增强脚本等等

   C++能力，部署简易模型，用过NCNN，TensorRT，Opencv，Darknet等框架。

   用过Darknet框架训练分类后部署作为接口，以作为后续二次判定。

   用过C++部署过NCNN模型，TensorRT模型，封装成接口。

   写过C++配合QT搭建界面，多线程库，记录Log类，配合信号槽等机制构建应用模块。

   项目为，车辆拥堵检测，车牌识别，火灾检测等模块(非开源)，语言为C++。

   Cuda了解，用过和学习过Cuda模块。

2. 工程能力，自己构建数据集，训练，数据清洗，数据分布调整等工作。

   docker的构建，写过dockefile，训练环境等。

   构建过reid数据集，检测数据集，分类数据集等。

3. 图像方向：较为熟悉传统算法，目标检测，分类方向。语义分割，NLP，细粒度分析了解不深。

4. 会记录文档，有良好记录习惯和自主学习和阅读paper能力。

**目前感觉分支方向**

个人分析目前特点

1. 模型搭建 ----> 开发新的算法模型，如果仅仅使用模型搭建构建网络，那将会处于一种试错的状态或者拼接的状态，感觉目前本科生入门新手即可构建所需的python算法，转部署仍需要考虑一定的优秀的**编程能力**
2. 部署 --->良好的编程能力和多线程，多进程，内存管理任务的分配，还有触发逻辑等因素。
3. 量化优化模型结构 考虑到不同平台的可迁移性，大部分会摆脱QT，而专用**Cmake**，构建项目，**量化**，从模型结构来说，仅仅考虑底层架构Arm可以用**NCNN**，MNN，TNN等，nvidia平台大部分会选择**Cuda**即**TensorRT**模块，其实也可以使用vulkan或者opengl进行调用。模型优化，优化有**剪枝（复杂的过程）**，**模型结构直接更改**等，没有良好的数学基础的情况下，纯尝试堆积。而MobileNet是一种良好的降计算量结构。
4. 算法全栈：从搭建到训练到量化和优化到部署，每一项的步骤都离不开**编程能力**。数据集构建能力（非单纯的采集），而是**后处理部分（特征提取部分）和样本现实状况分析能力**。**“炼丹”能力**（训模能力）不单单说从大模型到小模型的尝试，而是对于过拟合欠拟合的分析能力，数据分布分析能力，对于不同的模型如何最优化参数选择能力。**量化优化能力**，目前大部分服务器采用异构计算，x86+nvidia的平台下模型可以大小要求不高，如果考虑到部署于arm端侧模型就需要在一定程度上损失精度去降低模型参数量和计算量问题以达到提速的要求。**部署（需要对线程，内存，堆栈读写等情况进行合理分析，了解瓶颈所在）**情况就考虑到上述部署平台，根据实际情况下选择，作为一个推理框架，就需要后处理逻辑进行对特定需求筛选能力，并合理的将模块隔离，单模块可拆卸形式。

## easytalk

This use to store some paper that i readed and think this will be useful for myself 

maybe some personal advice in the github 

## Target

wanna Each one can add the code explanation with paper and help the each one to make easier to read code and paper 

## Topic

Paper with code explanation~!! Not paper with code ~! Use for the greenHand just like me 

### DataAug

---

This topic of Data Augments, may be used in the pre process of data .

Some paper has shown the behind of this 

### UseFul Paper

---

#### laneDetection

UtralFastLaneDetection

#### ObjectDetection

YoloRet

---

pytorch

https://discuss.pytorch.org/
