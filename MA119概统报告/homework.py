#! python2
#coding:utf-8
import urllib2
import re
from bs4 import BeautifulSoup

def isDate(str): #筛除日期
    pattern = re.compile("\d\d\d\d-")
    match = pattern.findall(str)
    if match:
        return True
    else:
        return False

def isFourSizeOrder(str): #找到每页的第一个四位题号
    pattern = re.compile("\d\d\d\d")
    match = pattern.findall(str)
    if match:
        return True
    else:
        return False

file = open("statics.txt","w")
for num in range(1,32):
    flag = False
    response = urllib2.urlopen("http://poj.org/problemlist?volume=%d"%num)
    html = response.read()
    soup = BeautifulSoup(html,"lxml")
    for tds in soup.find_all('td'):
        if tds.get("align")!="left":
            theString = str(tds.string) #转换td内的字符串格式
            if isFourSizeOrder(theString): #找到第一个序号之后才开始写入数据
                flag = True
            if flag and not isDate(theString):
                if theString=="None":
                    ratio = re.findall(r">(.+?)%",str(tds)) #把tds转换成字符串然后截取通过比率
                    print(ratio)
                    file.write(ratio[0]+" ")
                    for a in tds.find_all('a'): #AC 和 submit数量
                        file.write(str(a.string)+" ")
                        print(a.string)
                    file.write("\n")
                else:
                    file.write(theString+" ")
                    print(tds.string)
    print("page %d succeed!"%num)
