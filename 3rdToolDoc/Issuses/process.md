#frequency command in the linux

list all of the process

```
ps -ef 
```

find out the process create by "username"

```
ps -U username
```

list top 4 cpu use rate

```
ps -e -o "%C %c" | sort -u -k1 -r | head -5
```

list top 4 virtual memory use rate 

```
ps -e -o "%z %c" | sort -n -k1 -r | head -5
```

#关于函数调用关系 C方向

**calltree** tool

```
calltree -b -np -m *.c
```

# 性能测试方向

[参考文档](https://www.bookstack.cn/read/open-c-book/zh-chapters-02-chapter9.markdown)

gprof&kprof

实例，用于测试fibonacci数列，定义了递归函数。

```
#include <stdio.h>
int fibonacci(int n);
int main (int argc, char **argv)
{
    int fib;
    int n;
    for (n = 0; n <= 42; n++) {
        fib = fibonacci(n);
        printf("fibonnaci(%d) = %d\n", n, fib);
    }
    return 0;
}
int fibonacci(int n)
{
    int fib;
    if (n <= 0) {
        fib = 0;
    } else if (n == 1) {
        fib = 1;
    } else {
        fib = fibonacci(n -1) + fibonacci(n - 2);
    }
    return fib;
}
```

可以看出程序主要涉及到一个 `fibonacci` 函数，这个函数递归调用自己。为了能够使用 `gprof`，需要编译时加上 `-pg` 选项，让 `Gcc` 加入相应的调试信息以便 `gprof` 能够产生函数执行情况的报告。

```
$ gcc -pg -o fib fib.c
$ ls
fib  fib.c
```

执行如下命令

```
time ./fib
```

中间会生成，gmon.out 查询测试内容

```
gprof  -b ./fib gmon.out 
```

# 代码覆盖率测试

gcov&ggcov 

代码覆盖率：

- 函式覆盖率（Function coverage）：有呼叫到程式中的每一个函式（或副程式）吗？

- 指令覆盖率（Statement coverage）：若用[控制流图](https://baike.baidu.com/item/控制流图)表示程式，有执行到控制流图中的每一个节点吗？

- 判断覆盖率（Decision coverage）：（和分支覆盖率不同）若用控制流图表示程式，有执行到控制流图中的每一个边吗？例如控制结构中所有IF指令都有执行到逻辑运算式成立及不成立的情形吗？

- 条件覆盖率（Condition coverage）：也称为谓词覆盖（predicate coverage），每一个逻辑运算式中的每一个条件（无法再分解的逻辑运算式）是否都有执行到成立及不成立的情形吗？条件覆盖率成立不表示判断覆盖率一定成立。

- 条件/判断覆盖率（Condition/decision coverage）：需同时满足判断覆盖率和条件覆盖率。

  ```
  $ ls
  fib.c
  $ gcc -fprofile-arcs -ftest-coverage -o fib fib.c
  $ ls
  fib  fib.c  fib.gcno
  ```

  运行程序，通过gcov查看

  ```
  $ ./fib
  $ gcov fib.c
  File 'fib.c'
  Lines executed:100.00% of 12
  fib.c:creating 'fib.c.gcov'
  ```

# 内存访问越界 catchsegv, libSegFault.so

Segmentation fault” 是很头痛的一个问题，估计“纠缠”过很多人。这里仅仅演示通过 `catchsegv` 脚本测试段错误的方法，其他方法见后面相关资料。

`catchsegv` 利用系统动态链接的 `PRELOAD` 机制（请参考`man ld-linux`），把库 `/lib/libSegFault.so` 提前 load 到内存中，然后通过它检查程序运行过程中的段错误。

```c++
$ cat test.c
#include <stdio.h>
int main(void)
{
    char str[10];
        sprintf(str, "%s", 111);
        printf("str = %s\n", str);
        return 0;
}
$ make test
$ LD_PRELOAD=/lib/libSegFault.so ./test  #等同于catchsegv ./test
*** Segmentation fault
Register dump:
 EAX: 0000006f   EBX: b7eecff4   ECX: 00000003   EDX: 0000006f
 ESI: 0000006f   EDI: 0804851c   EBP: bff9a8a4   ESP: bff9a27c
 EIP: b7e1755b   EFLAGS: 00010206
 CS: 0073   DS: 007b   ES: 007b   FS: 0000   GS: 0033   SS: 007b
 Trap: 0000000e   Error: 00000004   OldMask: 00000000
 ESP/signal: bff9a27c   CR2: 0000006f
Backtrace:
/lib/libSegFault.so[0xb7f0604f]
[0xffffe420]
/lib/tls/i686/cmov/libc.so.6(vsprintf+0x8c)[0xb7e0233c]
/lib/tls/i686/cmov/libc.so.6(sprintf+0x2e)[0xb7ded9be]
./test[0x804842b]
/lib/tls/i686/cmov/libc.so.6(__libc_start_main+0xe0)[0xb7dbd050]
./test[0x8048391]
...
```

从结果中可以看出，代码的 `sprintf` 有问题。经过检查发现它把整数当字符串输出，对于字符串的输出，需要字符串的地址作为参数，而这里的 `111` 则刚好被解释成了字符串的地址，因此 `sprintf` 试图访问 `111` 这个地址，从而发生了非法访问内存的情况，出现 “Segmentation Fault”。

# 内存泄露问题Memwatch, Valgrind, mtrace

堆栈通常会被弄在一起叫，不过这两个名词却是指进程的内存映像中的两个不同的部分，栈（Stack）用于函数的参数传递、局部变量的存储等，是系统自动分配和回收的；而堆（heap）则是用户通过 `malloc` 等方式申请而且需要用户自己通过 `free` 释放的，如果申请的内存没有释放，那么将导致内存泄露，进而可能导致堆的空间被用尽；而如果已经释放的内存再次被释放（double-free）则也会出现非法操作。如果要真正理解堆和栈的区别，需要理解进程的内存映像，请参考[《缓冲区溢出与注入分析》](https://www.bookstack.cn/read/open-c-book/02-chapter5.markdown)

这里演示通过 `Memwatch` 来检测程序中可能存在内存泄露，可以从[这里](http://www.linkdata.se/sourcecode.html)下载到这个工具。
使用这个工具的方式很简单，只要把它链接（ld）到可执行文件中去，并在编译时加上两个宏开关`-DMEMWATCH -DMW_STDIO`。这里演示一个简单的例子。

```
$ cat test.c 
#include <stdlib.h>
#include <stdio.h>
#include "memwatch.h"
int main(void)
{
    char *ptr1;
    char *ptr2;
    ptr1 = malloc(512);
    ptr2 = malloc(512);
    ptr2 = ptr1;
    free(ptr2);
    free(ptr1);
}
$ gcc -DMEMWATCH -DMW_STDIO test.c memwatch.c -o test
$ cat memwatch.log
============= MEMWATCH 2.71 Copyright (C) 1992-1999 Johan Lindh =============
Started at Sat Mar  1 07:34:33 2008
Modes: __STDC__ 32-bit mwDWORD==(unsigned long)
mwROUNDALLOC==4 sizeof(mwData)==32 mwDataSize==32
double-free: <4> test.c(15), 0x80517e4 was freed from test.c(14)
Stopped at Sat Mar  1 07:34:33 2008
unfreed: <2> test.c(11), 512 bytes at 0x8051a14         {FE FE FE FE FE FE FE FE FE FE FE FE FE FE FE FE ................}
Memory usage statistics (global):
 N)umber of allocations made: 2
 L)argest memory usage      : 1024
 T)otal of all alloc() calls: 1024
 U)nfreed bytes totals      : 512
```

通过测试，可以看到有一个 512 字节的空间没有被释放，而另外 512 字节空间却被连续释放两次（double-free）。`Valgrind` 和 `mtrace` 也可以做类似的工作，请参考资料[[4\]](http://www.faqs.org/docs/Linux-HOWTO/Valgrind-HOWTO.html)，[[5\]](http://www.cprogramming.com/debugging/valgrind.html)和 `mtrace` 的手册。

