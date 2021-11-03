#  Docker docments 

##  一：开始

早期版本，图片损坏有意向可以帮忙pr补充一下

### 1.安装docker

- 环境测试（确认为ubuntu及其版本以上）

```shell
ubuntu@ubuntu-Default-string:~$ uname -r
4.18.0-15-generic
ubuntu@ubuntu-Default-string:~$ cat /etc/os-release 
NAME="Ubuntu"
VERSION="18.04.2 LTS (Bionic Beaver)"
ID=ubuntu
ID_LIKE=debian
PRETTY_NAME="Ubuntu 18.04.2 LTS"
VERSION_ID="18.04"
HOME_URL="https://www.ubuntu.com/"
SUPPORT_URL="https://help.ubuntu.com/"
BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
VERSION_CODENAME=bionic
UBUNTU_CODENAME=bionic
```



- 卸载旧版本Docekr

  ```shell
  sudo apt-get remove docker docker-engine docker.io containerd runc
  ```

  

- 配置package index 并且安装packages

  ```shell
  sudo apt-get update
  sudo apt-get install \
      apt-transport-https \
      ca-certificates \
      curl \
      gnupg-agent \
      software-properties-common
  ```

  添加Docker's 官方密钥

  ```shell
  curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add - #通过国外的网站 速度较慢
  curl -fsSL http://mirrors.aliyun.com/docker-ce/linux/ubuntu/gpg | sudo apt-key add - #阿里云镜像 
  ```

  

  安装docker引擎

  ```shell
  sudo apt-get update
  sudo apt-get install docker-ce docker-ce-cli containerd.io #docker-ce 社区版本 docker-ee 企业版（not free）
  ```

  - 需要安装指定版本的Docker

    ```shell
    apt-cache madison docker-ce  
    # 版本号会展示
    docker-ce | 18.06.0~ce~3-0~ubuntu       | https://download.docker.com/linux/ubuntu  xenial/stable amd64 Packages
    sudo apt-get install docker-ce=<VERSION_STRING> docker-ce-cli=<VERSION_STRING> containerd.io
    ```

- 测试安装

  ```shell
  sudo docker run hello-world
  ```

  如果你没有任何镜像文件（image file）会自动下载一个镜像并进行执行。

  

  ```shell
  #测试版本
  docker version 
  ```

  

  

## 二： 使用

###  1.开始使用

#### 1.1 阿里云镜像加速

1. 登陆阿里云，搜索容器镜像加速器。

2. 使用Ubuntu，配置使用加速器

   ```shell
   sudo mkdir -p /etc/docker
   sudo tee /etc/docker/daemon.json <<-'EOF'
   {
     "registry-mirrors": ["https://dbgxxju6.mirror.aliyuncs.com"]
   }
   EOF
   sudo systemctl daemon-reload
   ```

   每个人镜像不一样：

   ![aliyunjingxiang](/home/ubuntu/Pictures/docker/aliyunjingxiang.png)

   #### 测试（类似Client-Server结构）

   - Docker的守护进程运行主机，通过Socket从客户端访问。
   - 多个Client通过服务器对后台链接，利用的docker容器

   1. 对于Docker测试中使用的Hello Docker ，当时走的流程为利用判断本机是否存在Hello Docker ，如若不存在则从Docker Hub上Pull（Down）。

####  1.2 Docker常用命令

1. ##### **帮助命令**

   ```shell
   docker version #docker 版本信息
   docker info #docker 包括所有信息，镜像容器
   docker 命令 --help # 命令使用
   ```

2. ##### **镜像命令**

   ```shell
   docker images --help  # 显示使用方式及其信息
   docker images #用户权限下需要使用sudo
   # repository 镜像的仓库
   # TAG        镜像的标签 lastest 最新
   # IMAGE ID   镜像的ID
   # CREATED    镜像的创建时间
   # SIZE       镜像的大小
   ```

   ![docker image](/home/ubuntu/Pictures/docker/docker image.png)

3. ##### **搜索**

   ```shell
   docker search mysql #全部list
   docker search mysql --filter=STARS=3000  #不低于3000stars
   ```

