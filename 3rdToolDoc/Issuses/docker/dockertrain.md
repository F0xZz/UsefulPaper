Docker-Hub:[参考网站](https://hub.docker.com/)

Docker-doc:[参考网站](https://docs.docker.com/)

#  Docker 实战

##  Mirror 寻找

###  Docker 安装Nginx

```shell
ubuntu@ubuntu-Default-string:~$ sudo docker pull nginx  #下载镜像源码
# -d 后台运行 -p 映射端口 --name 命名
ubuntu@ubuntu-Default-string:~$ sudo docker run -d --name nginx01 -p 3344:80 nginx
61f5b2d82247c7d9a82b57bc7a570743d5648bfcaa9386f8dc62bf339f5b3cb5
# ps -a 对仓库list
ubuntu@ubuntu-Default-string:~$ sudo docker ps -a
CONTAINER ID   IMAGE     COMMAND                  CREATED          STATUS         PORTS                  NAMES
61f5b2d82247   nginx     "/docker-entrypoint.…"   11 seconds ago   Up 8 seconds   0.0.0.0:3344->80/tcp   nginx01
#curl localhost 从宿主内部映射3344到容器内部端口port80
ubuntu@ubuntu-Default-string:~$ curl localhost:3344
<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
<style>
    body {
        width: 35em;
        margin: 0 auto;
        font-family: Tahoma, Verdana, Arial, sans-serif;
    }
</style>
</head>
<body>
<h1>Welcome to nginx!</h1>
<p>If you see this page, the nginx web server is successfully installed and
working. Further configuration is required.</p>

<p>For online documentation and support please refer to
<a href="http://nginx.org/">nginx.org</a>.<br/>
Commercial support is available at
<a href="http://nginx.com/">nginx.com</a>.</p>

<p><em>Thank you for using nginx.</em></p>
</body>
</html>
```

```shell
ubuntu@ubuntu-Default-string:~$ sudo docker exec -it nginx01 /bin/bash  
root@61f5b2d82247:/# whereis nginx 
nginx: /usr/sbin/nginx /usr/lib/nginx /etc/nginx /usr/share/nginx
root@61f5b2d82247:/# cd /etc/nginx/
root@61f5b2d82247:/etc/nginx# ls 
conf.d	fastcgi_params	koi-utf  koi-win  mime.types  modules  nginx.conf  scgi_params	uwsgi_params  win-utf
root@61f5b2d82247:/etc/nginx# 
```

###  Tomcat 容器

```
#官方使用（用完即删，测试用）
docker run -it  --rm tomcat:9.0
#部署
docker pull tomcat:9.0
```

###  Commit 镜像

利用tomcat:9.0

```shell
sudo docker pull tomcat:9.0
sudo docker run -it -p 3344:8080 -name=tomcat01 tomact:9.0 
#利用3344:8080 创建 tomcat:9.0 容器 
ubuntu@ubuntu-Default-string:~$ sudo docker ps -a 
CONTAINER ID   IMAGE                 COMMAND             CREATED          STATUS          PORTS                            NAMES
d7e43f21243e   tomcat:9.0            "catalina.sh run"   13 minutes ago   Up 13 minutes   8080/tcp, 0.0.0.0:3344->80/tcp   tomcat01
ubuntu@ubuntu-Default-string:~$ sudo docker exec -it d7e4
root@d7e43f21243e:/usr/local/tomcat# ls
BUILDING.txt	 NOTICE		RUNNING.txt  lib	     temp	   work
CONTRIBUTING.md  README.md	bin	     logs	     webapps
LICENSE		 RELEASE-NOTES	conf	     native-jni-lib  webapps.dist

```



```shell
ubuntu@ubuntu-Default-string:~$ sudo docker ps -a 
CONTAINER ID   IMAGE                 COMMAND             CREATED          STATUS          PORTS                            NAMES
d7e43f21243e   tomcat:9.0            "catalina.sh run"   13 minutes ago   Up 13 minutes   8080/tcp, 0.0.0.0:3344->80/tcp   tomcat01

sudo docker commit 提交容器

sudo docker commit -m="描述信息" -a="作者名" #容器id 目标镜像名[TAG] 
#提交一个修改后的镜像
```

![commit2](/home/ubuntu/Pictures/docker/commit2.png)



 ## 容器数据卷

### 数据卷（容器外部修改后内部修改）

**docker将应用和环境打包成一个镜像**

数据并不可以放在容器内，一旦容器删除，数据就会丢失！ **数据持久化**

**类似MySQL，容器删除了GG**

**数据共享，同步到本地，目录挂在，容器的目录挂载到Linux上，容器之间可以进行数据共享**

#### 使用数据卷

```shell
方式一：直接使用命令进行挂载
sudo docker run -it -v 主机目录:容器目录 # 类似-p 端口映射

ubuntu@ubuntu-Default-string:~$ sudo docker run -it -v /home/ubuntu/Dockertes:/hoome centos /bin/bash 
[root@3f0764151bd1 /]# 
#本机出现Docertes
```

![shujujuan](/home/ubuntu/Pictures/docker/shujujuan.png)



```shell
sudo docker inspect 3f07 #可查看挂载信息
```

![guazai](/home/ubuntu/Pictures/docker/guazai.png)

**双向绑定**容器内部增加修改，都会对主机产生影响，即使停止Docker 容器，对本地目录进行操作也会更新容器目录

#### 具名挂载和匿名挂载

```shell
#-v 容器内路径 匿名挂载
sudo docker run -d -P --name nginx01 -v /etc/nginx nginx:latest #-P 随机映射端口
#-v 卷名:容器内路径 具名挂载
sudo docker run -d -P --name nginx01 -v jmguazai:/etc/nginx nginx:latest
#使用docker inspect 可查看具体位置
sudo docker volume inspect jmguazai 
#没有指定目录一般挂载在 /var/lib/docker/volumes/XXXX/_data
#可以方便的找到一个卷
#一旦设定容器权限，对挂载的内容进行限定了，仅仅能从外部改变通过宿主机进行改变
sudo docker run -d -P --name nginx01 -v jmguazai:/etc/nginx:ro nginx:latest # ro read only  rw readwrite 读写权限
```

### 容器与容器之间挂载

#### **多个mysql数据同步**

利用**下文中Dockfile步骤中所创建的镜像**创建两个centos仓库

```shell
ubuntu@ubuntu-Default-string:~$ sudo docker run -it --name centos02 foxcentos:1.0
ubuntu@ubuntu-Default-string:~$ sudo docker run -it --name centos03 --volumes-from centos02 foxcentos:1.0
ubuntu@ubuntu-Default-string:~$ sudo docker ps 
CONTAINER ID   IMAGE                 COMMAND                  CREATED          STATUS          PORTS                    NAMES
d46e99791479   foxcentos:1.0         "/bin/sh -c /bin/bash"   29 seconds ago   Up 25 seconds                            centos03
9050e9b71425   foxcentos:1.0         "/bin/sh -c /bin/bash"   7 minutes ago    Up 7 minutes                             centos02
#从上面看见创建了两个Centos仓库
```

```shell
#在centos02中创建的container01.txt在centos03的volume01中也可见 类似son extend father 
```

![dockerstorage1](/home/ubuntu/Pictures/docker/dockerstorage1.png)

多个centos挂载，同步共享，可理解为对宿主机内的volume进行同步挂载，可以从inspect 进行查看，都是宿主机内的共享

```shell
docker run -it --name centos04 --volumes-from centos03 foxcentos:1.0
```



```shell
sudo docker inspect centos04 
```



## Dockerfile的使用(方便后续 类似批处理创建镜像)

### 初步使用Dockfile

方式二:使用命令脚本，通过一个脚本可以生成一个镜像

类似用来构建docker镜像的构建文件，命令脚本.

创建dockfile1，内容如下，**第一行使用centos images，第二行是映射，第三行是输出打印，第四行是/bin/bash**

```shell
#都是大写
#每一步构建，逐步
FROM centos

VOLUME ["volume01","volume02"]

CMD echo "----------end-----------"
CMD /bin/bash
```

```shell
sudo docker build -f dockfile1 -t foxcentos:1.0 . #注意后面的点
```

![dockerfile](/home/ubuntu/Pictures/docker/dockerfile.png)

![dockerfile1](/home/ubuntu/Pictures/docker/dockerfile1.png)



```shell
#使用自己镜像
#第一种方式
ubuntu@ubuntu-Default-string:~/docker-test-volume$ sudo docker run -it foxcentos:1.0 /bin/bash
[root@1a5ed212189e /]# ls -l
#第二种方式
sudo docker run -it 789f8e9627da /bin/bash
ls -l
```

![dockerfile2](/home/ubuntu/Pictures/docker/dockerfile2.png)

**可以看到volume01 volume02被挂载**至于挂载的目录同步目录在: 匿名挂载

```shell
sudo docker inspect 1a5ed212189e #根据个人目录来
#可以查到挂载路径
```

![dockerfileguazai](/home/ubuntu/Pictures/docker/dockerfileguazai.png)

可以在需要的文件夹下进行搜索到你所在容器内创建的文件

**这种匿名挂载方式将会常用**

### Dockfile文件的编写

1. 编写dockfile文件
2. docker build构建一个镜像
3. docker run 运行镜像成立仓库
4. docker push 发布镜像（可利用DokcerHub、阿里云镜像仓库）

**指令**全部大写字母，保留关键字，执行从上到下。

DockerFile 定义了一切步骤，源码。

### Dockerfile的指令

![dockerfilecommand](/home/ubuntu/Pictures/docker/dockerfilecommand.png)



```shell
FROM             #基础镜像 centos
MAINTAINER       #镜像维护者信息
RUN              #命令
ADD              #本地文件，add tomcat
WORKDIR          #工作目录
VOLUME           #存放位置
EXPOST           #暴露端口
CMD              #指令启动运行的命令，只使用最后一条CMD
ENTRYPOINT       #指令容器命令，但是可以追加
ONBUILD          #当构建一个dockfile镜像，ONBUILD 的指令
COPY             #类似ADD 
ENV              #设置环境变量
```

### 测试使用

编写一个dockerfile2文件内容如下

```shell
FROM centos 
MAINTAINER Fox<NU>

ENV MYPATH /usr/local 
WORKDIR $MYPATH

RUN yum -y install vim 
RUN yum -y install net-tools

EXPOSE 80

CMD echo $MYPATH
CMD echo "---------end-----------"
CMD /bin/bash
```

```shell
sudo docker build -f dockerfile2 -t mycentos:0.1 .
```

![add command](/home/ubuntu/Pictures/docker/add command.png)

```shell
CMD # 
ENTRYPOINT # 
```

### 使用自主配置

编写dockerfile 文件

增加本机文件，配置tomact的压缩包，jdk压缩包

```shell
FROM centos 
MAINTAINER Fox<list>

ADD apache-tomcat-9.0.41.tar.gz /usr/local/
ADD jdk-15.0.1_linux-x64_bin.tar.gz /usr/local/

RUN yum -y install vim 

ENV MYPATH /usr/local
WORKDIR $MYPATH

ENV JAVA_HOME /usr/local/jdk-15.0.1
ENV CLASS_PATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
ENV CATALINA_HOME /usr/local/apache-tomcat-9.0.41
ENV CATALINA_BASH /usr/local/apache-tomcat-9.0.41
ENV PATH $PATH:$JAVA_HOME/bin:$CATALINA_HOME/lib:$CATALINA_HOME/bin

EXPOSE 8080

CMD /usr/local/apache-tomcat-9.0.41/bin/startup.sh && tail -F /url/local/apache-tomcat-9.0.41/bin/logs/catalina.out
```

构建镜像:

```shell
sudo docker build -f Dockerfile -t tomcatjdk:0.1
```

镜像实例化仓库:

```shell
sudo docker run -d --name 01 tomcatjdk:0.1
```

访问测试:

```shell
sudo docker exec   /sudo docker attach
```

### 镜像发布

**发布镜像:**官方网站

```shell
#注册自己的账号
#打开DockerHub
#hub.docker.com
sudo docker login -u id
password
sudo docker push tomcatjdk:0.1
#push 自己的东西上去

#error show in below
#The push refers to repository [docker.io/ubuntu/tomcatjdk]
#An image does not exist locally with the tag: ubuntu/tomcatjdk
ubuntu@ubuntu-Default-string:~$ sudo docker tag 70c11ab749d8 dockertest20201221/tomcatjdk:1.0
ubuntu@ubuntu-Default-string:~$ sudo docker push dockertest20201221/tomcatjdk:1.0
即可
```

​	在我们的服务器上提交自己的镜像

**阿里云镜像**

```shell
#登陆阿里云
#找到容器镜像服务
#创建命名空间
#创建仓库

$ sudo docker login --username=momowwww registry.cn-hangzhou.aliyuncs.com
$ sudo docker tag [ImageId] registry.cn-hangzhou.aliyuncs.com/fox930619/fox-test:[镜像版本号]
$ sudo docker push registry.cn-hangzhou.aliyuncs.com/fox930619/fox-test:[镜像版本号]
```



##  使用MySQL

**拉取镜像**

```shell
sudo docker pull mysql:5.7 #获取镜像
#  docker run --name some-mysql -e MYSQL_ROOT_PASSWORD=my-secret-pw -d mysql:tag 官方mysql 使用
sudo docker run -d -p 3310:3306 --name mysql01 -v /home/ubuntu/mysql:/etc/mysql -v /home/ubuntu/mysql/data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=123456 mysql:5.7  

#运行时进行数据挂载 ***挂载的时候默认将主机目录内文件直接覆盖容器内文件，如若需要容器内文件，需进入后对其进行cp后再进行挂载***

#可以进行 sqlyog进行链接
```



## 图形化面板

portainer

```shell
sudo docker run -d -p 8080:9000 --restart=always -v /var/run/docker.sock:/var/run/docker.sock --privileged=true portainer/portainer

```

![portainer](/home/ubuntu/Pictures/docker/portainer.png)

ip ad:

```
ifconfig -a
inlet 内网
```

## 问题

1. cd /var/lib/docker/volumes/6877ea4433dc07aaae933e7ef655dbb012300b6c826d48e92673e4d9e7b5575a/_data
   bash: cd: /var/lib/docker/volumes/6877ea4433dc07aaae933e7ef655dbb012300b6c826d48e92673e4d9e7b5575a/_data: Permission denied

   **网上部分解决办法行不通**：

   ```shell
      #原因是CentOS7中的安全模块selinux把权限禁掉了，至少有以下三种方式解决挂载的目录没有权限的问题：
      #1.在运行容器的时候，给容器加特权，及加上 --privileged=true 参数：
      docker run -i -t -v /soft:/soft --privileged=true 686672a1d0cc /bin/bash
      #2.临时关闭selinux：
      setenforce 0
      #3.添加selinux规则，改变要挂载的目录的安全性文本
      
   ```

   **直接解决**

   ```shell
   #直接给文件赋予权限
   sudo chmod 777 -R /var/lib/docker
   virtualenv py36 --no-site-packages -p /usr/bin/python3

   ```
   
   

























