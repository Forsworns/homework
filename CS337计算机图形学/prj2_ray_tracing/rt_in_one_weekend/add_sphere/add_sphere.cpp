#include "pch.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>
using namespace yph;

bool hitSphere(vec3<float> center,float radius,const ray<float>& r) {
	vec3<float> oc = r.getOrign() - center;								// 由二元一次方程根求解
	float a = dot(r.getDirection(),r.getDirection());
	float b = 2 * dot(r.getDirection(),oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant > 0) {
		return true;
	} else {
		return false;
	}
}

vec3<float> color(const ray<float>& r) {
	if (hitSphere(vec3<float>(0,0,-1),0.5,r)) {
		return vec3<float>(1,0,0);
	}
	else {
		vec3<float> unitDirection = r.getDirection().makeUnitVector();
		float t = 0.5*(unitDirection.getY() + 1.0);						//限定在0到1
		return (1.0f - t)*vec3<float>(1.0, 1.0, 1.0) + t * vec3<float>(0.5, 0.7, 1.0);
	}
}

float hitSphere2(vec3<float> center, float radius, const ray<float>& r) { // 利用法向量确定是否是在视角背面
	vec3<float> oc = r.getOrign() - center;								// 由二元一次方程根求解
	float a = dot(r.getDirection(), r.getDirection());
	float b = 2 * dot(r.getDirection(), oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1;
	}
	else {
		return (-b - sqrt(discriminant)) / (2 * a);
	}
}

vec3<float> color2(ray<float>& r) {
	float t = hitSphere2(vec3<float>(0, 0, -1), 0.5, r);
	if (t > 0) {
		vec3<float>	N = (r.getPosWithPara(t) - vec3<float>(0, 0, -1)).makeUnitVector(); 
		//利用光线与球面交点处的t值，求出球面交点位置减去球心，为该交点处球面法向量
		return 0.5f * (N + vec3<float>(1,1,1));
	}
	else {
		vec3<float> unitDirection = r.getDirection().makeUnitVector();
		t = 0.5*(unitDirection.getY() + 1.0);						//限定在0到1
		return (1.0f - t)*vec3<float>(1.0, 1.0, 1.0) + t * vec3<float>(0.5, 0.7, 1.0);
	}
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


	freopen_s(&stream, "test2.ppm", "w", stdout);
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; --j) {												 //从左上角开始绘制
		for (int i = 0; i < nx; ++i) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray<float> r(origin, lower_left_corner + u * horizontal + v * vertical); //遍历每个点，越靠上越蓝，靠下为白
			vec3<float> col = color2(r);
			int ir = int(col[0] * 255.99);
			int ig = int(col[1] * 255.99);
			int ib = int(col[2] * 255.99);
			std::cout << ir << " " << ig << " " << ib << std::endl;
		}
	}
}
