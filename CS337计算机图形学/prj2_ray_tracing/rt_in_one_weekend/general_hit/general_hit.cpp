#include "pch.h"
#include "ray.h"
#include "vec3.h"
#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <iostream>
#include <random>
using namespace yph;

vec3<float> color(const ray<float>& r, hitable *world) {
	hitRecord rec;
	if (world->hit(r,0,65536,rec)) {
		return 0.5f * (rec.normal + vec3<float>(1, 1, 1));
	}
	else {
		vec3<float> unitDirection = r.getDirection().makeUnitVector();
		float t = 0.5*(unitDirection.getY() + 1.0);						//限定在0到1
		return (1.0f - t)*vec3<float>(1.0, 1.0, 1.0) + t * vec3<float>(0.5, 0.7, 1.0);
	}
}

int main()
{
	FILE *stream;
	freopen_s(&stream, "test.ppm", "w", stdout);
	int nx = 200;
	int ny = 100;
	int ns = 100;

	std::default_random_engine engine;
	std::uniform_real_distribution<double> distibution(0,1);

	hitable* list[2];																// 指针数组
	list[0] = new sphere(vec3<float>(0,0,-1),0.5);
	list[1] = new sphere(vec3<float>(0,-100.5,-1),100);
	hitable* world = new hitableList(list, 2);

	camera cam;

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; --j) {												 //从左上角开始绘制
		for (int i = 0; i < nx; ++i) {
			vec3<float> col(0, 0, 0);
			for (int s = 0; s < ns; ++s) {											
				// 一个像素照射多次，每次随机产生一个偏差防止总是取一个像素的一个位置造成颜色剧变和锯齿，对颜色求和后再求平均值
				float u = float(i + distibution(engine)) / float(nx);
				float v = float(j + distibution(engine)) / float(ny);
				ray<float> r = cam.getRay(u, v);
				col += color(r, world);
			}
			col /= float(ns);
	
			int ir = int(col[0] * 255.99);
			int ig = int(col[1] * 255.99);
			int ib = int(col[2] * 255.99);
			std::cout << ir << " " << ig << " " << ib << std::endl;
		}
	}
}
