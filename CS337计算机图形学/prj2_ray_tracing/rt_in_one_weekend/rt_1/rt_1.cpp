#include "pch.h"
#include <iostream>
#pragma warning (disable:4996)
using namespace std;
int main()
{
	freopen("test.ppm", "w", stdout);
	int nx = 200;
	int ny = 200;
	cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int i = 0; i < nx; ++i) {
		for (int j = 0; j < ny; ++j) {
			float r = float(i)/float(nx);
			float g = float(j)/float(ny);
			float b = 0.2;
			int ir = int(r*255.99);
			int ig = int(g*255.99);
			int ib = int(b*255.99);
			cout << ir << " " << ig << " " << ib << endl;
		}
	}
}

