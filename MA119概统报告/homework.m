static = textread('E:\编程\概率统计大作业\statics.txt');%[题号，通过率，AC数,提交数]
for i=(1:3034)  %把比率转化成精确的值
    static(i,2)=static(i,3)/static(i,4);
end

%数值计算分析部分
[maxNumMatrix,maxNumPosMatrix]=max(static);%求取最大值返回的是每列的最大值和序号
maxRadio=maxNumMatrix(1,2);
maxRadioProblem=static(maxNumPosMatrix(1,2));%maxNumPos存的第二个数就是static中第二列最大值的位置，故为通过率最高的题目题号
[minNumMatrix,minNumPosMatrix]=min(static);
minRadio=minNumMatrix(1,2);
minRadioProblem=static(minNumPosMatrix(1,2));%通过率最低的题目题号

meanMatrix=mean(static);%平均值
meanNum=meanMatrix(1,2);
medianMatrix=median(static);%中间值
medianNum=medianMatrix(1,2);
stdMatrix=std(static);%标准差
stdNum=std(1,2);
corrcoefMatrix=corrcoef(static);%相关系数，每一列当前列与每一列的相关系数故(1,2)或(2,1)为题号和通过率的关系，(1,4)或(4,1)为题号与提交数的关系
orderRadio=corrcoefMatrix(1,2);
orderSubmit=corrcoefMatrix(1,4);
tmpText=sprintf('通过率最高的为第%d题,通过率是%d',maxRadioProblem,maxRadio);disp(tmpText);
tmpText=sprintf('最低的是第%d题，通过率为%d',minRadioProblem,minRadio);disp(tmpText);
tmpText=sprintf('通过率的平均值为%d，中间值为%d，标准差为',meanNum,medianNum,stdNum);disp(tmpText);
tmpText=sprintf('题号和通过率的相关系数为%d,题号和提交数的相关系数为%d',orderRadio,orderSubmit);disp(tmpText);

figure('NumberTitle','off','Name','散点图');
scatter(static(:,1),static(:,2),'g','.');%作散点图分析通过率的分布及集聚情况
figure('NumberTitle','off','Name','频数直方图');
hist(static(:,2));   %频数直方图，观察到有中间部分较高
dfittool(static(:,2));%集聚程度拟合
figure('NumberTitle','off','Name','分布的正态性检验');%猜想可能为正态分布
normplot(static(:,2));  %发现接近一条直线，应该是正态分布
x=zscore(static(:,2));%标准化数据
isNormaldistribution=lillietest(x);%对输入向量X进行Lilliefors测试，返回若为0则可以认为X是服从正态分布的;若为1,则可以否定X服从正态分布
if isNormaldistribution==0
    disp('通过率服从正态分布')
    [muhat,sigmahat,muci,sigmaci]=normfit(x);% 假如是正态分布，估计mu和sigma,区间估计，置信度默认95 %
    text=sprintf('mu范围是%d到%d',muci);
    disp(text);
    text=sprintf('sigma范围是%d到%d',sigmaci);
    disp(text);
else
    disp('通过率不服从正态分布');
end