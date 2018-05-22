## 编译原理大作业

### 预测分析表构造

源.cpp包含了代码的所有部分，完成了作业的全部要求，可以自行识别是否包含直接左递归并将之消除，在VS2015中完成，运用了STL标准模板库。

#### 变量声明

测试时在修改输入文法时，使用了@来代表空串epsilon。


#### 测试用例
用@表示空串epsilon，用课本上的标准，使用->和|描述产生式，我测试了两组文法，分别为包含直接左递归和不包含直接左递归的文法。输入输出分别为（另可见于文件leftRecursion.txt、withoutRecursion.txt、output1.txt、output2.txt)。
```shell
#input
E -> T E'
E' -> + T E' | @
T -> F T'
T' -> * F T' | @
F -> ( E ) | id

#output
E (   E->TE'
E id   E->TE'
E' $   E'->@
E' )   E'->@
E' +   E'->+TE'
F (   F->(E)
F id   F->id
T (   T->FT'
T id   T->FT'
T' $   T'->@
T' )   T'->@
T' *   T'->*FT'
T' +   T'->@
```
```shell
#input
E -> E + T | E - T | T 
T -> T * F | T / F | F
F -> ( E ) | id

#output
E (   E->TE'
E id   E->TE'
E' $   E'->@
E' )   E'->@
E' +   E'->+TE'
E' -   E'->-TE'
F (   F->(E)F'
F id   F->idF'
T (   T->FT'
T id   T->FT'
T' $   T'->@
T' )   T'->@
T' *   T'->*FT'
T' +   T'->@
T' -   T'->@
T' /   T'->/FT'
```
