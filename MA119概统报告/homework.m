static = textread('E:\���\����ͳ�ƴ���ҵ\statics.txt');%[��ţ�ͨ���ʣ�AC��,�ύ��]
for i=(1:3034)  %�ѱ���ת���ɾ�ȷ��ֵ
    static(i,2)=static(i,3)/static(i,4);
end

%��ֵ�����������
[maxNumMatrix,maxNumPosMatrix]=max(static);%��ȡ���ֵ���ص���ÿ�е����ֵ�����
maxRadio=maxNumMatrix(1,2);
maxRadioProblem=static(maxNumPosMatrix(1,2));%maxNumPos��ĵڶ���������static�еڶ������ֵ��λ�ã���Ϊͨ������ߵ���Ŀ���
[minNumMatrix,minNumPosMatrix]=min(static);
minRadio=minNumMatrix(1,2);
minRadioProblem=static(minNumPosMatrix(1,2));%ͨ������͵���Ŀ���

meanMatrix=mean(static);%ƽ��ֵ
meanNum=meanMatrix(1,2);
medianMatrix=median(static);%�м�ֵ
medianNum=medianMatrix(1,2);
stdMatrix=std(static);%��׼��
stdNum=std(1,2);
corrcoefMatrix=corrcoef(static);%���ϵ����ÿһ�е�ǰ����ÿһ�е����ϵ����(1,2)��(2,1)Ϊ��ź�ͨ���ʵĹ�ϵ��(1,4)��(4,1)Ϊ������ύ���Ĺ�ϵ
orderRadio=corrcoefMatrix(1,2);
orderSubmit=corrcoefMatrix(1,4);
tmpText=sprintf('ͨ������ߵ�Ϊ��%d��,ͨ������%d',maxRadioProblem,maxRadio);disp(tmpText);
tmpText=sprintf('��͵��ǵ�%d�⣬ͨ����Ϊ%d',minRadioProblem,minRadio);disp(tmpText);
tmpText=sprintf('ͨ���ʵ�ƽ��ֵΪ%d���м�ֵΪ%d����׼��Ϊ',meanNum,medianNum,stdNum);disp(tmpText);
tmpText=sprintf('��ź�ͨ���ʵ����ϵ��Ϊ%d,��ź��ύ�������ϵ��Ϊ%d',orderRadio,orderSubmit);disp(tmpText);

figure('NumberTitle','off','Name','ɢ��ͼ');
scatter(static(:,1),static(:,2),'g','.');%��ɢ��ͼ����ͨ���ʵķֲ����������
figure('NumberTitle','off','Name','Ƶ��ֱ��ͼ');
hist(static(:,2));   %Ƶ��ֱ��ͼ���۲쵽���м䲿�ֽϸ�
dfittool(static(:,2));%���۳̶����
figure('NumberTitle','off','Name','�ֲ�����̬�Լ���');%�������Ϊ��̬�ֲ�
normplot(static(:,2));  %���ֽӽ�һ��ֱ�ߣ�Ӧ������̬�ֲ�
x=zscore(static(:,2));%��׼������
isNormaldistribution=lillietest(x);%����������X����Lilliefors���ԣ�������Ϊ0�������ΪX�Ƿ�����̬�ֲ���;��Ϊ1,����Է�X������̬�ֲ�
if isNormaldistribution==0
    disp('ͨ���ʷ�����̬�ֲ�')
    [muhat,sigmahat,muci,sigmaci]=normfit(x);% ��������̬�ֲ�������mu��sigma,������ƣ����Ŷ�Ĭ��95 %
    text=sprintf('mu��Χ��%d��%d',muci);
    disp(text);
    text=sprintf('sigma��Χ��%d��%d',sigmaci);
    disp(text);
else
    disp('ͨ���ʲ�������̬�ֲ�');
end