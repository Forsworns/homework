#pragma once
#ifndef SPHERE
#define SPHERE
#include "hitable.h"

namespace yph {
	class sphere : public hitable {
	private:
		float radius;						// 半径为负不改变其形状，但是法向量会朝向内部
		vec3<float> center;
		material *materialPtr;
	public:
		sphere() {}
		sphere(vec3<float> cen, float r, material* mPtr) :center(cen), radius(r),materialPtr(mPtr) {}
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		float getRadius() const { return radius; }
		vec3<float> getCenter() const { return center; }
	};
	bool sphere::hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// 这里 tmin和tmax限定了求取交点的范围，不仅可以淘汰在视野背面的（如设tmin>0），之后在物体层叠的情况下可以找出最靠前的一个
		vec3<float> oc = r.getOrign() - center;
		float a = dot(r.getDirection(), r.getDirection());
		float b = 2 * dot(r.getDirection(), oc);
		float c = dot(oc, oc) - radius * radius;
		float discriminant = b * b - 4 * a * c;
		if (discriminant > 0) {
			float temp = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);			// 利用一元二次方程求根公式
			if (temp > tMin && temp < tMax) {
				rec.t = temp;
				rec.p = r.getPosWithPara(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.materialPtr = materialPtr;
				return true;
			}
			temp = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
			if (temp > tMin && temp < tMax) {
				rec.t = temp;
				rec.p = r.getPosWithPara(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.materialPtr = materialPtr;
				return true;
			}
		}
		return false;
	}
}

#endif // !SPHERE
