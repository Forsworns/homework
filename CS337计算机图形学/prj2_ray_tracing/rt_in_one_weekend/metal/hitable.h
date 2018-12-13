#pragma once
#ifndef HIT_RECORD
#define HIT_RECORD
#include "ray.h"

namespace yph {
	class material;
	const float T_MIN = 0.001f;
	const float T_MAX = 65535;
	struct hitRecord {
		float t;	// �����Ӵ�ʱ��tֵ
		vec3<float> p;	//�Ӵ���
		vec3<float> normal; // ������
		material *materialPtr;
	};
	class hitable {	// abstract class
	public:
		virtual bool hit(const ray<float>& r, float tMin,float tMax,hitRecord& rec) const = 0;
	};
}

#endif // !HIT_RECORD
