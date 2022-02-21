#

参考如下两个

c++版本

https://github.com/NEWPLAN/SMx 

python版本

https://github.com/bufbrane/SM4_cipher

python:

[关于进制转换](https://www.cnblogs.com/liao-lin/p/9882488.html)

[ASCII码](https://www.qqxiuzi.cn/bianma/ascii.htm)

[json格式化](https://tool.chinaz.com/Tools/jsonformat.aspx)

json是否完全一样利用工具

[json_tools](https://www.cnblogs.com/ailiailan/p/13921082.html)

json数据类型为dict

关于python [dict操作](https://blog.csdn.net/JNingWei/article/details/78757673)参考这个文章

端口开发问题:

```
第二种方式
             安装iptables-services ： yum install iptables-services

             进入下面目录进行修改： /etc/sysconfig/iptables

 

linux系统的端口设置在/etc/sysconfig/iptables文件中配置。使用编辑器打开该文件。内容如下：

# Firewall configuration written by system-config-firewall

# Manual customization of this file is not recommended.

*filter

:INPUT ACCEPT [0:0]

:FORWARD ACCEPT [0:0]

:OUTPUT ACCEPT [0:0]

-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

-A INPUT -p icmp -j ACCEPT

-A INPUT -i lo -j ACCEPT

-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT

-A INPUT -j REJECT --reject-with icmp-host-prohibited

-A FORWARD -j REJECT --reject-with icmp-host-prohibited

COMMIT

网上说的是如下code

-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 3001 -j ACCEPT

我在CentOS6.5中测试上面的代码，不能成功。

如果我们需要对外开放80端口，则上面文件中添加如下code

-A INPUT -m state --state NEW -m tcp -p tcp --dport 8080 -j ACCEPT

同时还需要注意的是，这段代码需要加入到

-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT

之后，否则端口也不能打开。最后的配如下：

# Firewall configuration written by system-config-firewall

# Manual customization of this file is not recommended.

*filter

:INPUT ACCEPT [0:0]

:FORWARD ACCEPT [0:0]

:OUTPUT ACCEPT [0:0]

-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

-A INPUT -p icmp -j ACCEPT

-A INPUT -i lo -j ACCEPT

-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT

-A INPUT -m state --state NEW -m tcp -p tcp --dport 8080 -j ACCEPT

-A INPUT -j REJECT --reject-with icmp-host-prohibited

-A FORWARD -j REJECT --reject-with icmp-host-prohibited

COMMIT

编辑上面的文件 需要提供su权限.

保存上面的文件后，在终端运行如下命令：更新防火墙配置

service iptables restart

下面这个命令可以看到开放的端口

/sbin/iptables -L -n

下面的命令可以关闭/打开防火墙（需要重启系统）

开启： chkconfig iptables on

关闭： chkconfig iptables off

下面的代码可以启动和停止防火墙（立即生效，重启后失效）

开启： service iptables start 

关闭： service iptables stop
————————————————
版权声明：本文为CSDN博主「积水潭坛主」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/tian_yan71/article/details/119184312
```

[reference关于socket](https://zhuanlan.zhihu.com/p/39982451)

[offical socket](https://docs.python.org/zh-cn/3/library/socket.html?highlight=socket%20socket#example)

[是否接受完](https://www.cnblogs.com/staff/p/9631916.html)

```
lsof -i:1935
nmap localhost
```

**combine文件夹中有获取processid info和自启动还有str的形式 str->list->dict->str->json**

**jiexi内含有 json->str->hex->list->encode->decode->list->hex->str->json的形式**

现在需要一个json传输的socket 通信长时挂载的接口

需要有一个函数测试socket挂载以后不停下来，然后监听某个情况如果掉了发信号



---



## C++的函数接口

python舍弃，c++版本做成静态库后编入动态库，封装后被py调用

仅仅需要的为:

```
easy_encrypt.cpython-38-aarch64-linux-gnu.so
```

内部存在可以用buffer增加异或运算

封装成easy_encrypt模块

函数如下：全部以需要将char转int作为list[int]传入

每次编码都是为16的倍数，多于部分需要增加补0

getkey(int,list[int])

`int list[int] 的长度`

`list[int] 明文公钥`

`返回私钥`

xsyenc(int,list[int],list[int]) 该函数内部加了MAC地址的异或

`int 编码字符长度16倍数`

`list[int] 文字内容`

`list[int] 私钥`

`返回编码后内容`

xsydec(int,list[int],list[int]) 该函数内部加了MAC地址的异或

`int 编码字符长度16倍数`

`list[int] 待解码`

`list[int] 私钥`

`返回解码后内容`

codeenc(int,list[int],list[int]) 没有经过MAC异或

`int 编码字符长度16倍数`

`list[int] 待解码`

`list[int] 私钥`

`返回编码后内容`

codedec(int,list[int],list[int])

`int 编码字符长度16倍数`

`list[int] 待解码`

`list[int] 私钥`

`返回解码后内容`



```
import easy_encrypt as m 
a = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
key = m.getkey(16,a)
input = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
input32 = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
encmask = m.xsyenc(16,input,key)
output = m.xsydec(16,encmask,key)
test1 = [0,2,3,5,8,9,12,13,16,18,19,20,12,3,1,1,2,5,6,4]
encmasktest1 = m.xsyenc(len(test1),test1,key)
outputtest1 = m.xsydec(len(encmasktest1),encmasktest1,key)
test2 = [1,2,3,4]
encmasktest2 = m.xsyenc(len(test2),test2,key)
outputtest2 = m.xsydec(len(encmasktest2),encmasktest2,key)

input3 = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
test3 = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
keytest3 = m.getkey(16,test3)
encmask3 = m.codeenc(16,input3,keytest3)
output3 = m.codedec(16,encmask3,keytest3)
input4 = [0,1,2,3,4]
encmask4 = m.codeenc(5,input4,keytest3)
output4 = m.codedec(5,encmask4,keytest3)
input5 = [0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3]
encmask5 = m.codeenc(16,input5,keytest3)
output5 = m.codedec(16,encmask5,keytest3)
```

