# 关于Python封装成动态库

[reference知乎](https://zhuanlan.zhihu.com/p/265462717)

采用setup.py方式进行安装

安装cython

```text
pip install cython
```

和开发环境

里面包含python头文件和gcc的编译器

```
apt-get install python-devel
apt-get install gcc
```

接下来内容采用

设置setup.py

```python
from distutils.core import setup
from Cython.Build import cythonize
setup(ext_modules = cythonize(["hello1.py", "hello2.py","hello3.py"]))
```

执行如下命令

```
python setup.py build_ext
```

