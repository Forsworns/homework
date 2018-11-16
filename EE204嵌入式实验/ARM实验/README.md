## jtag锁死

使用板子时，理论上只有使用GPIO PORT C时才会发生jtag的锁死。但是，发现如果在SysCtlClockFreqSet中使用过低的时钟频率参数也会导致锁死。

如果jtag被锁死，可以用官网提供的LM Flash Programmer软件解锁，方法如下：

1. 进入Other Utilities部分
2. 在Debug Port部分选择Fury, DustDevil...（因为我们使用的是TM4C1294NCPDT ）
3. 点击Unlock，之后一路确定，弹框会提示按住板子的reset但是不按也行
4. 按照弹框提示拔掉与电脑的连接线重连，重新供电启动板子