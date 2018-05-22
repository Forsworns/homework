#pragma warning(disable : 4996)
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <string>
using namespace std;

string characterArray[5] = {"age", "prescipt", "astigmatic", "tearRate", "conclusion"};
string CONCLUSION = "conclusion"; //�����޸Ŀ��Խ���������
int depth = 0;
const enum treeType {
	ID3TYPE,
	C45TYPE,
	CART
};

struct node
{
	string character;									//�ֻ������������
	vector<map<string, int>> statics; //��ǰ�ڵ�ʣ�����Ժ�����
	map<int, node *> children;				//�������
	int DFSdepth;											//������ʾ��ӡtab��
	int preAnswer;										//������ʾ��ӡ��֧ѡ��
};

class decisionTree
{
private:
	node root;
	void (*algorithmType)(vector<map<string, int>>, set<string>, node *);

	static inline double H(vector<map<string, int>> glassInfoDivide)
	{
		//��ֵ���㣬�����д洢���ݼ�
		map<int, double> division;
		double answer = 0;
		vector<int> statics;
		for (vector<map<string, int>>::iterator it = glassInfoDivide.begin(); it < glassInfoDivide.end(); ++it)
		{
			//��ȡ��������
			statics.push_back((*it)[CONCLUSION]);
		}
		for (vector<int>::iterator it = statics.begin(); it < statics.end(); ++it)
		{
			//���ݼ�����
			if (division.find(*it) == division.end())
			{
				division.insert(pair<int, double>(*it, 0)); //���ڱ��в�����У���ʵֱ��[]Ҳ�е��ǳ�ʼ����ȫһ��
			}
			division[*it]++;
		}
		for (map<int, double>::iterator it = division.begin(); it != division.end(); ++it)
		{
			double p = (*it).second / statics.size();
			answer += -p * log(p);
		}
		return answer;
	}
	static inline double H(vector<map<string, int>> InfoDivide, string characterSelect)
	{
		//��֪��ǰ��Ϣ�����ֵ
		map<int, vector<int>> cDivision; //��Ϊ����ֵ��ֵΪ����ֵ�µ����ݼ�
		map<int, map<int, double>> sDivision;
		double answer = 0;
		vector<pair<int, int>> charactorAndStatics;

		for (vector<map<string, int>>::iterator it = InfoDivide.begin(); it < InfoDivide.end(); ++it)
		{
			//��ȡ��������Ϣ����-���ݣ��ԡ��ò�����ʡ�ԣ�����ֱ������ȡʱ���ཨ��
			charactorAndStatics.push_back(pair<int, int>((*it)[characterSelect], (*it)[CONCLUSION]));
		}

		for (vector<pair<int, int>>::iterator it = charactorAndStatics.begin(); it < charactorAndStatics.end(); ++it)
		{
			//����ȡ�������ݰ�����ֵ����
			if (cDivision.find((*it).first) == cDivision.end())
			{
				cDivision.insert(pair<int, vector<int>>((*it).first, vector<int>()));
			}
			cDivision[(*it).first].push_back((*it).second);
		}
		for (map<int, vector<int>>::iterator it = cDivision.begin(); it != cDivision.end(); ++it)
		{
			for (vector<int>::iterator it2 = (*it).second.begin(); it2 < (*it).second.end(); ++it2)
			{
				//�ٶԸ�������ֵ�µ����ݼ�����
				if (sDivision[(*it).first].find(*it2) == sDivision[(*it).first].end())
				{
					sDivision[(*it).first].insert(pair<int, int>(*it2, 0)); //(*it).first������ֵ��ÿ������ֵ��Ӧһ����ϣ����ϣ���д��˼�Ϊ��ǰ���������ݵ�����
				}
				sDivision[(*it).first][*it2]++; //������һ����һ����ǰ����characterSelect��ֵ(*it).first��������ֵ�µ�*it2�������͵ļ�����
			}
			//(*it).second.size();
		}

		for (map<int, vector<int>>::iterator it = cDivision.begin(); it != cDivision.end(); ++it)
		{
			for (map<int, double>::iterator it2 = sDivision[(*it).first].begin(); it2 != sDivision[(*it).first].end(); ++it2)
			{
				double p = (*it2).second / (*it).second.size();
				answer += -p * log(p) * (*it).second.size() / charactorAndStatics.size();
			}
			//(*it).second.size()��������ֵ�µ�������
			//charactorAndStatics.size()��������ֵ�µ���������
			//(*it2).secondΪĳ������ֵ��ĳ������ֵ���ֵ���Ŀ
		}
		return answer;
	}

