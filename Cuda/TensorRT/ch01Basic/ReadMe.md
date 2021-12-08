# TensorRT入门

本垃圾，从9月份看文档，看代码，未曾上手，中间陆陆续续看了Cuda，NCNN框架源码，陆陆续续用过使用过，看过代码。NCNN的框架采用的格式目前wts方式不一样，但是原理也不尽相同。NCNN很多层都定义在param中而，目前使用的方案是定义在inference重头开始搭建。

此项目主要参考来自

[pytorch模型](https://github.com/wang-xinyu/pytorchx)

[tensorrtx](https://github.com/wang-xinyu/tensorrtx)

TensorRT API采用 6.0

文档采用官方文档

主要基础知识为Cuda和TensorRT基础限制，Cuda由于部分函数需要将数据运算丢入Device并从Host从取出方式，需要了解内部的情况。

注意TensorRT每个版本所deprecated的接口，虽然未摒弃，但是迟早会摒弃或者存在问题。

## ch?x00

基础环境，需要自己完成。

## ch0x00

官方文档的实际操作

[第三章](https://docs.nvidia.com/deeplearning/tensorrt/developer-guide/index.html#c_topics)作为主要参考，从第一小节开始看

**3.1节**我们需要创建一个Phase，这个Phase采用是ILogger类（本来以为是可直接使用的，没想到是个虚函数，需要采用自己实现）

```
//官方Ilogger类中
virtual void log(Severity severity, const char* msg) TRTNOEXCEPT = 0;
virtual ~ILogger() {}
```

而官方也给了一个demo实例，该方法是在主函数中，创建了一个Logger了继承了ILogger并创建对象logger。

~~当然也可以具体自己实现~~，直接从别人git clone 参考来自tensorrtx

```
class Logger : public ILogger           
{
    void log(Severity severity, const char* msg) override
    {
        // suppress info-level messages
        if (severity <= Severity::kWARNING)
            std::cout << msg << std::endl;
    }
} logger;
```

接下来，查阅下一小节

```
IBuilder* builder = createInferBuilder(logger);
```

可以通过上述，实例化一个网络builder

马上，就告诉需要第一步优化该builder生成器

```
uint32_t flag = 1U <<static_cast<uint32_t>
    (NetworkDefinitionCreationFlag::kEXPLICIT_BATCH) 
INetworkDefinition* network = builder->createNetworkV2(flag);
//本文参考tensorrtx代码为0U也就意味着该函数等于createNetwork().
```

参数具体详细内容参考文档。

后续内容采用为Onnx的格式去实现，实现方式直接能有IParser解析器，对Onnx网络进行解析。

```
#include “NvOnnxParser.h”
using namespace nvonnxparser;
IParser*  parser = createParser(*network, logger);
parser->parseFromFile(modelFile, ILogger::Severity::kWARNING);
for (int32_t i = 0; i < parser.getNbErrors(); ++i)
{
std::cout << parser->getError(i)->desc() << std::endl;
}
```

到这差不多就是TensorRT的入门大致档案。

## ch0x01

主要介绍具体

从生成构建模型到生成dict存储文件到生成推理文件情况。

Engine其实就是二进制文件了

从lenet.py仅仅尝试了conv1pool1两层代码方便直观理解

生成torch.save(net,"lenet.pt")，保存原有模型的结构和参数

