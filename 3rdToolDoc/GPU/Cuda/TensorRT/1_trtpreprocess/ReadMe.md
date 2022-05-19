#

最近看到TensorRTx中 yolov5的preprocess部分采用了cuda加速，故而想着采用的yolort的tensorrt中，毕竟两者采用的前处理方式较为统一。

先看Trt的序列化和反序列化方式，采用的是搭建Yolo手动搭建

反序列化过程中，采用

```
IRuntime* runtime = createInferRuntime(gLogger);
//Deserialize an engine from a stream.
//If an error recorder has been set for the runtime, it will also be passed to the engine.
//ICudaEngine * nvinfer1::IRuntime::deserializeCudaEngine(const void *  	blob,std::size_t  	size ) 	
ICudaEngine* engine = runtime->deserializeCudaEngine()
```

trt中获得index采用，使用了如下形式获得了inputIndex 和outputIndex

通过name用于绑定输入输出的blob



```
engine->getBindingIndex(const char* name);
```

在yolort的deployment中，获得index采用的是获取所有的blob

如果想getBindingIndex该内容应该小于engine->getNbBindings()的数量

通过engine->getBindingDimensions该函数获得某index的blob的维度(Dimension)

```
std::vector<void*> buffers(engine->getNbBindings());
传递给引擎的输入输出buffer指针- 需要精确的 IEngine::getNbBindings(),这里1个输入＋4个输出
```

```
Bind[0] {Name:images, Datatype:0, Shape:(1,3,640,640,)}
Bind[1] {Name:num_detections, Datatype:3, Shape:(1,1,)}
Bind[2] {Name:detection_boxes, Datatype:0, Shape:(1,100,4,)}
Bind[3] {Name:detection_scores, Datatype:0, Shape:(1,100,)}
Bind[4] {Name:detection_classes, Datatype:3, Shape:(1,100,)}
```

从yolov5中使用cudaStream_t stream

用于cuda计算流，使得首先声明一个Stream，可以把不同的操作放到Stream内，按照放入的先后顺序执行。由于yolov5的处理放入了preprocess中buffer这个buffer后续作为数据传输，binding计算流Index，在计算过程中binding了input 和 output blob



yolort，分析detect程序

一开始std::vector\<Detection> result用于存储detect output

中间获得std\<vector> buffers(engine->getNbBindings());用于获得所有的binding blob 个数，在c++上并未体现，故而去查看python代码，其实在转engine的过程中采用的方式为onnx转engine 但是在转onnx会生成trt.onnx用于将yolo detect 部分decode，查看代码python，绑定了4个outputblob

中间经历Malloc    

```
CHECK(cudaMalloc(&buffers[i], buffer_size * getElementSize(engine->getBindingDataType(i))));
```

Internally, `torch.onnx.export()` requires a [`torch.jit.ScriptModule`](https://pytorch.org/docs/stable/generated/torch.jit.ScriptModule.html#torch.jit.ScriptModule) rather than a [`torch.nn.Module`](https://pytorch.org/docs/stable/generated/torch.nn.Module.html#torch.nn.Module). If the passed-in model is not already a `ScriptModule`, `export()` will use *tracing* to convert it to one:



分析一下初始化device & Host 和后续取交换的数据量，因为在device端和Host端需要申请空间

**getBindingDimensions**  doc Get the dimensions of a binding. 

```
for (int j = 0; j < engine->getBindingDimensions(i).nbDims; j++) {
        std::cout << engine->getBindingDimensions(i).d[j] << ",";
      }
```

nvinfer1::Dims32 Class Reference底下有两个对象

| int32_t | **nbDims** 多少维度                                          |
| ------- | ------------------------------------------------------------ |
|         | The rank (number of dimensions).                             |
| int32_t | **d** [[MAX_DIMS](https://docs.nvidia.com/deeplearning/tensorrt/archives/tensorrt-823/api/c_api/classnvinfer1_1_1_dims32.html#a7fbc72600de48c63141f3a83d9df0150)] 每个维度的大小 |
|         | The extent of each dimension.                                |

设定多Batch inference 应用Batch 8

  Bind[0] {Name:images, Datatype:0,Shape:(8,3,640,640,)}
  Bind[1] {Name:num_detections, Datatype:3,Shape:(8,1,)}
  Bind[2] {Name:detection_boxes, Datatype:0,Shape:(8,100,4,)}
  Bind[3] {Name:detection_scores, Datatype:0,Shape:(8,100,)}
  Bind[4] {Name:detection_classes, Datatype:3,Shape:(8,100,)}

8batch 10times Time elapsed gpu 0.340840 sec 4.25/perimage

1batch 80times cpu Time elapsed cpu 0.504427 sec 6.25ms/perimg
1batch 80times gpu Time elapsed gpu 0.784668 sec 9.75ms/perimg

考虑到运行中warpaffine 的实现方式为透视变换的过程

其实yolort的形式为resize(bilinear+padding)的形式

关于 warpaffine resize https://blog.csdn.net/weixin_42398658/article/details/121019668

理论上是可以进行resize类似bilinear

```
s2d.value[0] = scale;
s2d.value[1] = 0;
s2d.value[2] = -scale * src_width  * 0.5  + dst_width * 0.5;
s2d.value[3] = 0;
s2d.value[4] = scale;
s2d.value[5] = -scale * src_height * 0.5 + dst_height * 0.5;
```

https://zhuanlan.zhihu.com/p/89684929

但是结果测试不同的

warpAffine也是通过格点采样，然后bilinear插值实现的吗？那就是格点采样的位置不对呀。上面的4*4矩阵分为四块方格儿，resize是格点放各块在中央，用bilinear采样；warpAffine是放在方格的左上角，从而分别是255， 0， 255, 0？