	static inline double SplitInformation(vector<map<string, int>> InfoDivide, string characterSelect)
	{
		//��֪��ǰ��Ϣ���SplitInformation
		map<int, vector<int>> cDivision; //��Ϊ����ֵ��ֵΪ����ֵ�µ����ݼ�
		map<int, map<int, double>> sDivision;
		double answer = 0;
		vector<pair<int, int>> charactorAndStatics;

		for (vector<map<string, int>>::iterator it = InfoDivide.begin(); it < InfoDivide.end(); ++it)
		{
			//��ȡ��������Ϣ����-���ݣ��ԡ��ò�����ʡ�ԣ�����ֱ������ȡʱ���ཨ��
			charactorAndStatics.push_back(pair<int, int>((*it)[characterSelect], (*it)[CONCLUSION]));
		}

		for (vector<pair<int, int>>::iterator it = charactorAndStatics.begin(); it < charactorAndStatics.end(); ++it)
		{
			//����ȡ�������ݰ�����ֵ����
			if (cDivision.find((*it).first) == cDivision.end())
			{
				cDivision.insert(pair<int, vector<int>>((*it).first, vector<int>()));
			}
			cDivision[(*it).first].push_back((*it).second);
		}

		for (map<int, vector<int>>::iterator it = cDivision.begin(); it != cDivision.end(); ++it)
		{
			double p = (*it).second.size();
			answer += -p * log(p) / charactorAndStatics.size();
			//(*it).second.size()��������ֵ�µ�������
			//charactorAndStatics.size()��������ֵ�µ���������
		}
		return answer;
	}

	static void ID3(vector<map<string, int>> InfoDivide, set<string> charactersRemain, node *treeNode)
	{
		treeNode->statics.resize(InfoDivide.size());
		treeNode->statics.assign(InfoDivide.begin(), InfoDivide.end()); //Ϊ�ڵ㸳����
		int item = InfoDivide[0][CONCLUSION];														//��(*(InfoDivide.begin()))[CONCLUSION]
		bool endFlag = false;
		for (vector<map<string, int>>::iterator it = InfoDivide.begin(); it < InfoDivide.end(); ++it)
		{
			if (item != (*it)[CONCLUSION])
			{ //�ж��Ƿ�ǰ�Ѿ��ﵽ����ֵ����ͬ�����
				endFlag = false;
				break;
			}
			else
			{
				endFlag = true;
			}
		}
		if (endFlag)
		{
			return;
		} //��������ж�
		else
		{
			double initialH = 0;
			string characters2erase;
			double minHwithInfo = 1;
			map<int, vector<map<string, int>>> divideInfoDivide;

			initialH = H(InfoDivide); //�����ʼδ֪������Ϣʱ���ݼ�����ֵ
			//cout << initialH << endl;

			for (set<string>::iterator it = charactersRemain.begin(); it != charactersRemain.end(); ++it)
			{
				//ѡ��ǰ��Ϣ��������һ�����ԣ�Ҳ����������ֵ��С��һ��
				double tempH = H(InfoDivide, *it); //���������أ�initialH-tempHΪ��Ϣ����
				if (minHwithInfo > tempH)
				{
					minHwithInfo = tempH;
					characters2erase = *it;
				}
			}
			treeNode->character = characters2erase; //Ϊ�ڵ㸳����

			for (vector<map<string, int>>::iterator it = InfoDivide.begin(); it < InfoDivide.end(); ++it)
			{
				//�ֻ���ǰ����
				if (divideInfoDivide.find((*it)[characters2erase]) == divideInfoDivide.end())
				{
					divideInfoDivide.insert(pair<int, vector<map<string, int>>>((*it)[characters2erase], vector<map<string, int>>()));
					treeNode->children.insert(pair<int, node *>((*it)[characters2erase], new node()));
				}
				divideInfoDivide[(*it)[characters2erase]].push_back(*it);

				for (vector<map<string, int>>::iterator it2 = divideInfoDivide[(*it)[characters2erase]].begin(); it2 < divideInfoDivide[(*it)[characters2erase]].end(); ++it2)
				{
					(*it2).erase(characters2erase);
				}
				//(*it)[characters2erase]Ϊ���ݱ�ÿ�е�ĳ�����Ե�ȡֵ
			}

			charactersRemain.erase(characters2erase); //ȥ���Ѿ��жϹ�������
			//cout << characters2erase << endl;
			depth++;
			for (map<int, vector<map<string, int>>>::iterator it = divideInfoDivide.begin(); it != divideInfoDivide.end(); ++it)
			{ //��ÿһ�����ٴ�ʹ��ID3
				treeNode->children[(*it).first]->DFSdepth = depth;
				treeNode->children[(*it).first]->preAnswer = (*it).first; //������������ӡ��
				ID3((*it).second, charactersRemain, treeNode->children[(*it).first]);
			}
			depth--;
		}
	}

