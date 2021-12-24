## Usage

获得Cuda

```shell
sudo dpkg -i cuda-repo-<distro>_<version>_<architecture>.deb //offline install 
```



```shell
2.Install the CUDA public GPG key using the following command:
sudo apt-key add /var/cuda-repo-<version>/7fa2af80.pub
3. Then, update the apt repository cache using the following command:
sudo apt-get update
4. Then you can install CUDA using the following command:
sudo apt-get install cuda
```



```shell
5. Include the CUDA installation path in the PATH environment variable using
the following command:
If you have not installed CUDA at default locations, you need to change the path to point at
your installation location.
export PATH=/usr/local/cuda-x.x/bin${PATH:+:${PATH}}
6. Set the LD_LIBRARY_PATH environment variable:
export LD_LIBRARY_PATH=/usr/local/cuda-x.x/lib64\
${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
```

获得带Cuda的Opencv 编译的指令如下

参考指令

```
mkdir build-cuda 
cd build-cuda
cmake  -D CMAKE_BUILD_TYPE=RELEASE                            \
           -D CMAKE_INSTALL_PREFIX=/usr/local              \
           -D WITH_CUDA=ON                                        \
           -D CUDA_GENERATION=Pascal                              \
           -D ENABLE_FAST_MATH=1                                  \
           -D CUDA_FAST_MATH=1                                    \
           -D WITH_CUBLAS=1                                       \
           -D WITH_CUDNN=ON                                       \
           -D WITH_OPENCL=ON                                      \
           -D INSTALL_PYTHON_EXAMPLES=ON                          \
           -D BUILD_EXAMPLES=OFF ..                                   

```

实际指令如下

```
mkdir build 
cd build 
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_CUDA=ON -D CUDA_GENERATION=Turing -D WITH_CUDNN=ON ..
```

目前

https://blog.csdn.net/qq_30263737/article/details/80581071

https://blog.csdn.net/qq_30263737/article/details/80581037?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163531600016780262511900%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=163531600016780262511900&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_v2~rank_v29-3-80581037.pc_v2_rank_blog_default&utm_term=cuda&spm=1018.2226.3001.4450

测试算力 算力值查询

https://developer.nvidia.com/zh-cn/cuda-gpus#compute
