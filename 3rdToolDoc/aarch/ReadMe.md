# Jetson Nano

Basic

CUDA Version 10.2.8

/usr/local/cuda

cudnn 8.0

/usr/include

lib环境系统设置

位于/etc/ld.so.conf中

cmake中虽然没有找到但是在系统里存在

系统环境里只剩下2G

# sdcard 

测试



[tensorrtx](https://github.com/wang-xinyu/tensorrtx)

测试容易，yolov4在该环境下正常，618 580ms左右 512 380ms左右



[tensorrt_demos](https://github.com/jkjung-avt/tensorrt_demos#yolov4)

可以利用onnx2trt转换

/home/aaeon/sdcard

folder

3rdparty  yolov4 tensorRT tensorrtx

lost+found  

workspace yolo 环境



具体参考

tensorRTx

u版yolov3

AB版权重yolov4 

weights 512x512

pybind11封装



python 引入包的

import numpy等出现 illegal问题

virtualenv 中activate文件加入

export OPENBLAS_CORETYPE=ARMV8



问题：

碰到TensorRT 多线程使用报错

[TensorRT] ERROR: ../rtSafe/cuda/reformat.cu (925) - Cuda Error in NCHWToNCHHW2: 400 (invalid resource handle)

utils/yolo_with_plugins.py

```
class TrtYOLO(object):
    """TrtYOLO class encapsulates things needed to run TRT YOLO."""

    def _load_engine(self):
        TRTbin = 'yolo/%s.trt' % self.model
        with open(TRTbin, 'rb') as f, trt.Runtime(self.trt_logger) as runtime:
            return runtime.deserialize_cuda_engine(f.read())

    def __init__(self, model, category_num=80, letter_box=False, cuda_ctx=None):
        """Initialize TensorRT plugins, engine and conetxt."""
        self.model = model
        self.category_num = category_num
        self.letter_box = letter_box
        # add this 
        cuda_ctx = cuda.Device(0).make_context()   
        
        self.cuda_ctx = cuda_ctx
```



```javascript
self.cfx= cuda.Device(0).make_context()
```
