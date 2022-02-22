#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/cudaimgproc.hpp>
#include <time.h>


#include <sys/time.h>

double cpuSecond()
{
    struct timeval tp;
    gettimeofday(&tp,NULL);
    return((double)tp.tv_sec+(double)tp.tv_usec*1e-6);

}


int main ()
{


    double iStart,iElaps;
    cv::Mat h_img1 = cv::imread("../images/cameraman.tif",0);
    cv::cuda::GpuMat d_img1,d_result1;
    cv::Mat h_result1;
    int i = 0;
    iStart=cpuSecond();
    while(i<10000){
        d_img1.upload(h_img1);
        cv::cuda::equalizeHist(d_img1, d_result1);
        d_result1.download(h_result1);
        ++i;
    }
    iElaps=cpuSecond()-iStart;
    printf("processGPU %f ms\n",iElaps/10);
    iStart=cpuSecond();
    cv::Mat dst;
    while(i<10000){
        cv::equalizeHist(h_img1, dst);
        ++i;
    }
    iElaps=cpuSecond()-iStart;
    printf("processCPU %f s\n",iElaps/10);
    //    cv::imshow("Original Image ", h_img1);
    //    cv::imshow("Histogram Equalized Image", h_result1);
    //    cv::imwrite("../images/result_inversionRes.png", h_img1);
    //    cv::imwrite("../images/result_inversionRes2.png", h_result1);
    //    cv::waitKey();

    return 0;
}
