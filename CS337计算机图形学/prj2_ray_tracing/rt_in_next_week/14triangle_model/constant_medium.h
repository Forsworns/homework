#pragma once
#ifndef CONSTANT_MEDIUM
#define CONSTANT_MEDIUM
#include "material.h"
#include "hitable.h"

namespace yph {
	// 用于烟雾效果的类
	class constantMedium :public hitable {
	private:
		hitable *boundary;
		float density;
		material *materialPtr;
	public:
		constantMedium(hitable *h, float d, texture *t) :boundary(h), density(d) {
			materialPtr = new isotropic(t);
		}
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			return boundary->boundingBox(t0, t1, box);
		}
	};

	bool constantMedium::hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		hitRecord rec1, rec2;
		if (boundary->hit(r, -T_MAX, T_MAX, rec1)) {
			if (boundary->hit(r, rec1.t + T_MIN, T_MAX, rec2)) {
				if (rec1.t < tMin) { rec1.t = tMin; }
				if (rec2.t > tMax) { rec2.t = tMax; }
				if (rec1.t >= rec2.t) { return false; }
				if (rec1.t < 0) { rec1.t = 0; }
				float distanceInsideBoundary = (rec2.t - rec1.t)*r.getDirection().length();
				float hitDistance = -(1 / density)*log((rand() % 10) / 10.0);
				if (hitDistance < distanceInsideBoundary) {
					rec.t = rec1.t + hitDistance / r.getDirection().length();
					rec.p = r.getPosWithPara(rec.t);
					rec.normal = vec3f(1, 0, 0);		//任意方向均可
					rec.materialPtr = materialPtr;
					return true;
				}
			}
		}
		return false;
	}

}
#endif // !CONSTANT_MEDIUM


