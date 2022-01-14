SSE AVX 优化的问题

[refer:GiantPandaCV](http://www.giantpandacv.com/project/%E9%83%A8%E7%BD%B2%E4%BC%98%E5%8C%96/AI%20PC%E7%AB%AF%E4%BC%98%E5%8C%96/%E3%80%90AI%20PC%E7%AB%AF%E7%AE%97%E6%B3%95%E4%BC%98%E5%8C%96%E3%80%91%E4%B8%80%EF%BC%8C%E4%B8%80%E6%AD%A5%E6%AD%A5%E4%BC%98%E5%8C%96RGB%E8%BD%AC%E7%81%B0%E5%BA%A6%E5%9B%BE%E7%AE%97%E6%B3%95/)

[官方AVX文档](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#techs=AVX)

暂时临时整理文档

Intel ICC 和开源的 GCC 编译器支持的 SSE/AVX 指令的 C 接口 (intrinsic，内置函数) 声明在`intrinsic.h`头文件中。其数据类型命名主要有`__m128/__m256`，`__m128d/__m256i`，默认为单精度 (`d`表示双精度，`i`表示整形)。其函数命名可大致分成 3 个使用`_`隔开的部分，3 个部分的含义如下：

- 第一个部分为`_mm`或`_mm256`。`_mm`表示其为 SSE 指令，操作的向量长度为 64 位或 128 位。`_mm256`表示 AVX 指令，操作的向量长度为 256 位。
- 第二个部分为操作函数名称，如`_add`，`_load`，`_mul`等，一些函数操作会增加修饰符，比如`loadu`表示不对齐到向量长度的存储器访问。
- 第三个部分为操作的对象名及数据类型，`_ps`表示操作向量中所有的单精度数据。`_pd`表示操作向量中所有的双精度数据。`_pixx`表示操作向量中所有的 xx 位的有符号整型数据，向量寄存器长度为 64 位。`_epixx`表示操作向量中所有的`xx`位的有符号整型数据，向量寄存器长度为 128 位。`_epuxx`表示操作向量中所有的 xx 位的无符号整形数据，向量寄存器长度为 128 位。`_ss`表示只操作向量中第一个单精度数据。`_si128`表示操作向量寄存器中第一个 128 位的有符号整型数据。

主要介绍了如果使用SSE AVX优化问题

#MILR

本内容主要用于自己MILR和

