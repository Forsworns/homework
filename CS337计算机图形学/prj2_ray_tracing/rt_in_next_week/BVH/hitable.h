#pragma once
#ifndef HIT_RECORD
#define HIT_RECORD
#include "ray.h"
#include "aabb.h"

namespace yph {
	class material;
	const float T_MIN = 0.001f;
	const float T_MAX = 65535;
	struct hitRecord {
		float t;	// �����Ӵ�ʱ�Ĺ�·�ϵ�tֵ
		float u, v; // ����ͼƬ��ͼƬ�ϵ��������
		vec3f p;	// �Ӵ���
		vec3f normal; // ������
		material *materialPtr;
	};
	class hitable {	// abstract class
	public:
		// ������ߺ�������ײ
		virtual bool hit(const ray<float>& r, float tMin,float tMax,hitRecord& rec) const = 0; 
		// ��ȡaabb��Χ��
		virtual bool boundingBox(float t0, float t1, aabb& box) const = 0; 
	};
}

#endif // !HIT_RECORD
