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
const string BLANK = "@";																		 //@����մ�
set<string> V_t = {"$"}; //�ս�������$����������
set<string> V_n = {};																				 //���ս������ʱȷ��

vector<vector<string>> grammer;																	//�ķ�
vector<pair<string, vector<string>>> production;								//����ʽ
map<string, pair<vector<string>, vector<string>>> modifyHelper; //������ݹ�ʱ�Բ���ʽ����
map<string, map<string, set<string>>> factorDivide;							//����������
map<string, set<string>> firstSet;															//first��
map<string, set<string>> followSet;															//follow��
map<string, map<string, pair<string, vector<string>>>> table;		//Ԥ�������

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
	//Ԥ����
	readAndPreprocess();
	//������ݹ�
	if (existDirectRecursion())
	{
		eliminateLeftRecursion();
	}
	//����������
	while (eliminateLeftFactor())
	{
		continue;
	}
	//������first���Ϻ�follow����
	calculateFirst();
	calculateFollow();
	//���湹���
	buildTable();
	//��ӡ���
	print();

	return 0;
}

void splitString(const std::string &s, std::vector<std::string> &v, const std::string &c)
{
	//���ߺ�������ĳ���ַ�������string������vector��
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
	//�����Կո��ʾ��ͬ��Ԫ
	string buffer;	 //���뻺��
	int counter = 0; //�ķ�����
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
		grammer[counter++].erase(it); //ϴȥ��ͷ->
	}
	grammer.pop_back(); //���һ�����Ƕ����һ��

	//ȷ�����ս��������pair�Ա�ʾ����ʽ
	for (vector<vector<string>>::iterator itptr = grammer.begin(); itptr < grammer.end(); ++itptr)
	{
		vector<string>::iterator it = itptr->begin(); //����ʽ�����һ����һ�����ս��
		if (!V_n.count(*it) && *it != BLANK)
		{ //��������ַ��Ƿ��ڷ��ս��������
			V_n.insert(*it);
		}
		//��ʼ�������ʽ�Ҳ�
		vector<string>::iterator start = itptr->begin() + 1;
		vector<string>::iterator end = itptr->begin() + 1;
		for (it = itptr->begin() + 1; it < itptr->end(); ++it)
		{ //��ʼ����Ҳ��ķ�����
			if (*it != OR)
			{
				end++;
				if (!V_n.count(*it) && !V_t.count(*it) && *it != BLANK)
				{ //���Ƿ��ս�����Ҳ��Ƿָ������ҷǿմ�����ô���ս��
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
		//��OR������ָ���ں�����first/follow���Ϻ����
		production.emplace_back(pair<string, vector<string>>(*itptr->begin(), vector<string>(start, end)));
	}
}

bool eliminateLeftFactor()
{
	bool flag = false;
	factorDivide.clear();
	for (vector<pair<string, vector<string>>>::iterator vecIt = production.begin(); vecIt < production.end(); ++vecIt)
	{
		//��ȡһ����ͬ�ַ����ظ������ȡ���
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
		//��ÿ�����ϵ�����
		int counter = 0;
		for (map<string, set<string>>::iterator it = mapIt->second.begin(); it != mapIt->second.end(); ++it)
		{
			if (it->second.size() > 1)
			{
				flag = true;
				counter++;
				V_n.insert(mapIt->first + to_string(counter));
				modifyHelper[mapIt->first].first.push_back(it->first + " " + mapIt->first + to_string(counter)); //��һ�� A -> p A_i
				for (set<string>::iterator setIt = it->second.begin(); setIt != it->second.end(); ++setIt)
				{ //������ A_i -> q_i
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
		//�ָ��ɲ���ʽ
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
	{ //�ս����first��ֻ���������ϵ�һ�����
		return set<string>(&input, &input + 1);
	}
	else if (V_n.count(input))
	{
		set<string> temp;
		for (vector<pair<string, vector<string>>>::iterator it = production.begin(); it < production.end(); ++it)
		{
			if ((*it).first == input)
			{
				bool flag = true; //����Ƿ�ǰһ���ս�������մ�����ʼӦ����true
				for (vector<string>::iterator vecIt = it->second.begin(); vecIt < it->second.end(); ++vecIt)
				{
					if ((*vecIt) == BLANK)
					{
						temp.insert(BLANK); //����ʽ���пմ����ѿմ����뵽first���У����ϵ��������
					}
					else
					{
						if (flag)
						{ //���ǰһ�����ս����first���к��пմ����÷��ŵ�first��ҲӦ�ü��뵽����first���У����ϵڶ������
							set<string> thisFirst;
							if (firstSet.count(*vecIt))
							{ //�Ѿ����˱�Ĳ��ٴ�����
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
		throw exception("������ȡ�������First��");
	}
}

void calculateFirst()
{
	//������һ����Ч�����⣬��һ���ֵݹ���������������Ѿ�����ˣ���֮��ȴ��Ȼ�ݹ����
	for (set<string>::iterator it = V_n.begin(); it != V_n.end(); ++it)
	{
		firstSet.insert(pair<string, set<string>>(*it, calculateFirst(*it)));
	}
	for (set<string>::iterator it = V_t.begin(); it != V_t.end(); ++it)
	{
		firstSet.insert(pair<string, set<string>>(*it, calculateFirst(*it)));
	}
	firstSet.insert(pair<string, set<string>>(BLANK, set<string>(&BLANK, &BLANK + 1))); //����Ϊ�˷����̣�������������ս���Ϳմ��ġ�first����
}

set<string> calculateFollow(const string &input)
{
	set<string> temp;
	for (vector<pair<string, vector<string>>>::iterator it = production.begin(); it < production.end(); ++it)
	{
		bool flag = false; //����Ƿ����Ҳ��ҵ���input
		for (vector<string>::iterator vecIt = it->second.begin(); vecIt < it->second.end(); ++vecIt)
		{

			if (flag)
			{ //����Ѿ��ҵ���input����ô�󷽵��ķ����ž�Ӧ��������first������Ӧ���ϵڶ������
				set<string> thisFirst = firstSet[*vecIt];
				for (set<string>::iterator setIt = thisFirst.begin(); setIt != thisFirst.end(); ++setIt)
				{
					temp.insert(*setIt);
				}
				if (thisFirst.count(BLANK))
				{ //���input����ķ����ŵ�first�������մ�����ôӦ�ü��������󣬵���Ҫȥ���ոռ��뵽follow���еĿմ�
					temp.erase(temp.find(BLANK));
				}
				else
				{
					flag = false; //���input���ķ����ŵ�first�������մ�����ô�൱�ڵ�ǰ�ҵ���input��follow��������Ѿ�������
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
		//ĩβ�ķ��ŵ�follow�����룬��Ӧ���ϵ��������
		if (it->first == input)
		{ //�ҵݹ�������Ӧ��������������������ĵݹ�!!!!!���￨�˰���ŷ���
			continue;
		}
		bool flag = true; //���ĩβ�Ƿ��ǿմ�
		for (vector<string>::reverse_iterator vecIt = it->second.rbegin(); vecIt < it->second.rend(); ++vecIt)
		{
			if (flag)
			{
				if (*vecIt == input)
				{
					set<string> thisFollow;
					if (followSet.count(it->first))
					{ //�����Ƿ��Ѿ����˱�������˾Ͳ��ݹ������
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
	{ //��������ʽ
		for (vector<string>::iterator vecIt = it->second.begin(); vecIt < it->second.end(); ++vecIt)
		{ //��һ����first���ս�����
			for (set<string>::iterator setIt = firstSet[*vecIt].begin(); setIt != firstSet[*vecIt].end(); ++setIt)
			{ //�Բ���ʽ�Ҳ���first��
				if (*setIt != BLANK)
				{
					table[it->first].insert(pair<string, pair<string, vector<string>>>(*setIt, *it));
				}
			}
			if (!firstSet[*vecIt].count(BLANK))
			{ //�Ƴ��մ��Ĳ���ʽҲ�ؾ����˴����Կմ���first��Ҫ���塭�������Ǹ��Ӹ��˰���
				break;
			}
			else
			{ //�ڶ�����first���մ��ģ���follow�����
				for (set<string>::iterator setIt = followSet[it->first].begin(); setIt != followSet[it->first].end(); ++setIt)
				{
					table[it->first].insert(pair<string, pair<string, vector<string>>>(*setIt, *it));
				} //map�Ѿ�ȷ�����Ѿ����뵽��������������ٴβ������
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