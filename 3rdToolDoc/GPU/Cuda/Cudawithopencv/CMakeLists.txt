cmake_minimum_required(VERSION 3.10)
project( main )
#include_directories(../include)
find_package( OpenCV REQUIRED )
# packages
find_package(CUDA)

# nvcc flags
#set(CUDA_NVCC_FLAGS -gencode arch=compute_70,code=sm_70;-G;-g)
set(CMAKE_CUDA_FLAGS "-arch=compute_75 -g -G -O3")
set(CMAKE_NVCC_FLAGS "CMAKE_NVCC_FLAGS -std=c++11")
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( main 04_image_resizing.cpp )
target_link_libraries( main ${OpenCV_LIBS} )
