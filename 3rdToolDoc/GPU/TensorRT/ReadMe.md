#

起因

首次接触一开始2021-04月份从FastReid框架中了解了Trt方向，但是由于惧于C++，和大量篇幅的英文文档，故而未尝试。

基于2021-09月份尝试TensorRT,当时仅仅尝试了YoloX方向的部署，YoloX的有用torch2trt的方式，但是尝试未果，故后来尝试Onnx2trt的方式，尝试即可。

YoloX 具体操作方式知乎有可查，需要注意导出的名称对齐。

后续

零零散散接触了Trt的项目，但是仍未有系统化的进行尝试。故而准备尝试系统性的进行尝试部署。关于序列化和反序列化问题。

环境

TensorRT 7.1.x +Cuda 10.2

系统不太受限，由于仅仅推理不训练，Windows下也可开发

Windows 下肯定使用Visual studio或者Cmake的方式进行debug和开发

官方文档

[TensorRT](https://developer.nvidia.com/tensorrt)

使用文档

API 文档

Start Learning baseknowledge

关于查询，不同迭代版本，请查看Archives

1. 首先得申明 Basic knowledge:TensorRT is a C++ library for high performance inference on NVIDIA GPUs and deep learning accelerators.    

   该框架for Nvidia GPUs not CPUs ，想CPU使用TensorRT的先看Framework abstract 已经申明没有CPUs，而且不开放源码，所以内部实现不呈现优化过程，而且对于不同显卡不同设备，需要特定去重新生成推理engine or trt .

2. 关于版本控制问题，TensorRT 是不考虑向下兼容的问题的，只管大步向前迭代，当然某些弃用的接口，肯定是能找到替代的，所以需要对版本进行控

   制。关于算子支持的问题，TensorRT是按照Onnx parser为基础进行拓展支持的。TensorRT支持为Onnx1.6 算子，所以在使用前确认Onnx version 或者

   Pytorch算子支持情况是一件非常重要的事。

3. 由于一开始就有使用过的Trt的方式，但是一直未有系统化的进行训练。从文档开始阅读起、简易的方式C++ 和Python接口调用的方式

实在不想进行接口尝试，官方提供了Samples代码，呈现了Onxx parser的使用

本文主要关于C++接口主要概述，以后慢慢填坑。

首先看文档和Sample，文档一开始说明了Python和C++近乎相同，但是C++ as a performance in solution 实际上更多会部署和使用高性能下场景。但是预处理部

分，python存在良好的矩阵运算库，类似的如Numpy Scipy。

如下形式，如何创建

```
In order to run inference, you need to use the `IExecutionContext` object. In order to create an object of type `IExecutionContext`, you first need to create an object of type `ICudaEngine` (the engine). 
```

[Instantiating TensorRT Objects in C++](https://docs.nvidia.com/deeplearning/tensorrt/archives/tensorrt-700/tensorrt-developer-guide/index.html#initialize_library)

从这开始实例化推理对象，创建方式有两种，如下方式不翻译了，大致就是手动定义网络，方便后续使用，或者直接通过读取生成好的engine，通过bytes passed

```
1.via the network definition from the user model. In this case, the engine can be optionally serialized and saved for later use.
2.by reading the serialized engine from the disk. In this case, the performance is better, since the steps of parsing the model and creating intermediate objects are bypassed.
```

对应方式两种

直接parser解析，这个对应sample中的sampleMNINST

```
The easiest way to achieve this is to import the model using the TensorRT parser library, which supports serialized models in the following samples: 
```

第二种构建网络api 对应SampleMNISTAPI

```
An alternative is to define the model directly using the TensorRT API. This requires you to make a small number of API calls to define each layer in the network graph and to implement your own import mechanism for the model’s trained parameters.
```



先讲第一种开始Code，每次创建推理Engine的过程，需要有个ILogger type

```
class Logger : public ILogger           
 {
     void log(Severity severity, const char* msg) override
     {
         // suppress info-level messages
         if (severity != Severity::kINFO)
             std::cout << msg << std::endl;
     }
 } gLogger;
```

接下来需要配合API文档，Sample，使用文档进行使用

关键性code构建GLogger ，在推理的过程中作为msg传递，类似一个logger信息记录类

参考开始

首先需要构建ICudaEngine，才能创建IExecutionContext

ICudaEngine有两种方式，后续通过该方式构建IExecutionContext

第一种方式

```
//Builds an engine for the given INetworkDefinition and given IBuilderConfig
//It enables the builder to build multiple engines based on the same network definition, but with different
//! builder configurations.
nvinfer1::ICudaEngine* buildEngineWithConfig(
        INetworkDefinition& network, IBuilderConfig& config)
```

第二种方式

```
\brief Build a CUDA engine from a network definition.
//!
//! \see INetworkDefinition ICudaEngine
//!
//! \depercated API will be removed in a future release, use IBuilderConfig::buildEngineWithConfig instead.
//!
nvinfer1::ICudaEngine* buildCudaEngine(
        nvinfer1::INetworkDefinition& network)
```