4. ##### **下载**

   ```shell
   sudo docker pull **** #sudo docker pull mysql tag:latest
   #defaut 
   sudo docker pull mysql:5.7
   #利用联合文件系统，sudo docker pull 所得到的latest mysql 在后面执行的mysql:5.7中会存在重复内容，将不再重复下载
   ```

5. ##### **删除**

   ```shell
   #sudo docker rmi -f IMAGE ID
   #sudo docker rmi -f IMAGE ID IMAGE ID
   #sudo docker rmi -f $(docker images -aq) #递归删除全部
   ```

6. ##### **容器的命令**

   有镜像的情况下，利用镜像创建容器（镜像好比类，容器好比实例化）

   ```shell
   docker pull centos 
   docker run [参数] ID
   # 使用docker run --help
   #--name="Name" 
   #-d  #后台式运行
   #-it #交互式
   #-p  #指定端口 8080:8080
   #-P  #随机主机端口
   sudo docker run -it centos /bin/bash 
   exit
   ```

   ![centos](/home/ubuntu/Pictures/docker/centos.png)

   ![centos2](/home/ubuntu/Pictures/docker/centos2.png)

   ```shell
   sudo docker ps -a #如果没有-a 则查看正在使用的容器
   sudo docker ps -n=[NUM] #最近使用的几个容器
   sudo docker ps -q #只显示当前容器编号
   ```

7. ##### **容器退出**

   ```shell
   exit #直接退出并停止
   Ctrl + P + Q  #容器退出不停止
   ```

8. ##### **删除容器**

   ```shell
   docker rm 容器id #不能删除正在运行的容器 rm -f
   docker rm -f $(docker ps -aq)
   docker ps -a -q|xargs docker rm 
   ```

9. **启动和停止**

   ```shell
   docker start IMAGE ID
   docker restart IMAGE ID
   docker stop IMAGE ID 
   docker kill IMAGE ID 
   ```

####  1.3 Docker其他命令

1. **后台启动**

   ```shell
   docker run -d centos # 后台启动 Run container in background and print container ID
   
   #docker run -d centos  虽然后台运行但是直接停止
   
   #因为启动后没有前台进程，直接停止
   #nginx:容器启动后，发现自己没有提供服务，就会立刻停止
   ```

2. **查看日志**

   ```shell
   docker logs --help
   docker logs -f -t --tail  ID
   docker run -d centos /bin/sh -c "while true;do echo Hu;sleep 1;done"
   #-c, --cpu-shares int                 CPU shares (relative weight)
   #-d, --detach                         Run container in background and
   #                                     print container ID
   
   ```

