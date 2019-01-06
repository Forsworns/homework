#ifndef SPHERE
#define SPHERE
#include "hitable.h"

namespace yph {
	// ��texture�еı������껻���������ϵ�µĽǶ�����
	void getSphereUV(const vec3f& pos, float &u, float&v) {
		float phi = atan2(pos.getZ(), pos.getX());
		float theta = asin(pos.getY());
		u = 1 - (phi + PI) / (2 * PI);
		v = (theta + PI / 2) / PI;
	}

	class sphere : public hitable {
	private:
		float radius;						// �뾶Ϊ�����ı�����״�����Ƿ������ᳯ���ڲ�
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
		// ���� tmin��tmax�޶�����ȡ����ķ�Χ������������̭����Ұ����ģ�����tmin>0����֮����������������¿����ҳ��ǰ��һ��
		vec3f oc = r.getOrigin() - center;
		float a = dot(r.getDirection(), r.getDirection());
		float b = 2 * dot(r.getDirection(), oc);
		float c = dot(oc, oc) - radius * radius;
		float discriminant = b * b - 4 * a * c;
		if (discriminant > 0) {
			float temp = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);			// ����һԪ���η��������ʽ
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
