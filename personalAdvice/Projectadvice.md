#关于工程上的问题

- 小目标问题

尝试过的解决方法1080p底下20pixel以下的物体

解释：scale一直是检测器的问题，感受野的影响下，会对小目标检测不出，不用担心大尺度检测不准，CNN原本就具有尺度鲁棒性和旋转鲁棒性。

method1 : slid windows 滑窗 最直接，效果显著 但是造成inference的时间是成倍增长，汇总结果进行NMS

method2 : 利用更大分辨率inference，效果并不显著，有一定的效果，但是cast memory will increase

method3 : data Aug 小目标数据增强的话比较常见，个人脚本提供了一个demo关于xml下voc格式的小目标增强，有一定效果更偏向于少样本的情况下

method4 :train method 多尺度训练，每隔多少epoch更换一下size 进行训练

method5 :直接上分割模型

method6 :特殊场景也许可结合传统算法进行运算

- 工程上算子加速和优化推荐

对于大量的if操作，和逻辑运算，乘积运算，会存在一部分CPU在搁那调度一个CPU在那使劲卖力计算的情况。

method1: c++上多线程操作 ，py感觉GIL的问题，有方案总体还是上C++吧，小数据Cuda不建议使用有可能还没送入显存和从显存读取的的时间多

method2 : gemm优化，更多偏向于乘积运算里的优化

method3 : 基于method2的指令集优化

method4 : 去pr领导找个大佬加速，并拓展技术

method5 : 暂时想不到

- 尽可能的前后端分离，尽可能的分开自己的所有模块以方便自己的测试