	static void C45(vector<map<string, int>> InfoDivide, set<string> charactersRemain, node *treeNode)
	{
		treeNode->statics.resize(InfoDivide.size());
		treeNode->statics.assign(InfoDivide.begin(), InfoDivide.end()); //Ϊ�ڵ㸳����
		int item = InfoDivide[0][CONCLUSION];
		bool endFlag = false;
		for (vector<map<string, int>>::iterator it = InfoDivide.begin(); it < InfoDivide.end(); ++it)
		{
			if (item != (*it)[CONCLUSION])
			{ //�ж��Ƿ�ǰ�Ѿ��ﵽ����ֵ����ͬ�����
				endFlag = false;
				break;
			}
			else
			{
				endFlag = true;
			}
		}
		if (endFlag)
		{
			return;
		} //��������ж�
		else
		{
			double initialH = 0;
			string characters2erase;
			double minRadioWithInfo = 65536; //ȡΪ�����Ϊĳ�����޷���������ʱSplitInformation�ᵽ0
			map<int, vector<map<string, int>>> divideInfoDivide;

			initialH = H(InfoDivide); //�����ʼδ֪������Ϣʱ���ݼ�����ֵ
			//cout << initialH << endl;

			for (set<string>::iterator it = charactersRemain.begin(); it != charactersRemain.end(); ++it)
			{
				//ѡ��ǰ��Ϣ�����������һ������
				double tempH = H(InfoDivide, *it);
				double ratio;
				double split = SplitInformation(InfoDivide, *it);
				if (split != 0 && tempH / split < minRadioWithInfo)
				{
					ratio = tempH / split;
				}
				else
				{ //���ٱ�֤��characters2erase���ǿ��ַ���
					ratio = 65535;
				}
				if (minRadioWithInfo > ratio)
				{
					minRadioWithInfo = ratio;
					characters2erase = *it;
				}
			}
			treeNode->character = characters2erase; //Ϊ�ڵ㸳����

			for (vector<map<string, int>>::iterator it = InfoDivide.begin(); it < InfoDivide.end(); ++it)
			{
				//�ֻ���ǰ����
				if (divideInfoDivide.find((*it)[characters2erase]) == divideInfoDivide.end())
				{
					divideInfoDivide.insert(pair<int, vector<map<string, int>>>((*it)[characters2erase], vector<map<string, int>>()));
					treeNode->children.insert(pair<int, node *>((*it)[characters2erase], new node()));
				}
				divideInfoDivide[(*it)[characters2erase]].push_back(*it);

				for (vector<map<string, int>>::iterator it2 = divideInfoDivide[(*it)[characters2erase]].begin(); it2 < divideInfoDivide[(*it)[characters2erase]].end(); ++it2)
				{
					(*it2).erase(characters2erase);
				}
				//(*it)[characters2erase]Ϊ���ݱ�ÿ�е�ĳ�����Ե�ȡֵ
			}

			charactersRemain.erase(characters2erase); //ȥ���Ѿ��жϹ�������
																								//cout << characters2erase << endl;
			depth++;
			for (map<int, vector<map<string, int>>>::iterator it = divideInfoDivide.begin(); it != divideInfoDivide.end(); ++it)
			{ //��ÿһ�����ٴ�ʹ��C45
				treeNode->children[(*it).first]->DFSdepth = depth;
				treeNode->children[(*it).first]->preAnswer = (*it).first; //������������ӡ��
				C45((*it).second, charactersRemain, treeNode->children[(*it).first]);
			}
			depth--;
		}
	}

public:
	decisionTree()
	{
		root.character = "";
		root.DFSdepth = 0;
	}
	decisionTree(vector<map<string, int>> InfoDivide, set<string> charactersRemain, treeType type)
	{
		root.character = ""; //Ҷ�ڵ�����Ϊ��
		root.DFSdepth = 0;
		if (type == ID3TYPE)
		{
			algorithmType = &ID3;
		}
		else if (type == C45TYPE)
		{
			algorithmType = &C45;
		}
		algorithmType(InfoDivide, charactersRemain, &root);
	}

