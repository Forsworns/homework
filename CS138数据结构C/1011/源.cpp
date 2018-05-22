#include <iostream>
#include <iomanip>
using namespace std;

class MyComplex
{
private:
	double x, y;
public:
	MyComplex() :x(0), y(0) {}
	MyComplex operator+(const MyComplex another) {
		MyComplex tmp;
		tmp.x = x + another.x;
		tmp.y = y + another.y;
		return tmp;
	}
	MyComplex operator-(const MyComplex another) {
		MyComplex tmp;
		tmp.x = x - another.x;
		tmp.y = y - another.y;
		return tmp;
	}
	MyComplex operator*(const MyComplex another) {
		MyComplex tmp;
		tmp.x = x * another.x - y * another.y;
		tmp.y = y * another.x + x * another.y;
		return tmp;
	}
	MyComplex operator/(const MyComplex another) {
		MyComplex tmp;
		tmp.x = (x * another.x + y * another.y) / (another.x*another.x + another.y*another.y);
		tmp.y = (y * another.x - x * another.y) / (another.x*another.x + another.y*another.y);
		return tmp;
	}
	MyComplex& operator+=(const MyComplex another) { //与上面的区别是改变了左值
		x += another.x;
		y += another.y;
		return *this;
	}
	MyComplex& operator-=(const MyComplex another) {
		x -= another.x;
		y -= another.y;
		return *this;
	}
	MyComplex& operator*=(const MyComplex another) {
		MyComplex tmp;
		tmp.x = x;
		tmp.y = y;
		x = tmp.x * another.x - tmp.y * another.y;
		y = tmp.y * another.x + tmp.x * another.y;
		return *this;
	}
	MyComplex& operator/=(const MyComplex another) {
		MyComplex tmp;
		tmp.x = x;
		tmp.y = y;
		x = (tmp.x * another.x + tmp.y * another.y) / (another.x*another.x + another.y*another.y);
		y = (tmp.y * another.x - tmp.x * another.y) / (another.x*another.x + another.y*another.y);
		return *this;
	}
	friend ostream& operator<<(ostream& os, const MyComplex &obj) {
		os <<fixed<<setprecision(2)<< obj.x <<' '<< obj.y;
		return os;
	}
	friend istream& operator>>(istream& is,MyComplex &obj) {
		is >> obj.x >> obj.y;
		return is;
	}
};

int main()
{
	MyComplex z1;
	MyComplex z2;

	cin >> z1 >> z2;

	cout << z1 + z2 << endl;
	cout << z1 - z2 << endl;
	cout << z1 * z2 << endl;
	cout << z1 / z2 << endl;
	cout << (z1 += z2) << endl;
	cout << (z1 -= z2) << endl;
	cout << (z1 *= z2) << endl;
	cout << (z1 /= z2) << endl;

	return 0;
}