#pragma once
#ifndef SPHERE
#define SPHERE
#include "hitable.h"

namespace yph {
	class sphere : public hitable {
	private:
		float radius;
		vec3<float> center;
	public:
		sphere() {}
		sphere(vec3<float> cen, float r) :center(cen), radius(r) {}
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		float getRadius() const { return radius; }
		vec3<float> getCenter() const { return center; }
	};
	bool sphere::hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// ���� tmin��tmax�޶�����ȡ����ķ�Χ������������̭����Ұ����ģ�����tmin>0����֮����������������¿����ҳ��ǰ��һ��
		vec3<float> oc = r.getOrign() - center;
		float a = dot(r.getDirection(), r.getDirection());
		float b = 2 * dot(r.getDirection(), oc);
		float c = dot(oc, oc) - radius * radius;
		float discriminant = b * b - 4 * a * c;
		if (discriminant > 0) {
			float temp = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);
			if (temp > tMin && temp < tMax) {
				rec.t = temp;
				rec.p = r.getPosWithPara(rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
			temp = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
			if (temp > tMin && temp < tMax) {
				rec.t = temp;
				rec.p = r.getPosWithPara(rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
		}
		return false;
	}
}

#endif // !SPHERE
