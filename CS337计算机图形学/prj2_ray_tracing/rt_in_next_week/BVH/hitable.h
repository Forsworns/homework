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
		float t;	// 产生接触时的光路上的t值
		float u, v; // 读入图片后图片上的相对坐标
		vec3f p;	// 接触点
		vec3f normal; // 法向量
		material *materialPtr;
	};
	class hitable {	// abstract class
	public:
		// 计算光线和物体碰撞
		virtual bool hit(const ray<float>& r, float tMin,float tMax,hitRecord& rec) const = 0; 
		// 获取aabb包围盒
		virtual bool boundingBox(float t0, float t1, aabb& box) const = 0; 
	};
}

#endif // !HIT_RECORD
