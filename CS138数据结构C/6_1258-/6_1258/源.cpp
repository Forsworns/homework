#include <iostream>  
#include <string>  
using namespace std;
//���ù鲢����ķ���ͳ�������  
//���ж�����������a[first...mid]��a[mid...last]�ϲ���    
long long int mergearray(int a[], int first, int mid, int last, int temp[])
{
	long long int count = 0;//ͳ��������  
	int i = first;  //ǰһ��Ŀ�ʼ  
	int n = mid;  //ǰһ��Ľ���  
	int j = mid + 1;  //��һ��Ŀ�ʼ  
	int m = last;  //��һ��Ľ���  
	int k = last; //��ʱ����Ӻ��濪ʼ��������  
	while (i <= n&&j <= m)
	{
		//�������Ƚ϶������У�����ȷ�����ʱ�����С�  
		if (a[n] <= a[m]) //���ʱ����Ϊ������
		{
			temp[k--] = a[m--];
		}
		else
		{
			temp[k--] = a[n--];
			count = count + m - j + 1;//���ǰ������������ں�һ����������Ļ�����ô��һ������ǰ���������ǰ��һ�������������������  
		}
	}
	//������Ϊ�գ���ֱ�ӽ���һ�����е����ݴ�ĩβ����ȡ������    
	while (i <= n)
	{
		temp[k--] = a[n--];
	}
	while (j <= m)
	{
		temp[k--] = a[m--];
	}
    
	for (i = first; i <= last; i++)
	{
		a[i] = temp[i];
	}
	return count;
}
long long int  mergesort(int a[], int first, int last, int temp[])
{

	if (first == last)
	{
		return 0;
	}
	else
	{
		int mid = (first + last) / 2;
		long long int left = mergesort(a, first, mid, temp);    //��ߵ�������Ŀ
		/*cout << "��" << left << endl;;
		for (int i = first; i <= mid;++i) {
			cout << a[i]<<" ";
		}
		cout << endl;*/
		long long int right = mergesort(a, mid + 1, last, temp); //�ұߵ�������Ŀ
		/*cout << "��" << right << endl;;
		for (int i = mid+1; i <= last; ++i) {
			cout << a[i] << " ";
		}
		cout << endl;*/
		long long int all = mergearray(a, first, mid, last, temp); //����֮���������Ŀ      
		/*cout << "ȫ��" << all << endl;;
		for (int i = first; i <= last; ++i) {
			cout << a[i] << " ";
		}
		cout << endl;*/
		return  (left + right + all);
	}

}
int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	int p[100000];
	int q[100000];
	int N;
	cin >> N;
	for (int i = 0; i < N; ++i) {
		cin>>p[i];
	}
	long long int res = mergesort(p, 0, N - 1, q); //ע�ⷶΧlast��Ҫ����ģ����Բ�����NҪ��N-1
	cout << res;
	for (int i = 0; i < N; ++i) {
		cout << p[i];
	}
	return 0;
}