# Tesseract

出于特殊需求，用于读取指定ROI区域的字符，背景单一，且字体清晰，故采用简易OCR服务。

使用参考官方网站进行安装

[Tesseract](https://tesseract-ocr.github.io/)

官方网站其实提供了PPA接口，但是一直Public-key挂不上，就放弃采用手动编译了。

安装之前需要安装libleptonica-dev，其余一些图像等基础库，一般是开发机器，最多只有该库没有安装。

```
sudo apt-get install libleptonica-dev
```

[install文档](https://tesseract-ocr.github.io/tessdoc/Compiling-%E2%80%93-GitInstallation.html)

安装完后会生成静态库和头文件。

开始使用需要有模型文件

[testdata](https://github.com/tesseract-ocr/tessdata)

下好的文件放到/usr/local/share/tessdata中

使用文档参考如下形式

[C++ API](https://tesseract-ocr.github.io/tessdoc/Examples_C++.html)

其中会遇到数据结构转换的问题Mat数据存储格式和Pix存储格式

[Mat2Pix](https://blog.csdn.net/andylanzhiyong/article/details/83305924)

```c++
Pix* cvtMat2PIX(cv::Mat imgGray)
{
    int cols = imgGray.cols;
    int rows = imgGray.rows;

    Pix *pixS = pixCreate(cols, rows, 8);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            pixSetPixel(pixS, j, i, (l_uint32)imgGray.at<uchar>(i, j));
    return pixS;
}
```

