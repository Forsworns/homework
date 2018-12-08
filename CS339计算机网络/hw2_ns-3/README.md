### 计算机网络第二次作业

#### 1. 简介 

本次作业中完成了网络拓扑图的构建，对内置的路由协议进行了测试。尝试仿照ns-3路由协议源码修改实现自己的路由协议配置。

目录结构如下：

```shell
homework
├── hw	# 路由协议实现
│   ├── doc
│   │   └── hw.rst
│   ├── examples
│   │   ├── hw-example.cc
│   │   └── wscript
│   ├── helper
│   │   ├── hw-helper.cc
│   │   └── hw-helper.h
│   ├── model
│   │   ├── hw-packet.cc
│   │   ├── hw-packet.h
│   │   ├── hw-packet-queue.cc
│   │   ├── hw-packet-queue.h
│   │   ├── hw-routing-protocol.cc
│   │   ├── hw-routing-protocol.h
│   │   ├── hw-rtable.cc
│   │   └── hw-rtable.h
│   ├── test
│   │   └── hw-test-suite.cc
│   └── wscript
├── image	# 利用output中数据作出的图像
├── ├── hw.png
│   ├── hw_latency.png
│   ├── hw_queue.png
│   ├── hw_throughput.png
│   └── topograph.PNG
├── output	# 测试文件输出的数据文件
│   ├── hw_errorModel.data
│   ├── hw_latency.data
│   ├── hw_onoffApplication.txt
│   ├── hw_queue.data
│   ├── hw_routerAddress.txt
│   ├── hw_routerTable.txt
│   ├── hw_terminalAddress.txt
│   └── hw_throughput.data
├── README.md
└── scratch	# 测试文件
    └── hw.cc
```

#### 2. 分析输出数据文件

网络结构拓扑图如下

![](image\topograph.PNG)

对应输出了`hw_terminalAddress.txt`和`hw_routerAddress.txt` ，分别记录了为主机分配的ip地址和为路由器端口分配的ip地址。其中主机ip地址第三位与所连的路由器有关，相同数值代表连在同一路由器上；路由器间类似方法根据连接关系分配了ip地址。

之后进行了路由协议的测试，利用Simulator::Schedule()及构造自身为回调函数，实现了在主机间随机发送包的同时，每秒记录一次延迟（累计延迟）和吞吐量，每十毫秒查询一次队列长度，分别存储在`hw_latency.data`  ， `hw_throughput.data`和 `hw_queue.data` 。应用aodv路由协议测试了50s，gnuplot作出的图像如下

![](D:\文档\大三上\计算机网络\sprj2\homework\image\hw_latency.png)

![](D:\文档\大三上\计算机网络\sprj2\homework\image\hw_throughput.png)

![](D:\文档\大三上\计算机网络\sprj2\homework\image\hw_queue.png)

`hw_errorModel.data`中是添加错误模型后产生随机错误的时间。

`hw_networkTrace.tr`中是测试时的详细记录文件过大接近1G不便上传。

`hw_routerTable.txt` 中是在开始模拟时路由表的打印信息。

`hw_onoffApplication.txt` 中是onoffApplication发送的记录。

路由协议模块安装截图

![](image/hw.png)

#### 3. 测试方法

以下用`ns-3/` 代指ns-3的根目录

```shell
# 测试文件
cd homework
cp scrach ns-3/scratch
cd ns-3-directory
mkdir output

./waf --run "scratch/hw --verbose=true" 
# use --verbose means to show some output on your screen
# use --protocol to change the protocol used
# use --time to change the simulating time
```

```shell
# 绘图
gnuplot
set terminal png size 640,480
set output "figure name.png"
plot "data name.data" using 1:2 title "title name" with linespoints
exit
```

```shell
# 添加自制的路由模块 
# cd ns-3/src
# ./create-module.py hw
cd homework
cp hw ns-3/src/hw
./waf configure --enable-examples --enable-tests
./waf build
# now hw-module.h is in build/debug/ns3/
```

#### 4. 自定义路由机制

自定义路由机制修改了ns-3源码中dsdv路由机制。dsdv的目标是在每个路由节点存储一个路由表，对每个目的地址存储下一跳节点。选取下一跳节点的规则是：首先给每条路径设置一个路由跳数和序列号。用路由跳数可以表示两点间的距离，但是注意这个路由跳数并不是固定的，是动态生成的，初始均为无穷大，在有包传递过来的时候填写，因此在包中需要添加信息记录路由的转发数目。而序列号，则表示路径信息的新鲜程度和路径的优劣，所以包中需要添加信息维护转发时的序列号。而路径的选择方式十分朴素：序号相同时选择跳数少的路径；否则按照序号顺序选择序列号更大的路径。

