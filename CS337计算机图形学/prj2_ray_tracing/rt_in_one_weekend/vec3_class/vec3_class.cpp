#include "pch.h"
#include "vec3.h"
using namespace std;

int main()
{
	FILE *stream;
	freopen_s(&stream,"test.ppm", "w", stdout);
	int nx = 200;
	int ny = 200;
	cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int i = 0; i < nx; ++i) {
		for (int j = 0; j < ny; ++j) {
			vec3<float> col(float(i) / float(nx), float(j) / float(ny),0.2);
			int ir = int(col[0]*255.99);
			int ig = int(col[1]*255.99);
			int ib = int(col[2]*255.99);
			cout << ir << " " << ig << " " << ib << endl;
		}
	}
}