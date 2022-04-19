#

[ffcnn](https://github.com/rockcarry/ffcnn)

一款可读取darknet cfg方式的前向推理框架

cvt 执行分辨率改变

从

mynet = net_load(file_cfg, file_weights);

中实现了网络模型的载入

载入方式采用首先先载入cfg文件

其中会将bytes读取到buf中

buf作为static char*

```
size = (int)fread(buf, 1, size, fp); buf[size] = '\0'; 
```

