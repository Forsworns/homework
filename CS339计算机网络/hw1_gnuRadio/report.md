## 计算机网络第一次作业

#### 1. 作业要求

利用gnuradio实现WiFi接收端（OFDM子载波32,64,128可切换）。

#### 2. 模块功能

提交的example目录下的grc文件中的模块就是我们所用到的。其中在FileSink之前的部分均为发送端所需的模块，下面仅介绍接收端的具体功能及部分实现原理。

* Polyphase Clock Sync和delay，Complex Conjugate三个模块均为gnu-radio自带的模块，用来配合Coarse CFO Fixer进行子载波偏移的计算和修正，CFO即carrier frequency offset的缩写。由于OFDM技术依赖子载波的正交性，而在实际传输中，子载波会出现偏移而导致子载波的正交性受到影响，导致子载波之间的信息相互干扰，难以处理。将经过多相时钟同步的信号和一个经过时延处理后的信号送入该模块，消除子载波偏移，应用的算法是基于循环前缀的OFDM同步算法，参考了他人的实现。
* Symbol Sync是一个将串行信号转变为并行信号的模块，这里我们讨论64个子载波的情况，在64个子载波中间安插了4个pilot信号，12个空白的信号，均用来辅助信息的降噪，另外48个为实际负载数据的子载波。该模块为之后做快速傅里叶变换做了准备，快速傅里叶变换的模块是gnu-radio自带的模块，这里变换的规模和在发送端约定的子载波数有关。通过快速傅里叶变换将发送时逆变换处理的信息复原，进行了时频域的转换
* Symbol Parser对FFT处理后的信号，利用之前的进行同步降噪等处理，并提取出数据信息。
* Block Decoder进行的是解调工作，这里利用了维特比算法(Viterbi algorithm)，它是一种动态规划算法，用于寻找最可能产生观测到的事件的序列，从而推断发送端在进行调制时选择的调制方式，该部分较复杂且不在要求内，我们选择直接采用了别人的实现方式。经过这个模块，我们取到了协议数据单元，并在PPDU Sink中拆解数据单元，最后输出数据信息到Socket PDU模块显示在终端中。

#### 3. 模块连接

以OFDM子载波数为64为例，连接如下图

![avator](ofdm_32_64_128\examples\models.png)

#### 4. 结果演示

以OFDM子载波为64为例，我们进行了如下测试

```shell
# in the first terminal
nc -u 127.0.0.1 8000
# in the second terminal
nc -u -l 127.0.0.1 8001
```

![avator](ofdm_32_64_128\examples\test.png)

#### 5. 小组成员

#### 6. 测试环境搭建

```shell
# make sure you have installed gnu-radio
cd ofdm_32_64_128
mkdir build
cd build
cmake ..
sudo make install
```

#### 7. 小结
