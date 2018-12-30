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
		vec3f w, u, v; // ������
		float lensRadius;
	public:
		camera(vec3f lookfrom,vec3f lookat,vec3f vup, float vfov,float aspect,float aperture, float focusDist) { 
			// vfov���ӽ�
			// aperture�ǹ�Ȧ����
			// aspectΪͼ�񳤿��
			float theta = vfov * PI / 180;
			float halfHeight = tan(theta / 2);
			float halfWidth = aspect * halfHeight;
			lensRadius = aperture / 2;
			origin = lookfrom;
			w = (lookfrom - lookat).makeUnitVector(); // ��ȷ���λ����
			u = cross(vup,w).makeUnitVector();	// ��ֱ����λ����
			v = cross(w,u);	// ˮƽ����λ����
			lowerLeftCorner = origin - halfWidth * u * focusDist - halfHeight * v * focusDist - w * focusDist;
			horizontal = 2 * halfWidth * u * focusDist; // ˮƽ�ĳ�������
			vertical = 2 * halfHeight * v * focusDist; // ��ֱ��ȫ�����ȵ�������ʾ
		}

		ray<float> getRay(float x,float y, bool useAperture = false) {
			// useAperture�����Ƿ����ģ��ʵ�ʵľ�ͷ
			vec3f rd = lensRadius * randomInUnitDisk();
			vec3f offset = u * rd.getX() + v * rd.getY();
			if(!useAperture) offset = vec3f(0,0,0);
			return ray<float>(origin + offset, lowerLeftCorner + x * horizontal + y * vertical - origin - offset);
		}
	};
}

#endif