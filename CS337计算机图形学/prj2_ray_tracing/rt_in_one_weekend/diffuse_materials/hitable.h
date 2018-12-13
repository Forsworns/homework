#pragma once
#ifndef HIT_RECORD
#define HIT_RECORD
#include "ray.h"

namespace yph {
	struct hitRecord {
		float t;	// 产生接触时的t值
		vec3<float> p;	//接触点
		vec3<float> normal; // 法向量
	};
	class hitable {	// abstract class
	public:
		virtual bool hit(const ray<float>& r, float tMin,float tMax,hitRecord& rec) const = 0;
	};
}

#endif // !HIT_RECORD
