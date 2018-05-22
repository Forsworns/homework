#include <iostream>  
#include <string>  
using namespace std;
//利用归并排序的方法统计逆序对  
//将有二个有序数列a[first...mid]和a[mid...last]合并。    
long long int mergearray(int a[], int first, int mid, int last, int temp[])
{
	long long int count = 0;//统计逆序数  
	int i = first;  //前一组的开始  
	int n = mid;  //前一组的结束  
	int j = mid + 1;  //后一组的开始  
	int m = last;  //后一组的结束  
	int k = last; //临时数组从后面开始放入数据  
	while (i <= n&&j <= m)
	{
		//从最后面比较二个数列，大的先放入临时数组中。  
		if (a[n] <= a[m]) //相等时不认为是逆序
		{
			temp[k--] = a[m--];
		}
		else
		{
			temp[k--] = a[n--];
			count = count + m - j + 1;//如果前面数组的数大于后一个数组的数的话，那么后一个数组前面的数都是前面一个数组的数的逆序数。  
		}
	}
	//有数列为空，那直接将另一个数列的数据从末尾依次取出即可    
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
		long long int left = mergesort(a, first, mid, temp);    //左边的逆序数目
		/*cout << "左" << left << endl;;
		for (int i = first; i <= mid;++i) {
			cout << a[i]<<" ";
		}
		cout << endl;*/
		long long int right = mergesort(a, mid + 1, last, temp); //右边的逆序数目
		/*cout << "右" << right << endl;;
		for (int i = mid+1; i <= last; ++i) {
			cout << a[i] << " ";
		}
		cout << endl;*/
		long long int all = mergearray(a, first, mid, last, temp); //两者之间的逆序数目      
		/*cout << "全部" << all << endl;;
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
	long long int res = mergesort(p, 0, N - 1, q); //注意范围last是要记入的，所以不能用N要用N-1
	cout << res;
	for (int i = 0; i < N; ++i) {
		cout << p[i];
	}
	return 0;
}