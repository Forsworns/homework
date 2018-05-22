#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <map>
#pragma warning(disable : 4996)
using namespace std;

const string ARROW = "->";
const string OR = "|";
const string BLANK = "@";																		 //@代表空串
set<string> V_t = {"$"}; //终结符这里把$放入了其中
set<string> V_n = {};																				 //非终结符运行时确定

vector<vector<string>> grammer;																	//文法
vector<pair<string, vector<string>>> production;								//产生式
map<string, pair<vector<string>, vector<string>>> modifyHelper; //消除左递归时对产生式分类
map<string, map<string, set<string>>> factorDivide;							//消除左因子
map<string, set<string>> firstSet;															//first集
map<string, set<string>> followSet;															//follow集
map<string, map<string, pair<string, vector<string>>>> table;		//预测分析表

void splitString(const std::string &s, std::vector<std::string> &v, const std::string &c);
void readAndPreprocess();
bool existDirectRecursion();
void eliminateLeftRecursion();
bool eliminateLeftFactor();
set<string> calculateFirst(const string &input);
void calculateFirst();
set<string> calculateFollow(const string &input);
void calculateFollow();
void buildTable();
void print();

int main()
{
	//freopen("withoutRecursion.txt", "r", stdin);
	//freopen("output1.txt","w",stdout);
	freopen("leftRecursion.txt", "r", stdin);
	//freopen("output2.txt", "w", stdout);
	//预处理
	readAndPreprocess();
	//消除左递归
	if (existDirectRecursion())
	{
		eliminateLeftRecursion();
	}
	//消除左因子
	while (eliminateLeftFactor())
	{
		continue;
	}
	//下面求first集合和follow集合
	calculateFirst();
	calculateFollow();
	//下面构造表
	buildTable();
	//打印输出
	print();

	return 0;
}

