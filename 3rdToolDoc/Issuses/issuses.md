#  遇到问题

1. Docker-Hub:[参考网站](https://hub.docker.com/)

2. Docker-doc:[参考网站](https://docs.docker.com/)
3. Nvidia-Docker:[参考下载网站](https://download.docker.com/linux/ubuntu/dists/bionic/pool/stable/amd64/)
4. Nvidia-Docker-doc:[参考网站](https://github.com/NVIDIA/nvidia-docker)

5. 端口外部TCP映射问题:<span name = "端口映射">[网址](https://www.cnblogs.com/kevingrace/p/9453987.html)</span>

6. 端口占用问题:[网址](https://www.cnblogs.com/yingchen/p/10113751.html)

##  command line 

```shell
sudo apt --fix-broken install # 安装丢失包可使用该指令
sudo cp 
sudo mv 
#display verison
sudo dpkg-reconfigure lightdm/gdm3
```

```shell
#if you wanna to make sure that then driver whether or not  in the system 
#use this command
lspci | grep -i vga
#find out the display's version in the website
# http://pci-ids.ucw.cz/mods/PC/10de?action=help?help=pci
```

python version

```
sudo update-alternatives --install /usr/bin/python python /usr/bin/python2 100
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 150
```



```
版本升级 (https://www.jb51.net/article/182392.htm)
sudo add-apt-repository ppa:jonathonf/python-3.6 #choose the verison that you wanna to download 
可以使用which python3
或者直接使用virtualenv


pip install llvmlite==0.35.0 -i http://mirrors.aliyun.com/pypi/simple --trusted-host mirrors.aliyun.com

-i http://mirrors.aliyun.com/pypi/simple --trusted-host mirrors.aliyun.com

pip install torch==0.4.0 torchvision==0.2.2 -f https://download.pytorch.org/whl/cu100/torch_stable.html -i http://mirrors.aliyun.com/pypi/simple --trusted-host mirrors.aliyun.com
```

```shell
#make sure which one ubuntu's kernal 
sudo lsb_release -a
#download the rep not install the softwar 
apt download ****
#make sure the dependence of the soft 
apt-cache depends ***
#split the file  
tar -zcvf - rocketmq.tar.gz | split -b 1G -d -a 1 - rocketmq.tar.gz.
#fix the file 
cat test.tar.gz.* > test.tar.gz
#find the version
sudo apt-cache madison libip4tc0

```

kill the nonreturn mission

```shell


首先查看是否有进程占用，有则kill

ps -e | grep apt 或者 ps -e | grep apt-get

kill -9 进程id

没有，可以强制清锁

sudo rm /var/cache/apt/archives/lock 

sudo rm /var/lib/dpkg/lock

注意：清锁后必须reboot系统才能生效

```

```shell
sudo vi /etc/resolv.conf
#add the dns in the conf
nameserver 223.5.5.5  
nameserver 223.6.6.6
sudo apt-get update
```

**select the version of package**

```shell
sudo apt-cache madison qt5-default
```

```
cat /etc/group
```



```ruby
sudo cat /etc/shadow
```

```
no moudule python.h
sudo apt-get install python3-dev
```

