#pragma once
#ifndef CAMERA
#define CAMERA

#include "ray.h"

namespace yph {
	vec3f randomInUnitDisk() {
		vec3f p;
		do {
			p = vec3f((rand() % 10) / 10.0, (rand() % 10) / 10.0, 0) * 2-vec3f(1.0,1.0,0);
		} while (dot(p, p) >= 1.0);
		return p;
	}

	class camera {
	private:
		vec3f origin;
		vec3f lowerLeftCorner;
		vec3f vertical;
		vec3f horizontal;
		vec3f w, u, v; // 坐标轴
		float lensRadius;
	public:
		camera(vec3f lookfrom,vec3f lookat,vec3f vup, float vfov,float aspect,float aperture, float focusDist) { 
			// vfov是从上向下看的角度
			// aperture是光圈参数
			// aspect为图像长宽比
			float theta = vfov * PI / 180;
			float halfHeight = tan(theta / 2);
			float halfWidth = aspect * halfHeight;
			lensRadius = aperture / 2;
			origin = lookfrom;
			w = (lookfrom - lookat).makeUnitVector(); // 深度方向单位向量
			u = cross(vup,w).makeUnitVector();	// 竖直方向单位向量
			v = cross(w,u);	// 水平方向单位向量
			lowerLeftCorner = origin - halfWidth * u * focusDist - halfHeight * v * focusDist - w * focusDist;
			horizontal = 2 * halfWidth * u * focusDist; // 水平的长度向量
			vertical = 2 * halfHeight * v * focusDist; // 竖直的全部长度的向量表示
		}

		ray<float> getRay(float x,float y, bool useAperture = false) {
			// useAperture决定是否进行模拟实际的镜头
			vec3f rd = lensRadius * randomInUnitDisk();
			vec3f offset = u * rd.getX() + v * rd.getY();
			if(!useAperture) offset = vec3f(0,0,0);
			return ray<float>(origin + offset, lowerLeftCorner + x * horizontal + y * vertical - origin - offset);
		}
	};
}

#endif