3. **查看容器的进程信息**

   ```shell
   docker top ID #容器内部命令
   docker inspect ID 
   ```

   ```shell
   [
       {
           "Id": "f6c56d93081a35afd0d89dd18cd469499bb4068c3a12e38e2d15af97e0ee6633",
           "Created": "2020-12-22T06:45:07.838881806Z",
           "Path": "/bin/sh",
           "Args": [
               "-c",
               "while true;do echo Hu;sleep 1;done"
           ],
           "State": {
               "Status": "exited",
               "Running": false,
               "Paused": false,
               "Restarting": false,
               "OOMKilled": false,
               "Dead": false,
               "Pid": 0,
               "ExitCode": 137,
               "Error": "",
               "StartedAt": "2020-12-22T06:45:10.128422941Z",
               "FinishedAt": "2020-12-22T06:47:11.728367539Z"
           },
           "Image": "sha256:300e315adb2f96afe5f0b2780b87f28ae95231fe3bdd1e16b9ba606307728f55",
           "ResolvConfPath": "/var/lib/docker/containers/f6c56d93081a35afd0d89dd18cd469499bb4068c3a12e38e2d15af97e0ee6633/resolv.conf",
           "HostnamePath": "/var/lib/docker/containers/f6c56d93081a35afd0d89dd18cd469499bb4068c3a12e38e2d15af97e0ee6633/hostname",
           "HostsPath": "/var/lib/docker/containers/f6c56d93081a35afd0d89dd18cd469499bb4068c3a12e38e2d15af97e0ee6633/hosts",
           "LogPath": "/var/lib/docker/containers/f6c56d93081a35afd0d89dd18cd469499bb4068c3a12e38e2d15af97e0ee6633/f6c56d93081a35afd0d89dd18cd469499bb4068c3a12e38e2d15af97e0ee6633-json.log",
           "Name": "/funny_feistel",
           "RestartCount": 0,
           "Driver": "overlay2",
           "Platform": "linux",
           "MountLabel": "",
           "ProcessLabel": "",
           "AppArmorProfile": "docker-default",
           "ExecIDs": null,
           "HostConfig": {
               "Binds": null,
               "ContainerIDFile": "",
               "LogConfig": {
                   "Type": "json-file",
                   "Config": {}
               },
               "NetworkMode": "default",
               "PortBindings": {},
               "RestartPolicy": {
                   "Name": "no",
                   "MaximumRetryCount": 0
               },
               "AutoRemove": false,
               "VolumeDriver": "",
               "VolumesFrom": null,
               "CapAdd": null,
               "CapDrop": null,
               "CgroupnsMode": "host",
               "Dns": [],
               "DnsOptions": [],
               "DnsSearch": [],
               "ExtraHosts": null,
               "GroupAdd": null,
               "IpcMode": "private",
               "Cgroup": "",
               "Links": null,
               "OomScoreAdj": 0,
               "PidMode": "",
               "Privileged": false,
               "PublishAllPorts": false,
               "ReadonlyRootfs": false,
               "SecurityOpt": null,
               "UTSMode": "",
               "UsernsMode": "",
               "ShmSize": 67108864,
               "Runtime": "runc",
               "ConsoleSize": [
                   0,
                   0
               ],
               "Isolation": "",
               "CpuShares": 0,
               "Memory": 0,
               "NanoCpus": 0,
               "CgroupParent": "",
               "BlkioWeight": 0,
               "BlkioWeightDevice": [],
               "BlkioDeviceReadBps": null,
               "BlkioDeviceWriteBps": null,
               "BlkioDeviceReadIOps": null,
               "BlkioDeviceWriteIOps": null,
               "CpuPeriod": 0,
               "CpuQuota": 0,
               "CpuRealtimePeriod": 0,
               "CpuRealtimeRuntime": 0,
               "CpusetCpus": "",
               "CpusetMems": "",
               "Devices": [],
               "DeviceCgroupRules": null,
               "DeviceRequests": null,
               "KernelMemory": 0,
               "KernelMemoryTCP": 0,
               "MemoryReservation": 0,
               "MemorySwap": 0,
               "MemorySwappiness": null,
               "OomKillDisable": false,
               "PidsLimit": null,
               "Ulimits": null,
               "CpuCount": 0,
               "CpuPercent": 0,
               "IOMaximumIOps": 0,
               "IOMaximumBandwidth": 0,
               "MaskedPaths": [
                   "/proc/asound",
                   "/proc/acpi",
                   "/proc/kcore",
                   "/proc/keys",
                   "/proc/latency_stats",
                   "/proc/timer_list",
                   "/proc/timer_stats",
                   "/proc/sched_debug",
                   "/proc/scsi",
                   "/sys/firmware"
               ],
               "ReadonlyPaths": [
                   "/proc/bus",
                   "/proc/fs",
                   "/proc/irq",
                   "/proc/sys",
                   "/proc/sysrq-trigger"
               ]
           },
           "GraphDriver": {
               "Data": {
                   "LowerDir": "/var/lib/docker/overlay2/d5f891dbc36a945d9f63f8b9e23a86174cda2a7ea9a548ea99090a7da10f9ac1-init/diff:/var/lib/docker/overlay2/11f029ecda8018d460c040485ae6aa1bca91176f59a52cae1094021ee36ec387/diff",
                   "MergedDir": "/var/lib/docker/overlay2/d5f891dbc36a945d9f63f8b9e23a86174cda2a7ea9a548ea99090a7da10f9ac1/merged",
                   "UpperDir": "/var/lib/docker/overlay2/d5f891dbc36a945d9f63f8b9e23a86174cda2a7ea9a548ea99090a7da10f9ac1/diff",
                   "WorkDir": "/var/lib/docker/overlay2/d5f891dbc36a945d9f63f8b9e23a86174cda2a7ea9a548ea99090a7da10f9ac1/work"
               },
               "Name": "overlay2"
           },
           "Mounts": [],
           "Config": {
               "Hostname": "f6c56d93081a",
               "Domainname": "",
               "User": "",
               "AttachStdin": false,
               "AttachStdout": false,
               "AttachStderr": false,
               "Tty": false,
               "OpenStdin": false,
               "StdinOnce": false,
               "Env": [
                   "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
               ],
               "Cmd": [
                   "/bin/sh",
                   "-c",
                   "while true;do echo Hu;sleep 1;done"
               ],
               "Image": "centos",
               "Volumes": null,
               "WorkingDir": "",
               "Entrypoint": null,
               "OnBuild": null,
               "Labels": {
                   "org.label-schema.build-date": "20201204",
                   "org.label-schema.license": "GPLv2",
                   "org.label-schema.name": "CentOS Base Image",
                   "org.label-schema.schema-version": "1.0",
                   "org.label-schema.vendor": "CentOS"
               }
           },
           "NetworkSettings": {
               "Bridge": "",
               "SandboxID": "b373cfefd744af4c2581f2486acee912f9f03b4c7ae3c1c4e914ad3908c2549c",
               "HairpinMode": false,
               "LinkLocalIPv6Address": "",
               "LinkLocalIPv6PrefixLen": 0,
               "Ports": {},
               "SandboxKey": "/var/run/docker/netns/b373cfefd744",
               "SecondaryIPAddresses": null,
               "SecondaryIPv6Addresses": null,
               "EndpointID": "",
               "Gateway": "",
               "GlobalIPv6Address": "",
               "GlobalIPv6PrefixLen": 0,
               "IPAddress": "",
               "IPPrefixLen": 0,
               "IPv6Gateway": "",
               "MacAddress": "",
               "Networks": {
                   "bridge": {
                       "IPAMConfig": null,
                       "Links": null,
                       "Aliases": null,
                       "NetworkID": "7b2a5376621b25b0763ca0ed515cca4e05a22c08a80f89516c026792a405d3db",
                       "EndpointID": "",
                       "Gateway": "",
                       "IPAddress": "",
                       "IPPrefixLen": 0,
                       "IPv6Gateway": "",
                       "GlobalIPv6Address": "",
                       "GlobalIPv6PrefixLen": 0,
                       "MacAddress": "",
                       "DriverOpts": null
                   }
               }
           }
       }
   ]
   
   ```