void splitString(const std::string &s, std::vector<std::string> &v, const std::string &c)
{
	//工具函数，按某个字符串划分string，存入vector中
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (pos2 != string::npos)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void readAndPreprocess()
{
	//输入以空格表示不同单元
	string buffer;	 //输入缓冲
	int counter = 0; //文法行数
	while (cin)
	{
		getline(cin, buffer);
		grammer.push_back(vector<string>());
		splitString(buffer, grammer[counter], " ");
		vector<string>::iterator it;
		for (it = grammer[counter].begin(); it < grammer[counter].end(); ++it)
		{
			if ((*it) == ARROW)
				break;
		}
		grammer[counter++].erase(it); //洗去箭头->
	}
	grammer.pop_back(); //最后一行总是多读入一次

	//确定非终结符，建立pair对表示产生式
	for (vector<vector<string>>::iterator itptr = grammer.begin(); itptr < grammer.end(); ++itptr)
	{
		vector<string>::iterator it = itptr->begin(); //产生式的左侧一定是一个非终结符
		if (!V_n.count(*it) && *it != BLANK)
		{ //检查左侧的字符是否在非终结符集合中
			V_n.insert(*it);
		}
		//开始处理产生式右侧
		vector<string>::iterator start = itptr->begin() + 1;
		vector<string>::iterator end = itptr->begin() + 1;
		for (it = itptr->begin() + 1; it < itptr->end(); ++it)
		{ //开始检查右侧文法符号
			if (*it != OR)
			{
				end++;
				if (!V_n.count(*it) && !V_t.count(*it) && *it != BLANK)
				{ //不是非终结符并且不是分隔符，且非空串，那么是终结符
					V_t.insert(*it);
				}
			}
			else
			{
				production.emplace_back(pair<string, vector<string>>(*itptr->begin(), vector<string>(start, end)));
				start = end + 1;
				end++;
			}
		}
		//将OR的情况分割开便于后续求first/follow集合和填表
		production.emplace_back(pair<string, vector<string>>(*itptr->begin(), vector<string>(start, end)));
	}
}

bool eliminateLeftFactor()
{
	bool flag = false;
	factorDivide.clear();
	for (vector<pair<string, vector<string>>>::iterator vecIt = production.begin(); vecIt < production.end(); ++vecIt)
	{
		//提取一层相同字符，重复多次提取多层
		string temp;
		for (vector<string>::iterator it = vecIt->second.begin() + 1; it < vecIt->second.end(); ++it)
		{
			temp += *it;
			temp += " ";
		}
		if (temp.size())
		{
			temp.erase(temp.end() - 1);
		}
		factorDivide[vecIt->first].insert(pair<string, set<string>>(*(vecIt->second.begin()), set<string>()));
		factorDivide[vecIt->first][*(vecIt->second.begin())].insert(temp);
	}
	modifyHelper.clear();
	for (map<string, map<string, set<string>>>::iterator mapIt = factorDivide.begin(); mapIt != factorDivide.end(); ++mapIt)
	{
		//对每个集合单另处理
		int counter = 0;
		for (map<string, set<string>>::iterator it = mapIt->second.begin(); it != mapIt->second.end(); ++it)
		{
			if (it->second.size() > 1)
			{
				flag = true;
				counter++;
				V_n.insert(mapIt->first + to_string(counter));
				modifyHelper[mapIt->first].first.push_back(it->first + " " + mapIt->first + to_string(counter)); //加一个 A -> p A_i
				for (set<string>::iterator setIt = it->second.begin(); setIt != it->second.end(); ++setIt)
				{ //补充多个 A_i -> q_i
					modifyHelper[mapIt->first + to_string(counter)].first.push_back(*setIt);
				}
			}
			else
			{
				modifyHelper[mapIt->first].first.push_back(it->first + " " + *it->second.begin());
			}
		}
	}
	production.clear();
	for (map<string, pair<vector<string>, vector<string>>>::iterator it = modifyHelper.begin(); it != modifyHelper.end(); ++it)
	{
		//恢复成产生式
		for (vector<string>::iterator vecIt = it->second.first.begin(); vecIt < it->second.first.end(); ++vecIt)
		{
			vector<string> helperVector;
			splitString(*vecIt, helperVector, " ");
			production.push_back(pair<string, vector<string>>(it->first, helperVector));
		}
	}
	return flag;
}

bool existDirectRecursion()
{
	for (vector<pair<string, vector<string>>>::iterator vecIt = production.begin(); vecIt < production.end(); ++vecIt)
	{
		if (vecIt->first == *(vecIt->second.begin()))
		{
			return true;
		}
	}
	return false;
}

void eliminateLeftRecursion()
{
	for (vector<pair<string, vector<string>>>::iterator vecIt = production.begin(); vecIt < production.end(); ++vecIt)
	{
		if (vecIt->first == *(vecIt->second.begin()))
		{
			string temp;
			for (vector<string>::iterator it = vecIt->second.begin() + 1; it < vecIt->second.end(); ++it)
			{
				temp += *it;
				temp += " ";
			}
			temp += vecIt->first + "'";
			modifyHelper[vecIt->first].first.push_back(temp);
		}
		else
		{
			string temp;
			for (vector<string>::iterator it = vecIt->second.begin(); it < vecIt->second.end(); ++it)
			{
				temp += *it;
				temp += " ";
			}
			temp += vecIt->first + "'";
			modifyHelper[vecIt->first].second.push_back(temp);
		}
	}
	for (set<string>::iterator setIt = V_n.begin(); setIt != V_n.end(); ++setIt)
	{
		if (modifyHelper[*setIt].first.size() > 0)
		{
			modifyHelper[*setIt].first.push_back(BLANK);
		}
	}
	production.clear();
	for (map<string, pair<vector<string>, vector<string>>>::iterator it = modifyHelper.begin(); it != modifyHelper.end(); ++it)
	{
		for (vector<string>::iterator vecIt = it->second.first.begin(); vecIt < it->second.first.end(); ++vecIt)
		{
			vector<string> helperVector;
			splitString(*vecIt, helperVector, " ");
			production.push_back(pair<string, vector<string>>(it->first + "'", helperVector));
			V_n.insert(it->first + "'");
		}
		for (vector<string>::iterator vecIt = it->second.second.begin(); vecIt < it->second.second.end(); ++vecIt)
		{
			vector<string> helperVector;
			splitString(*vecIt, helperVector, " ");
			production.push_back(pair<string, vector<string>>(it->first, helperVector));
		}
	}
}

set<string> calculateFirst(const string &input)
{
	if (V_t.count(input))
	{ //终结符的first集只有自身，书上第一种情况
		return set<string>(&input, &input + 1);
	}
	else if (V_n.count(input))
	{
		set<string> temp;
		for (vector<pair<string, vector<string>>>::iterator it = production.begin(); it < production.end(); ++it)
		{
			if ((*it).first == input)
			{
				bool flag = true; //标记是否前一非终结符包含空串，初始应该是true
				for (vector<string>::iterator vecIt = it->second.begin(); vecIt < it->second.end(); ++vecIt)
				{
					if ((*vecIt) == BLANK)
					{
						temp.insert(BLANK); //产生式中有空串，把空串加入到first集中，书上第三种情况
					}
					else
					{
						if (flag)
						{ //如果前一个非终结符的first集中含有空串，该符号的first集也应该加入到左侧的first集中，书上第二种情况
							set<string> thisFirst;
							if (firstSet.count(*vecIt))
							{ //已经填了表的不再次求了
								thisFirst = firstSet[*vecIt];
							}
							else
							{
								thisFirst = calculateFirst(*vecIt);
							}
							for (set<string>::iterator setIt = thisFirst.begin(); setIt != thisFirst.end(); ++setIt)
							{
								temp.insert(*setIt);
							}
							if (thisFirst.count(BLANK))
							{
								flag = true;
							}
							else
							{
								flag = false;
							}
						}
						else
						{
							continue;
						}
					}
				}
			}
		}
		return temp;
	}
	else
	{
		throw exception("不能求取该输入的First集");
	}
}

void calculateFirst()
{
	//这里有一定的效率问题，有一部分递归求解的子问题可能已经解决了，在之后却依然递归求解
	for (set<string>::iterator it = V_n.begin(); it != V_n.end(); ++it)
	{
		firstSet.insert(pair<string, set<string>>(*it, calculateFirst(*it)));
	}
	for (set<string>::iterator it = V_t.begin(); it != V_t.end(); ++it)
	{
		firstSet.insert(pair<string, set<string>>(*it, calculateFirst(*it)));
	}
	firstSet.insert(pair<string, set<string>>(BLANK, set<string>(&BLANK, &BLANK + 1))); //仅仅为了方便编程，在这里加上了终结符和空串的“first集”
}

set<string> calculateFollow(const string &input)
{
	set<string> temp;
	for (vector<pair<string, vector<string>>>::iterator it = production.begin(); it < production.end(); ++it)
	{
		bool flag = false; //标记是否在右侧找到了input
		for (vector<string>::iterator vecIt = it->second.begin(); vecIt < it->second.end(); ++vecIt)
		{

			if (flag)
			{ //如果已经找到了input，那么后方的文法符号就应该依次求first集，对应书上第二种情况
				set<string> thisFirst = firstSet[*vecIt];
				for (set<string>::iterator setIt = thisFirst.begin(); setIt != thisFirst.end(); ++setIt)
				{
					temp.insert(*setIt);
				}
				if (thisFirst.count(BLANK))
				{ //如果input后的文法符号的first集包含空串，那么应该继续往后求，但是要去除刚刚加入到follow集中的空串
					temp.erase(temp.find(BLANK));
				}
				else
				{
					flag = false; //如果input后文法符号的first集不含空串，那么相当于当前找到的input的follow集的求解已经结束了
				}
			}
			else
			{
				if (*vecIt == input)
				{
					flag = true;
				}
				else
				{
					continue;
				}
			}
		}
	}

	for (vector<pair<string, vector<string>>>::iterator it = production.begin(); it < production.end(); ++it)
	{
		//末尾的符号的follow集并入，对应书上第三种情况
		if (it->first == input)
		{ //右递归的情况，应该跳过，否则会出现无穷的递归!!!!!这里卡了半天才发现
			continue;
		}
		bool flag = true; //标记末尾是否是空串
		for (vector<string>::reverse_iterator vecIt = it->second.rbegin(); vecIt < it->second.rend(); ++vecIt)
		{
			if (flag)
			{
				if (*vecIt == input)
				{
					set<string> thisFollow;
					if (followSet.count(it->first))
					{ //检验是否已经填了表，如果填了就不递归求解了
						thisFollow = followSet[it->first];
					}
					else
					{
						thisFollow = calculateFollow(it->first);
					}
					for (set<string>::iterator setIt = thisFollow.begin(); setIt != thisFollow.end(); ++setIt)
					{
						temp.insert(*setIt);
					}
				}
				else
				{
					if (*vecIt != BLANK && firstSet[*vecIt].count(BLANK))
					{
						flag = true;
					}
					else
					{
						flag = false;
					}
				}
			}
		}
	}
	return temp;
}

void calculateFollow()
{
	for (set<string>::iterator it = V_n.begin(); it != V_n.end(); ++it)
	{
		followSet.insert(pair<string, set<string>>(*it, calculateFollow(*it)));
		if (it == V_n.begin())
		{
			followSet[*it].insert("$");
		}
	}
}

void buildTable()
{
	for (vector<pair<string, vector<string>>>::iterator it = production.begin(); it < production.end(); ++it)
	{ //遍历产生式
		for (vector<string>::iterator vecIt = it->second.begin(); vecIt < it->second.end(); ++vecIt)
		{ //第一条：first集终结加入表
			for (set<string>::iterator setIt = firstSet[*vecIt].begin(); setIt != firstSet[*vecIt].end(); ++setIt)
			{ //对产生式右侧求first集
				if (*setIt != BLANK)
				{
					table[it->first].insert(pair<string, pair<string, vector<string>>>(*setIt, *it));
				}
			}
			if (!firstSet[*vecIt].count(BLANK))
			{ //推出空串的产生式也必经过此处所以空串的first集要定义……这里是个坑改了半天
				break;
			}
			else
			{ //第二条：first含空串的，把follow加入表
				for (set<string>::iterator setIt = followSet[it->first].begin(); setIt != followSet[it->first].end(); ++setIt)
				{
					table[it->first].insert(pair<string, pair<string, vector<string>>>(*setIt, *it));
				} //map已经确保了已经插入到表格里的情况不会再次插入更新
			}
		}
	}
}

void print()
{
	for (auto row = table.begin(); row != table.end(); ++row)
	{
		for (auto col = row->second.begin(); col != row->second.end(); ++col)
		{
			cout << row->first << " " << col->first << "   " << col->second.first << "->";
			for (auto item = col->second.second.begin(); item < col->second.second.end(); ++item)
			{
				cout << *item;
			}
			cout << endl;
		}
	}
}