# 0x00 ProtoBuf

其实该工程应用中，或多或少有使用到该ProtoBuf，但是我们更为关注的Json和XML格式的数据交互。平常以前看到过，也许使用过，但是并没有好好的了解过。

最近使用onnx的时候，发现onnx也使用该数据格式的交互形式，故而又想着去进行了解。在平常使用json，xml对于编码格式统一，是否存在转义符，都会存在

异议。类似启用了Protocol Buffers 的形式，进行传递

## 0x01 basic 

protobuf，主要应用在网络通信和通用数据交互的场景中

主要存在protoc编译器，对指定的内容编译成各个语言可使用的接口，方便统一

## 0x02 feature

protocol buffers 是一种语言无关、平台无关、可扩展的序列化结构数据的方法，它可用于（数据）通信协议、数据存储等。

Protocol Buffers 是一种灵活，高效，自动化机制的结构数据序列化方法－可类比 XML，但是比 XML 更小（3 ~ 10倍）、更快（20 ~ 100倍）、更为简单。

你可以定义数据的结构，**然后使用特殊生成的源代码轻松的在各种数据流中使用各种语言进行编写和读取结构数据**。你甚至可以更新数据结构，而不破坏由旧数据结构编译的已部署程序。

## 0x03 basic usage

```protobuf
// 例1: 在 xxx.proto 文件中定义 Example1 message
message Example1 {
    optional string stringVal = 1;
    optional bytes bytesVal = 2;
    message EmbeddedMessage {
        int32 int32Val = 1;
        string stringVal = 2;
    }
    optional EmbeddedMessage embeddedExample1 = 3;
    repeated int32 repeatedInt32Val = 4;
    repeated string repeatedStringVal = 5;
}

```

编译流程

```
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/xxx.proto
```



## -1x00 refer



官方git

官方文档

[简书简介](https://www.jianshu.com/p/a24c88c0526a)

