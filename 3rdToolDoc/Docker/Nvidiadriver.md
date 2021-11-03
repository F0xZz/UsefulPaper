#  Nvidia driver 

## 显卡驱动安装

1. 通过官网下载所需驱动:[offical web](https://www.geforce.com/drivers) 

2. 卸载原有的自带驱动

   ```shell
   sudo apt-get remove --purge nvidia*
   ```

3. 安装依赖项

   ```shell
   sudo apt install dkms build-essential linux-headers-generic
   ```

4. 屏蔽nouveau等 .

   ```shell
   sudo gedit /etc/modprobe.d/blacklist.conf
   ```

   把下列添加入文本末尾

   ```cmake
   blacklist rivafb
   blacklist vga16fb
   blacklist nouveau
   blacklist nvidiafb
   blacklist rivatv
   ```
2.需要禁用 nouveau，只有在禁用掉 nouveau 后才能顺利安装 NVIDIA 显卡驱动，禁用方法就是在 /etc/modprobe.d/blacklist-nouveau.conf 文件中添加一条禁用命令，首先需要打开该文件，通过以下命令打开:

   sudo gedit /etc/modprobe.d/blacklist.conf

输入密码后在最后一行加上:  blacklist nouveau 是将Ubuntu自带的显卡驱动加入黑名单
5. 重启电脑

   ```shell
   sudo reboot
   ```

6. 安装Nvidia driver

   ```shell
   sudo chmod a+x NVIDIA-Linux-x86_64-384.111.run -no-opengl-files
   sudo ./NVIDIA-Linux-x86_64-384.111.run -no-opengl-files -no-x-check -no-nouveau-check 
   ```

7. 确认安装

   ```shell
   nvidia-smi
   ```

   如果没有出现显卡信息尝试使用下列指令

   ```shell
   sudo apt-get install dkms
   sudo dkms install -m nvidia -v xxx #版本号
   ```

   **确认驱动版本** :

   ```shell
   cd /usr/src/
   ls
   #下面就有版本号
   ```

   Cuda安装

   1. 下载文件：[官方网站](https://developer.nvidia.com/cuda-10.0-download-archive)

   2. 给文件权限后进行执行

      ```shell
      sudo chmod a+x cuda10.0.run file
      sudo ./cuda10.0.run file -no-opengl-libs
      ```

   3. 对接下来出现的是否安装驱动(Driver 410.34)：选择N

   4. 其他一律默认

   5. 添加进环境

      ```shell
      sudo vim ~/.bashrc 
      ```

      添加下列文本进入文本末尾（注意所安装的cuda与安装位置）

      ```
      export CUDA_HOME=/usr/local/cuda
      export PATH=$PATH:$CUDA_HOME/bin
      export LD_LIBRARY_PATH=/usr/local/cuda-10.0/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
      ```

   6. ```shell
      source ~/.bashrc
      ```

   7. 测试（注意个人Samples安装位置）

      ```shell
      cd /home/fox/NVIDIA_CUDA-10.0_Samples/1_Utilities/deviceQuery
      sudo make 
      ./deviceQuery
      ```

      ##  Cudnn 安装

      下载地址：[官方网站](https://developer.nvidia.com/cudnn)

1. 解压

   ```shell
   tar zxvf cudnn-10.0-linux-x64-v7.5.0.56.tgz cuda
   ```

2. ```shell
   sudo cp cuda/include/cudnn.h /usr/local/cuda/include/ 
   sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/ 
   sudo chmod a+r /usr/local/cuda/include/cudnn.h 
   sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
   
   ```

   添加进环境作链接

3. ```shell
   cat /usr/local/cuda/include/cudnn.h | grep CUDNN_MAJOR -A 2
   ```

   看是否输出测试是否环境安装成功