- 首先同样构造了一个helper用于辅助安装该路由协议。在`hw-helper.h`和`hw-helper.cc`中，继承了`Ipv4RoutingHelper` 类，主要基于`ObjectFactory`类实现了`Create()`，`Copy()`，`Set()`等基本的函数。有了这样的`helper`之后，可在测试代码`hw.cc`中对`Ipv4ListRoutingHelper`的实例对象添加该路由协议及设定的优先级，之后通过节点协议栈`InternetStackHelper`类的实例对象的`SetRoutingHelper`函数协议栈中的路由协议。
- 自定义了HwHeader的结构，于`hw-packet.h`和`hw-packet.cc` ，继承自Header，存储了目标地址，当前跳数，当前序列号。需要注意的是，在ns-3中，header除了要实现一些基础的setter，getter方法，还要做序列化和逆序列化，将结构体转化后，在`Packet::AddHeader()` 中调用将packet和header结合起来转发。
- 之后定义了用于记录路由表的数据结构和表中的条目，于`hw-rtable.cc`和`hw-rtable.h` 。路由表项中有序列号，路由转发跳数，路由路径，存储时间等信息。
- 于`hw-packet-queue.h`和`hw-packet-queue.cc ` 定义了两个类，一个是队列中的元素，队列中缓存有包（`m_packet`），刚刚定义的`HwHeader`对象`m_header，`队列项过期的时间（m_expire）等；另一个类是队列，除了一般的队列方法实现，主要还实现了超时丢弃，针对固定目标地址丢弃的方法。
- `hw-routing-protocol.cc`和`hw-routing-protocol.h` 中是协议的具体实现。

最后修改hw目录下的wscript，进行编译。

#### 5. 测试所用公式及函数

测试文件`hw.cc`中通过Schedule()函数实现计算/输出吞吐量，队列长度，延迟的函数如下

```c++
// 输出在ReceivePacket()函数中计算的吞吐量
void RoutingExperiment::CheckThroughput()
{
    NS_LOG_INFO("Set the throughtput checker");
    uint32_t bitPerSecond = m_bytesTotal * 8.0;
    m_bytesTotal = 0;
    std::ofstream fout(fileHeader+throughputFile, std::ios::app);
    fout << (Simulator::Now()).GetSeconds() << " "
        << bitPerSecond << " "
        << m_packetsReceived
        << std::endl;
    fout.close();
    m_packetsReceived = 0;
    Simulator::Schedule(Seconds(1.0), &RoutingExperiment::CheckThroughput,this);
}

// 遍历所有路由节点寻找最长的队列
void RoutingExperiment::CheckQueueLength(NodeContainer &routers){
    NS_LOG_INFO("Set the queue checker");
    uint32_t queueMaximum = 0;
    for(int i = 0; i<routerNum; ++i)
    {
        Ptr<Node> tempNode = routers.Get(i);
        uint32_t queueTemp = 0;
        for(uint16_t j=1;j<tempNode->GetNDevices();++j)
        {
            queueTemp += tempNode->GetDevice(j)->GetObject<PointToPointNetDevice>()->GetQueue()->GetNBytes();
        } 
        queueMaximum = queueMaximum > queueTemp ? queueMaximum : queueTemp;
    }
    std::ofstream fout(fileHeader+queueFile, std::ios::app);
    fout << (Simulator::Now()).GetSeconds() << " "
        << queueMaximum
        << std::endl;
    fout.close();
    Simulator::Schedule(Seconds(0.01),&RoutingExperiment::CheckQueueLength,this,routers);
}

// 时延（计算的是当前累计时延之和，可以通过图像斜率判断某一时刻延迟大小）
void RoutingExperiment::CheckLatency(FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor)
{
    NS_LOG_INFO("Set the latency checker");
    double latency = 0;
    monitor->CheckForLostPackets ();
    std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats ();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier ());
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = flowStats.begin (); i != flowStats.end (); ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
        // UDP_PROT_NUMBER = 17
        if (17 == unsigned(t.protocol))
        {
            latency += i->second.delaySum.GetSeconds ();
        }
        else
        {
            std::cout << "This is not UDP traffic" << std::endl;
        }
    }
    std::ofstream fout(fileHeader+latencyFile, std::ios::app);
    fout << (Simulator::Now()).GetSeconds() << " "
        << latency
        << std::endl;
    fout.close();
    Simulator::Schedule(Seconds(1.0),&RoutingExperiment::CheckLatency,this, fmhelper, monitor);
}
```



#### 6. 结语
