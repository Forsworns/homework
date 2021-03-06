#include "pch.h"
#include "ray.h"
#include "vec3.h"
using namespace yph;

vec3<float> color(ray<float> r) {
	vec3<float> unitDirection = r.getDirection().makeUnitVector();
	float t = 0.5*(unitDirection.getY() + 1.0);						//限定在0到1
	return (1.0f - t)*vec3<float>(1.0, 1.0, 1.0) + t * vec3<float>(0.5, 0.7, 1.0);
}


int main()
{
	FILE *stream;
	freopen_s(&stream, "test.ppm", "w", stdout);
	int nx = 200;
	int ny = 100;

	vec3<float> lower_left_corner(-2.0, -1.0, -1.0);
	vec3<float> horizontal(4.0, 0.0, 0.0);
	vec3<float> vertical(0.0, 2.0, 0.0);
	vec3<float> origin(0.0, 0.0, 0.0);

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; --j) {												 //从左上角开始绘制
		for (int i = 0; i < nx; ++i) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray<float> r(origin, lower_left_corner + u * horizontal + v * vertical); //遍历每个点，越靠上越蓝，靠下为白
			vec3<float> col = color(r);
			int ir = int(col[0] * 255.99);
			int ig = int(col[1] * 255.99);
			int ib = int(col[2] * 255.99);
			std::cout << ir << " " << ig << " " << ib << std::endl;
		}
	}
}