4. **进入当前正在运行的容器**

   ```shell
   #方式一
   sudo docker exec -it 容器id /bin/bash  #进入后台交互式,开启新的命令行
   
   #方式二
   sudo docker attach 容器id  /bin/bash # 进入容器正在执行的终端
   ```

5. **容器内拷贝文件到主机上**

   ```shell
   docker cp 容器id:容器内路径 目的的主机路径
   #进入docker 容器
   #主要利用docker id:/home/
   #拷贝利用手动过程
   ```

   ![cp1](/home/ubuntu/Pictures/docker/cp1.png)



![cp2](/home/ubuntu/Pictures/docker/cp2.png)

## 问题

1. Package docker-ce is not available, but is referred to by another package.This may mean that the package is missing, has been obsoleted, or
   is only available from another source.

![issuses1](/home/ubuntu/Pictures/docker/issuses1.png)

**问题原因**：Ubuntu 18.04 Kernal version问题，可使用旧版本的docker

**官方解决办法**：

```shell
sudo apt install apt-transport-https ca-certificates curl software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu bionic stable"
```

**民间**：

```shell
sudo gedit /etc/apt/sources.list.d/docker.list
# 增加下面一行
deb [arch=amd64] https://download.docker.com/linux/ubuntu bionic stable
```

2. Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Get http://%2Fvar%2Frun%2Fdocker.sock/v1.24/images/json: dial unix /var/run/docker.sock: connect: permission denied

   ```shell
   可用:sudo docker images 
   ```

   ```shell
   sudo groupadd docker #添加docker用户组
   
   sudo gpasswd -a $USER docker #将登陆用户加入到docker用户组中
   
   newgrp docker #更新用户组
   ```

   

3. 



## 卸载docker

```shell
#卸载packages
sudo apt-get purge docker-ce docker-ce-cli containerd.io
#删除资源 docker 默认工作路径
sudo rm -rf /var/lib/docker
```

