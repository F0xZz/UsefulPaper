关于warpaffine的cuda实现，一开始测试，我以为warpaffine的实现resize bilinear lettor box没有对齐

关于warpaffine和resize总共两个问题

1.padding问题，查询了资料，该cuda的实现的变换矩阵采用了padding，因为使用了缩放和平移，

并且带越界处理问题

[变换矩阵](https://blog.csdn.net/weixin_42398658/article/details/121019668)参考这个解释

就是使用带偏移和scale的矩阵，由于使用的是双线性插值https://zhuanlan.zhihu.com/p/89684929 [2]

需要的是dst img 到原图的srcimg的value映射关系，所以需要有dst 坐标

640 480  0 0--> 0 0 配合boarder

opencv resize linear 

```
for (int dx = 0; dx < dsize.width; dx++)
            {
                fxx = (float)((dx+0.5)*inv_fx - 0.5);
                //用到了0.5 1 0 scale 1 1 情况下 1 0
                sx = cvFloor(fxx);
      //         int i = (int)value;
    //return i - (i > value);
                fxx -= sx;

                if (sx < 0)
                    fxx = 0, sx = 0;

                if (sx >= ssize.width-1)
                    fxx = 0, sx = ssize.width-1;

                xofs[dx] = sx;
                ialpha[dx*2 + 0] = saturate_cast<short>((1.f - fxx) * INTER_RESIZE_COEF_SCALE);
                ialpha[dx*2 + 1] = saturate_cast<short>(fxx         * INTER_RESIZE_COEF_SCALE);
            }

            for (int dy = 0; dy < dsize.height; dy++)
            {
                fyy = (float)((dy+0.5)*inv_fy - 0.5);
                sy = cvFloor(fyy);
                fyy -= sy;

                yofs[dy] = sy;
                ibeta[dy*2 + 0] = saturate_cast<short>((1.f - fyy) * INTER_RESIZE_COEF_SCALE);
                ibeta[dy*2 + 1] = saturate_cast<short>(fyy         * INTER_RESIZE_COEF_SCALE);
            }
```

warpaffine在源码中变换

warpaffine matrix

```
s2d.value[0] = scale;
s2d.value[1] = 0;
s2d.value[2] = -scale * src_width * 0.5 + dst_width * 0.5;
s2d.value[3] = 0;
s2d.value[4] = scale;
s2d.value[5] = -scale * src_height * 0.5 + dst_height * 0.5;
////做了逆变换到逆矩阵
 cv::invertAffineTransform(m2x3_s2d, m2x3_d2s);
 
```

![img](https://img-blog.csdnimg.cn/e6e87c59c08545be9d1c6f45be3e6042.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAenNmZnV0dXJl,size_20,color_FFFFFF,t_70,g_se,x_16)

```
int position = blockDim.x * blockIdx.x + threadIdx.x;
//这里获得了运算grid中块的idx坐标
int dx = position % dst_width;
int dy = position / dst_width;
//获得dst 目标计算图中坐标dx dy
```

测试640 480 则 0 0 -->0 0

```
//0.5f已经使用offset 
//m_x1:scale_x m_x2:scale_y 缩放比例
//m_z1:偏移
//0.5f:是为计算中心 resize使用的是0.5f 而传统的warpaffine用的0.0f所以有误差 
//参考[2]
//这里有个0.5的偏移，主要是因为warpaffine做resize和中心和opencvresize差距
//https://zhuanlan.zhihu.com/p/99626808 该文章
//https://zhuanlan.zhihu.com/p/89684929 该文章
// 0 0 scale 1的情况下 0.5f
float src_x = m_x1 * dx + m_y1 * dy + m_z1 + 0.5f;
float src_y = m_x2 * dx + m_y2 * dy + m_z2 + 0.5f;
```

cuda 实现中采用边界处理的方式

```
if (src_x <= -1 || src_x >= src_width || src_y <= -1 || src_y >= src_height)
{
// out of range
c0 = const_value_st;
c1 = const_value_st;
c2 = const_value_st;
}
//利用逆变换矩阵,对于dx dy 0 0点会映射到边界外直接赋予 const_value_st 114 同理
```

获得四个角点坐标执行公式即参考

```
int y_low = floorf(src_y);
int x_low = floorf(src_x);
int y_high = y_low + 1;
int x_high = x_low + 1;
//获得坐标
v1 = src + y_low * src_line_size + x_low * 3;
v2 = src + y_low * src_line_size + x_high * 3;
v3 = src + y_high * src_line_size + x_low * 3;
v4 = src + y_high * src_line_size + x_high * 3;
```

![[公式]](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+f%28x%2Cy%29%26%5Capprox%5Cfrac%7By_2-y%7D%7By_2-y_1%7Df%28x%2Cy_1%29%2B%5Cfrac%7By-y_1%7D%7By_2-y_1%7Df%28x%2Cy_2%29%5C%5C+%26%5Capprox%5Cfrac%7Bx_2-x%7D%7Bx_2-x_1%7D%5Cfrac%7By_2-y%7D%7By_2-y_1%7Df%28x_1%2Cy_1%29%2B%5Cfrac%7Bx-x_1%7D%7Bx_2-x_1%7D%5Cfrac%7By_2-y%7D%7By_2-y_1%7Df%28x_2%2Cy_1%29%5C%5C+%26%2B%5Cfrac%7Bx_2-x%7D%7Bx_2-x_1%7D%5Cfrac%7By-y_1%7D%7By_2-y_1%7Df%28x_1%2Cy_2%29%2B%5Cfrac%7Bx-x_1%7D%7Bx_2-x_1%7D%5Cfrac%7By-y_1%7D%7By_2-y_1%7Df%28x_2%2Cy_2%29+%5Cend%7Balign%7D)

```
float ly = src_y - y_low;
float lx = src_x - x_low;

float hy = 1 - ly;
float hx = 1 - lx;

float w1 = hy * hx, w2 = hy * lx, w3 = ly * hx, w4 = ly * lx;
计算了点的value
c0 = w1 * v1[0] + w2 * v2[0] + w3 * v3[0] + w4 * v4[0];
```

w1 = hy * hx, w2 = hy * lx, w3 = ly * hx, w4 = ly * lx;

其中w1为 v1[0]该点的权重值，v1为像素值

后续部分是rgb通道互换还有NHWC转NCHW

```
//完成通道转换
float t = c2;
c2 = c0;
c0 = t;

//normalization
c0 = c0 / 255.0f;
c1 = c1 / 255.0f;
c2 = c2 / 255.0f;

//rgbrgbrgb to rrrgggbbb
int area = dst_width * dst_height;
float *pdst_c0 = dst + dy * dst_width + dx;
float *pdst_c1 = pdst_c0 + area;
float *pdst_c2 = pdst_c1 + area;
//赋值
*pdst_c0 = c0;
*pdst_c1 = c1;
*pdst_c2 = c2;
```

2.bilinear的实现形式，的确在warpaffine的实现中使用的是opencv bilinear但是在使用过程中会出现边界问题，参考如下文章

https://zhuanlan.zhihu.com/p/89684929

https://zhuanlan.zhihu.com/p/99626808

但是考虑到运算过程调用中

```python
import numpy
import cv2
outputsize = 8
scale = outputsize/4
d = numpy.array([[255, 200, 0, 50],
[200, 255, 50, 0],
[ 0, 50, 255, 200],
[ 50, 0, 200, 255]], numpy.uint8)
cv2.resize(d, (outputsize,outputsize))
cv2.warpAffine(d, numpy.matrix([[scale,0,scale*0.5-0.5],[0,scale,scale*0.5-0.5]]), (outputsize,outputsize))
#输出结果并没有对齐
#resize
array([[255, 241, 214, 150, 50, 12, 37, 50],
[241, 234, 221, 163, 63, 19, 31, 38],
[214, 221, 234, 190, 88, 31, 19, 13],
[150, 163, 190, 178, 127, 88, 63, 50],
[ 50, 63, 88, 127, 178, 190, 163, 150],
[ 13, 19, 31, 88, 190, 234, 221, 214],
[ 38, 31, 19, 63, 163, 221, 234, 241],
[ 50, 37, 12, 50, 150, 214, 241, 255]], dtype=uint8)
#warpaffine
array([[143, 181, 160, 113, 38, 9, 28, 28],
[181, 234, 221, 163, 63, 19, 31, 28],
[160, 221, 234, 190, 88, 31, 19, 9],
[113, 163, 190, 178, 127, 88, 63, 38],
[ 38, 63, 88, 127, 178, 190, 163, 113],
[ 9, 19, 31, 88, 190, 234, 221, 160],
[ 28, 31, 19, 63, 163, 221, 234, 181],
[ 28, 28, 9, 38, 113, 160, 181, 143]], dtype=uint8)
#询问后，对齐要采用
#borderMode的问题，warpAffine的borderMode指定成BORDER_REPLICATE就一样了
cv2.warpAffine(d, numpy.matrix([[scale,0,scale*0.5-0.5],[0,scale,scale*0.5-0.5]]), (outputsize,outputsize),borderMode=cv2.BORDER_REPLICATE)
array([[255, 241, 214, 150,  50,  13,  38,  50],
       [241, 234, 221, 163,  63,  19,  31,  38],
       [214, 221, 234, 190,  88,  31,  19,  13],
       [150, 163, 190, 178, 127,  88,  63,  50],
       [ 50,  63,  88, 127, 178, 190, 163, 150],
       [ 13,  19,  31,  88, 190, 234, 221, 214],
       [ 38,  31,  19,  63, 163, 221, 234, 241],
       [ 50,  38,  13,  50, 150, 214, 241, 255]], dtype=uint8)

```

那么borderMode 指定BORDER_REPLICATE的作用

官方opencv给的解释

BORDER_REPLICATE 
Python: cv.BORDER_REPLICATE
aaaaaa|abcdefgh|hhhhhhh 

但是cuda的实现其实已经考虑了boarder边界问题最关键还是偏移位置设定的问题

后赖尝试采用bilinear使用cuda，但是不会处理padding问题，感觉速度还是很慢

```
__global__ void resize_linear_channel_shuffle(uint8_t *src, int src_height, int src_width,
                                              uint8_t *dst, int dst_height, int dst_width,
                                              int net_height, int net_width,
                                              const float fy, const float fx, const int const_value,
                                              int padding_x, int padding_y)

{
    //get the data
    const int dst_x = blockDim.x * blockIdx.x + threadIdx.x;
    const int dst_y = blockDim.y * blockIdx.y + threadIdx.y;
    // uint8_t *out =
    float c0, c1, c2;
    // make sure the out boundary
    // dst_height dst_width

    if (dst_x < dst_width && dst_y < dst_height)
    {
        //get the source pixel address
        const float src_x = dst_x * fx;
        const float src_y = dst_y * fy;
        //get four point
        const int x1 = __float2int_rd(src_x);
        const int y1 = __float2int_rd(src_y);
        const int x2 = x1 + 1;
        const int y2 = y1 + 1;
        const int x2_read = ::min(x2, src_width - 1);
        const int y2_read = ::min(y2, src_height - 1);
        //x1,y1 point
        uint8_t *v1 = src + x1 + y1 * src_width * 3;
        //x1,y2 point
        uint8_t *v2 = src + x1 + y2_read * src_width * 3;
        //x2,y2 point
        uint8_t *v3 = src + x2_read + y2_read * src_width * 3;
        //x2,y1 point
        uint8_t *v4 = src + x2_read + y1 * src_width * 3;
        //x1,y1 point weight
        float w1 = (x2 - src_x) * (y2 - src_y);
        //x2 y2 point weight
        float w2 = (src_x - x1) * (src_y - y1);
        //x1 y2 point weight
        float w3 = (x2 - src_x) * (src_y - y1);
        //y1 x2 point weight
        float w4 = (src_x - x1) * (y2 - src_y);
        // b
        c0 = w1 * v1[0] + w2 * v2[0] + w3 * v3[0] + w4 * v4[0];
        // g
        c1 = w1 * v1[1] + w2 * v2[1] + w3 * v3[1] + w4 * v4[1];
        // r
        c2 = w1 * v1[2] + w2 * v2[2] + w3 * v3[2] + w4 * v4[2];
    }
    //bgr to rgb
    float t = c2;
    c2 = c0;
    c0 = t;
    // normalize
    c0 = c0 / 255.0f;
    c1 = c1 / 255.0f;
    c2 = c2 / 255.0f;
    //rgbrgbrgb to rrrgggbbb
    int area = dst_width * dst_height;
    if (padding_x || padding_y)
    {
        float *pdst_c0 = dst + dst_y * dst_width + dst_x;
        float *pdst_c1 = pdst_c0 + area;
        float *pdst_c2 = pdst_c1 + area;
        *pdst_c0 = c0;
        *pdst_c1 = c1;
        *pdst_c2 = c2;
    }
}
```

