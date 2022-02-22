# EncDec 

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

[reference关于socket](https://zhuanlan.zhihu.com/p/39982451)

[offical socket](https://docs.python.org/zh-cn/3/library/socket.html?highlight=socket%20socket#example)

[是否接受完](https://www.cnblogs.com/staff/p/9631916.html)



## C++的函数接口

如果采用C++版本封装成动态库，可参考Pybind方式

python舍弃，c++版本做成静态库后编入动态库，封装后被py调用

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

