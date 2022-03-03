# 编译原理

[参考文档](https://www.bookstack.cn/read/open-c-book/zh-chapters-02-chapter2.markdown)

参考书籍：《编译原理》

编译的过程：

将c语言编译的过程参与了预处理，编译，汇编，链接的过程。

gcc  如下command对应的内容

-E cpp 

-S cc1

-c as

-o ld

分别对应相应的程序，预处理，编译，汇编，链接

在走指令`gcc -o exec main.cpp`过程中，其实已经走了预处理，编译，链接生成可执行的文件的过程，只不过省略了-o链接的过程。

## 预处理过程

采用编译指令gcc -E hello.c

编译的预处理过程

如果需要编译中间设置宏定义，即设置macro

即相当于在代码中设置

```c++
#ifdef macro 
print("there was in the macro definition")
#endif
```

设置了marco的代码，将会被翻译编译

```shell
$ gcc -Dmacro hello.c
```

## 编译（翻译）

由于编译之前，需要对语言进行基础的词法分析，语法分析，接着才会将源代码翻译成中间语言。

