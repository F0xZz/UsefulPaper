SSE AVX 优化的问题

[refer:GiantPandaCV](http://www.giantpandacv.com/project/%E9%83%A8%E7%BD%B2%E4%BC%98%E5%8C%96/AI%20PC%E7%AB%AF%E4%BC%98%E5%8C%96/%E3%80%90AI%20PC%E7%AB%AF%E7%AE%97%E6%B3%95%E4%BC%98%E5%8C%96%E3%80%91%E4%B8%80%EF%BC%8C%E4%B8%80%E6%AD%A5%E6%AD%A5%E4%BC%98%E5%8C%96RGB%E8%BD%AC%E7%81%B0%E5%BA%A6%E5%9B%BE%E7%AE%97%E6%B3%95/)

[官方AVX文档](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#techs=AVX)

暂时临时整理文档

Intel ICC 和开源的 GCC 编译器支持的 SSE/AVX 指令的 C 接口 (intrinsic，内置函数) 声明在`intrinsic.h`头文件中。其数据类型命名主要有`__m128/__m256`，`__m128d/__m256i`，默认为单精度 (`d`表示双精度，`i`表示整形)。其函数命名可大致分成 3 个使用`_`隔开的部分，3 个部分的含义如下：

- 第一个部分为`_mm`或`_mm256`。`_mm`表示其为 SSE 指令，操作的向量长度为 64 位或 128 位。`_mm256`表示 AVX 指令，操作的向量长度为 256 位。
- 第二个部分为操作函数名称，如`_add`，`_load`，`_mul`等，一些函数操作会增加修饰符，比如`loadu`表示不对齐到向量长度的存储器访问。
- 第三个部分为操作的对象名及数据类型，`_ps`表示操作向量中所有的单精度数据。`_pd`表示操作向量中所有的双精度数据。`_pixx`表示操作向量中所有的 xx 位的有符号整型数据，向量寄存器长度为 64 位。`_epixx`表示操作向量中所有的`xx`位的有符号整型数据，向量寄存器长度为 128 位。`_epuxx`表示操作向量中所有的 xx 位的无符号整形数据，向量寄存器长度为 128 位。`_ss`表示只操作向量中第一个单精度数据。`_si128`表示操作向量寄存器中第一个 128 位的有符号整型数据。

主要介绍了如果使用SSE AVX优化问题

key operations of sse2

sse关键指令如下转灰度图

关于Reference的代码code中，code中的问题，

```sse
__m128i _mm_setr_epi16 (short e7, short e6, short e5, short e4, short e3, short e2, short e1, short e0)
Synopsis
__m128i _mm_setr_epi16 (short e7, short e6, short e5, short e4, short e3, short e2, short e1, short e0)
#include <emmintrin.h>
Instruction: Sequence
CPUID Flags: SSE2
Description
//short int
Set packed 16-bit integers in dst with the supplied values in reverse order.
dst[15:0] := e7
dst[31:16] := e6
dst[47:32] := e5
dst[63:48] := e4
dst[79:64] := e3
dst[95:80] := e2
dst[111:96] := e1
dst[127:112] := e0
```

```
_mm_loadu_si128((__m128i *)(LinePS + 0))
Load 128-bits of integer data from unaligned memory into dst. This intrinsic may perform better than _mm_loadu_si128 when the data crosses a cache line boundary.
Description
Zero extend packed unsigned 8-bit integers in a to packed 16-bit integers, and store the results in dst.
```



```c++
void RGB2Y_3(unsigned char *Src, unsigned char *Dest, int Width, int Height, int Stride) {
    const int B_WT = int(0.114 * 256 + 0.5);
    const int G_WT = int(0.587 * 256 + 0.5);
    const int R_WT = 256 - B_WT - G_WT; // int(0.299 * 256 + 0.5)

    for (int Y = 0; Y < Height; Y++) {
        unsigned char *LinePS = Src + Y * Stride;
        unsigned char *LinePD = Dest + Y * Width;
        int X = 0;
        for (; X < Width - 12; X += 12, LinePS += 36) {
            __m128i p1aL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 0))), _mm_setr_epi16(B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT)); //1
            __m128i p2aL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 1))), _mm_setr_epi16(G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT)); //2
            __m128i p3aL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 2))), _mm_setr_epi16(R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT)); //3

            __m128i p1aH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 8))), _mm_setr_epi16(R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT));//4
            __m128i p2aH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 9))), _mm_setr_epi16(B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT));//5
            __m128i p3aH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 10))), _mm_setr_epi16(G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT));//6

            __m128i p1bL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 18))), _mm_setr_epi16(B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT));//7
            __m128i p2bL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 19))), _mm_setr_epi16(G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT));//8
            __m128i p3bL = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 20))), _mm_setr_epi16(R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT));//9

            __m128i p1bH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 26))), _mm_setr_epi16(R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT));//10
            __m128i p2bH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 27))), _mm_setr_epi16(B_WT, G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT));//11
            __m128i p3bH = _mm_mullo_epi16(_mm_cvtepu8_epi16(_mm_loadu_si128((__m128i *)(LinePS + 28))), _mm_setr_epi16(G_WT, R_WT, B_WT, G_WT, R_WT, B_WT, G_WT, R_WT));//12

            __m128i sumaL = _mm_add_epi16(p3aL, _mm_add_epi16(p1aL, p2aL));//13
            __m128i sumaH = _mm_add_epi16(p3aH, _mm_add_epi16(p1aH, p2aH));//14
            __m128i sumbL = _mm_add_epi16(p3bL, _mm_add_epi16(p1bL, p2bL));//15
            __m128i sumbH = _mm_add_epi16(p3bH, _mm_add_epi16(p1bH, p2bH));//16
            __m128i sclaL = _mm_srli_epi16(sumaL, 8);//17
            __m128i sclaH = _mm_srli_epi16(sumaH, 8);//18
            __m128i sclbL = _mm_srli_epi16(sumbL, 8);//19
            __m128i sclbH = _mm_srli_epi16(sumbH, 8);//20
            __m128i shftaL = _mm_shuffle_epi8(sclaL, _mm_setr_epi8(0, 6, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));//21
            __m128i shftaH = _mm_shuffle_epi8(sclaH, _mm_setr_epi8(-1, -1, -1, 18, 24, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));//22
            __m128i shftbL = _mm_shuffle_epi8(sclbL, _mm_setr_epi8(-1, -1, -1, -1, -1, -1, 0, 6, 12, -1, -1, -1, -1, -1, -1, -1));//23
            __m128i shftbH = _mm_shuffle_epi8(sclbH, _mm_setr_epi8(-1, -1, -1, -1, -1, -1, -1, -1, -1, 18, 24, 30, -1, -1, -1, -1));//24
            __m128i accumL = _mm_or_si128(shftaL, shftbL);//25
            __m128i accumH = _mm_or_si128(shftaH, shftbH);//26
            __m128i h3 = _mm_or_si128(accumL, accumH);//27
            //__m128i h3 = _mm_blendv_epi8(accumL, accumH, _mm_setr_epi8(0, 0, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, 1, 1, 1, 1));
            _mm_storeu_si128((__m128i *)(LinePD + X), h3);//28
        }
        for (; X < Width; X++, LinePS += 3) {//29
            LinePD[X] = (B_WT * LinePS[0] + G_WT * LinePS[1] + R_WT * LinePS[2]) >> 8;//30
        }
    }
}
```

#MILR

本内容主要用于自己MILR和