	void displayTree(node *child)
	{
		for (int i = 0; i < child->DFSdepth; ++i)
		{
			cout << "\t";
		}
		cout << child->preAnswer << " " << child->character << " " << child->statics.size() << endl;
		for (map<int, node *>::iterator it = child->children.begin(); it != child->children.end(); ++it)
		{
			displayTree((*it).second);
		}
	}
	void displayTree()
	{
		displayTree(&root);
	}

	int predict(map<string, int> info, node *now)
	{
		if (now->children.empty())
		{
			return (*(now->statics.begin()))[CONCLUSION]; //�����ǰ�ڵ��޶�����ôֱ�ӷ��ص�ǰ�ڵ����ݼ��Ľ�������
		}
		else
		{
			int maxChild = 0;
			for (map<int, node *>::iterator it = now->children.begin(); it != now->children.end(); ++it)
			{
				if ((*it).first == info[now->character])
				{
					return predict(info, (*it).second);
				}
				if ((*it).second->statics.size() > now->children[maxChild]->statics.size())
				{
					maxChild = (*it).first;
				}
			}
			//���û��������Է�֧����ȡ����һ���Ӽ��е�Ԫ�ص�����
			return predict(info, now->children[maxChild]);
		}
	}
	int predict(map<string, int> info)
	{
		return predict(info, &root);
	}
};

int main()
{
	freopen("test.txt", "r", stdin);
	set<string> characters(characterArray, characterArray + sizeof(characterArray) / sizeof(string) - 1);
	vector<map<string, int>> glassInfo;
	map<string, int> glasses; //��������ʱ�洢
	int trainNum = 24;				//���ٸ�����ѵ����ʣ����������

	for (int i = 0; i < trainNum; ++i)
	{
		//�����������ݶ�ÿ��������ϣ����vector��ÿ��Ԫ��Ϊһ�е�����
		for (int j = 0; j < sizeof(characterArray) / sizeof(string); ++j)
		{
			cin >> glasses[characterArray[j]];
		}
		glassInfo.push_back(glasses);
	}
	decisionTree newTree(glassInfo, characters, ID3TYPE);
	newTree.displayTree();

	//����Ϊѵ������ΪԤ��

	for (int i = trainNum; i < 24; ++i)
	{
		//�����������ݶ�ÿ��������ϣ����vector��ÿ��Ԫ��Ϊһ�е�����
		for (int j = 0; j < sizeof(characterArray) / sizeof(string); ++j)
		{
			cin >> glasses[characterArray[j]];
		}
		glassInfo.push_back(glasses);
		cout << newTree.predict(glassInfo[i]) << endl;
	}
}