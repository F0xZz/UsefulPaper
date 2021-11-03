#  

Docker offline downloadsite:

https://download.docker.com/linux/ubuntu/dists/bionic/pool/stable/amd64/



nvidia-docker2:

```shell
sudo apt download libnvidia-container1 libnvidia-container-tools nvidia-container-toolkit nvidia-container-runtime nvidia-docker2 
```

   ## 一个驱动容器配合一个调用GPU的容器

### 说明

从图上所示，应该是可以驱动容器和CUDA容器配合宿主机进行互通。

​    ![nvidia+driver](/home/ubuntu/Pictures/docker/images/nvidia+driver.jpg)

使用条件：前提条件

- **需要有Ubuntu16.04，18.04并且安装了IPMI驱动。需禁止Nouveau驱动**

- **NVIDIA显卡架构需要Fermi>2.1（目前暂时无法确定1080）**

- **给Docker配置容器的root权限**

- **如果使用AWS kernal 的ubuntu 需要使用下行代码**

```shell
 sudo tee /etc/modules-load.d/ipmi.conf <<< "i2c_core"
```

配置过程:（**确认已经安装了Docker与Nvidia-Docker、NVIDIA Container Toolkit **）



1. **先更改root权限**

方式一手动

```shell
disable-require = false
#swarm-resource = "DOCKER_RESOURCE_GPU"

[nvidia-container-cli]
root = "/run/nvidia/driver"
#path = "/usr/bin/nvidia-container-cli"
environment = []
#debug = "/var/log/nvidia-container-toolkit.log"
#ldcache = "/etc/ld.so.cache"
load-kmods = true
#no-cgroups = false
#user = "root:video"
ldconfig = "@/sbin/ldconfig.real"

[nvidia-container-runtime]
#debug = "/var/log/nvidia-container-runtime.log"
```

方式二

```shell
sudo vim 
sudo sed -i 's/^#root/root/' /etc/nvidia-container-runtime/config.toml
```

上述方式将config文件root更改

2. **升级initramfs**（内核更新）

```shell
 sudo update-initramfs -u
```

3. **配置IPMI关闭Nouveau**

```shell
#关闭Nouveau
sudo tee /etc/modules-load.d/ipmi.conf <<< "ipmi_msghandler" \
  && sudo tee /etc/modprobe.d/blacklist-nouveau.conf <<< "blacklist nouveau" \
  && sudo tee -a /etc/modprobe.d/blacklist-nouveau.conf <<< "options nouveau modeset=0"
```

根据官方IPMI，从Ubuntu8.04开始默认支持openipmi

但是并没有安装ipmitools

```shell
先下载ipmitool
apt-get install ipmitool
```

```shell
 modprobe ipmi_msghandler
 modprobe ipmi_devintf
 modprobe ipmi_si
 modprobe ipmi_poweroff
 modprobe ipmi_watchdog
```

4. 包升级(可选但是不升级)

```shell
sudo apt-get dist-upgrade
```

5. **重启**

```shell
sudo reboot
```

6. **启动一个驱动容器**

```shell
sudo docker run --name nvidia-driver -d --privileged --pid=host \
  -v /run/nvidia:/run/nvidia:shared \
  -v /var/log:/var/log \
  --restart=unless-stopped \
  nvidia/driver:450.80.02-ubuntu18.04
  
sudo docker run --name nvidia-driver440 -d --privileged --pid=host \
  -v /run/nvidia:/run/nvidia:shared \
  -v /var/log:/var/log \
  --restart=unless-stopped \
  nvidia/driver:440.64.00-ubuntu18.04  
  
  
  sudo docker logs -f nvidia-driver
```

7. **启动一个GPU的容器**

```shell
sudo docker run --rm --gpus all nvidia/cuda:11.0-base nvidia-smi
```



```
# 导入镜像文件
docker load -i XXX.tar
# 查询docker镜像ID
docker images
# 根据查询到id进行启动容器
# 启动图形化界面的容器或者使用qtcreator时需采用如下指令
# 打开xhost+
sudo xhost +
# 创建运行容器
sudo docker run -it --privileged -v /tmp/.X11-unix:/tmp/.X11-unix -v /home/ubuntu/project:/home/ubuntu/project -e DISPLAY=unix$DISPLAY $PULSE_SERVER_TCP_ENV --gpus all -p 4000:4000/tcp -e LANG=C.UTF-8 --shm-size 8G --name projecttest02 codetest:0.1
# docker 容器启动指令
sudo docker run -it --privileged -v /tmp/.X11-unix:/tmp/.X11-unix -v [本地目录]:[容器目录] -e DISPLAY=unix$DISPLAY $PULSE_SERVER_TCP_ENV --gpus all -p 4000:4000/tcp -e LANG=C.UTF-8 --name [容器名字] [上步所查到的镜像ID]


sudo docker run -it --privileged -v /tmp/.X11-unix:/tmp/.X11-unix -v /home/ubuntu/3060code:/home/ubuntu/3060code -e DISPLAY=unix$DISPLAY $PULSE_SERVER_TCP_ENV --gpus all -e LANG=C.UTF-8 --shm-size 32G --name 3060cuda11 3b45442d83f1


sudo docker run -it --privileged -v /tmp/.X11-unix:/tmp/.X11-unix -v /home/ubuntu/3060code:/home/ubuntu/3060code -e DISPLAY=unix$DISPLAY $PULSE_SERVER_TCP_ENV --gpus all -e LANG=C.UTF-8 --shm-size 32G --name 3060cuda11trt7.0 b1386993f571

sudo docker run -it --privileged -v /tmp/.X11-unix:/tmp/.X11-unix -v /home/ubuntu/20210401-/trafficjam:/home/ubuntu/20210401-/trafficjam -e DISPLAY=unix$DISPLAY $PULSE_SERVER_TCP_ENV --gpus all -e LANG=C.UTF-8 --shm-size 32G --name makeenv a3bd8cb789b0
sudo docker run -it --privileged -v /tmp/.X11-unix:/tmp/.X11-unix -v /home/ubuntu/NVENC:/home/ubuntu/NVENC -e DISPLAY=unix$DISPLAY $PULSE_SERVER_TCP_ENV --gpus all -e LANG=C.UTF-8 --shm-size 32G --name makeenv a3bd8cb789b0
```

