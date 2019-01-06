#ifndef SPHERE
#define SPHERE
#include "hitable.h"

namespace yph {
	// 讲texture中的比例坐标换算成球坐标系下的角度坐标
	void getSphereUV(const vec3f& pos, float &u, float&v) {
		float phi = atan2(pos.getZ(), pos.getX());
		float theta = asin(pos.getY());
		u = 1 - (phi + PI) / (2 * PI);
		v = (theta + PI / 2) / PI;
	}

	class sphere : public hitable {
	private:
		float radius;						// 半径为负不改变其形状，但是法向量会朝向内部
		vec3f center;
		material *materialPtr;
	public:
		sphere() {}
		sphere(vec3f cen, float r, material* mPtr) :center(cen), radius(r),materialPtr(mPtr) {}
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		virtual bool boundingBox(float t0, float t1, aabb& box) const;
		float getRadius() const { return radius; }
		vec3f getCenter() const { return center; }
	};
	bool sphere::hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// 这里 tmin和tmax限定了求取交点的范围，不仅可以淘汰在视野背面的（如设tmin>0），之后在物体层叠的情况下可以找出最靠前的一个
		vec3f oc = r.getOrigin() - center;
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
				getSphereUV((rec.p-center)/radius,rec.u,rec.v);
				return true;
			}
			temp = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
			if (temp > tMin && temp < tMax) {
				rec.t = temp;
				rec.p = r.getPosWithPara(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.materialPtr = materialPtr;
				getSphereUV((rec.p - center) / radius, rec.u, rec.v);
				return true;
			}
		}
		return false;
	}
	bool sphere::boundingBox(float t0, float t1, aabb& box) const {
		box = aabb(center-vec3f(radius, radius, radius), center + vec3f(radius, radius, radius));
		return true;
	}
}

#endif // !SPHERE
