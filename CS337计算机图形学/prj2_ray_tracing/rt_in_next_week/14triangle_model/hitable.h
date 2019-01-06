#pragma once
#ifndef HIT_RECORD
#define HIT_RECORD
#include "ray.h"
#include "aabb.h"
#include "texture.h"

namespace yph {
	class material;

	const float T_MIN = 0.001f;
	const float T_MAX = 1000000000.0f;
	struct hitRecord {
		float t;	// �����Ӵ�ʱ�Ĺ�·�ϵ�tֵ
		float u, v; // ����ͼƬ��ͼƬ�ϵ��������
		vec3f p;	// �Ӵ���λ��
		vec3f normal; // ������
		material *materialPtr;
	};

	// ��ײ��������
	class hitable {
	public:
		// ������ߺ�������ײ
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const = 0;
		// ��ȡaabb��Χ��
		virtual bool boundingBox(float t0, float t1, aabb& box) const = 0;
	};

	// ֻ���ڷ�ת�淨��������
	class flipNormals : public hitable {
	private:
		hitable *hitablePtr;
	public:
		flipNormals(hitable *ptr) :hitablePtr(ptr) {}
		// ������ߺ�������ײ
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
			if (hitablePtr->hit(r, tMin, tMax, rec)) {
				rec.normal = -rec.normal;
				return true;
			}
			else {
				return false;
			}
		}
		// ��ȡaabb��Χ��
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			return hitablePtr->boundingBox(t0, t1, box);
		}
	};

	// ֻ����ƽ�Ƶ���
	class translation : public hitable {
	private:
		hitable *hitablePtr;
		vec3f offset;
	public:
		translation(hitable *ptr, const vec3f& displacement) :hitablePtr(ptr), offset(displacement) {}
		// ������ߺ�������ײ
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
			ray<float> rMoved(r.getOrigin() - offset, r.getDirection());							// �ƶ�ƽ�ƺ�λ�õĹ��ߵ�ƽ��ǰ���ж��ཻ�����֮���ٽ���ײ���������һ��ƫ��
			if (hitablePtr->hit(rMoved, tMin, tMax, rec)) {
				rec.p += offset;
				return true;
			}
			else {
				return false;
			}
		}
		// ��ȡaabb��Χ��
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			if (hitablePtr->boundingBox(t0, t1, box)) {
				box = aabb(box.getMin() + offset, box.getMax() + offset);
				return true;
			}
			else {
				return false;
			}
		}
	};

	// ֻ������ת���࣬����y����ת
	class rotateY : public hitable {
	private:
		hitable *hitablePtr;
		float sinTheta;
		float cosTheta;
		bool hasbox;
		aabb _box;
	public:
		rotateY(hitable *p, float angle);
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			box = _box;
			return hasbox;
		}
	};

	rotateY::rotateY(hitable *p, float angle):hitablePtr(p) {
		float theta = (PI / 180.0)*angle;
		sinTheta = sin(theta);
		cosTheta = cos(theta);
		hasbox = hitablePtr->boundingBox(0, 1, _box);
		// �����ת����µİ�Χ��
		vec3f minPos(T_MAX, T_MAX, T_MAX);
		vec3f maxPos(-T_MAX, -T_MAX, -T_MAX);
		for (int i = 0; i < 2;++i) {
			for (int j = 0; j < 2;++j) {
				for (int k = 0; k < 2;++k) {
					float x = i * _box.getMax().getX() + (1 - i)*_box.getMin().getX();
					float y = j * _box.getMax().getY() + (1 - j)*_box.getMin().getY();
					float z = k * _box.getMax().getZ() + (1 - k)*_box.getMin().getZ();
					float newx = cosTheta * x + sinTheta * z;
					float newz = -sinTheta * x + cosTheta * z;
					vec3f tester(newx, y, newz);
					for (int c = 0; c < 3;c++) {
						if (tester[c] > maxPos[c]) {
							maxPos[c] = tester[c];
						}
						if (tester[c]<minPos[c]) {
							minPos[c] = tester[c];
						}
					}
				}
			}
		}
		_box = aabb(minPos, maxPos);
	}

	bool rotateY::hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// ���ű任���ߣ�����Ƿ��뵱ǰ�������ཻ��֮���ٱ任��ײ��
		vec3f origin = r.getOrigin();
		vec3f direction = r.getDirection();
		origin[0] = cosTheta* r.getOrigin()[0] - sinTheta* r.getOrigin()[2]; // ������ȡ��-theta���ķ��任
		origin[2] = sinTheta* r.getOrigin()[0] + cosTheta* r.getOrigin()[2];
		direction[0] = cosTheta* r.getDirection()[0] - sinTheta* r.getDirection()[2];
		direction[2] = sinTheta* r.getDirection()[0] + cosTheta* r.getDirection()[2];
		ray<float> rayRotated(origin,direction);
		if (hitablePtr->hit(rayRotated,tMin,tMax,rec)) {
			// ��xzƽ������ת��ײ��
			vec3f pos = rec.p;
			vec3f normal = rec.normal;
			pos[0] = cosTheta*rec.p[0]+sinTheta*rec.p[2];
			pos[2] = -sinTheta * rec.p[0] + cosTheta * rec.p[2];
			normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
			normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];
			rec.p = pos;
			rec.normal = normal;
			return true;
		}
		else {
			return false;
		}
	}

	// ֻ������ת���࣬����z����ת
	class rotateZ : public hitable {
	private:
		hitable *hitablePtr;
		float sinTheta;
		float cosTheta;
		bool hasbox;
		aabb _box;
	public:
		rotateZ(hitable *p, float angle);
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			box = _box;
			return hasbox;
		}
	};

	rotateZ::rotateZ(hitable *p, float angle) :hitablePtr(p) {
		float theta = (PI / 180.0)*angle;
		sinTheta = sin(theta);
		cosTheta = cos(theta);
		hasbox = hitablePtr->boundingBox(0, 1, _box);
		// �����ת����µİ�Χ��
		vec3f minPos(T_MAX, T_MAX, T_MAX);
		vec3f maxPos(-T_MAX, -T_MAX, -T_MAX);
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					float x = i * _box.getMax().getX() + (1 - i)*_box.getMin().getX();
					float y = j * _box.getMax().getY() + (1 - j)*_box.getMin().getY();
					float z = k * _box.getMax().getZ() + (1 - k)*_box.getMin().getZ();
					float newx = cosTheta * x - sinTheta * y;
					float newy = sinTheta * x + cosTheta * y;
					vec3f tester(newx, newy, z);
					for (int c = 0; c < 3; c++) {
						if (tester[c] > maxPos[c]) {
							maxPos[c] = tester[c];
						}
						if (tester[c] < minPos[c]) {
							minPos[c] = tester[c];
						}
					}
				}
			}
		}
		_box = aabb(minPos, maxPos);
	}

	bool rotateZ::hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// ���ű任���ߣ�����Ƿ��뵱ǰ�������ཻ��֮���ٱ任��ײ��
		vec3f origin = r.getOrigin();
		vec3f direction = r.getDirection();
		origin[0] = cosTheta * r.getOrigin()[0] + sinTheta * r.getOrigin()[1];
		origin[1] = -sinTheta * r.getOrigin()[0] + cosTheta * r.getOrigin()[1];
		direction[0] = cosTheta * r.getDirection()[0] + sinTheta * r.getDirection()[1];
		direction[1] = -sinTheta * r.getDirection()[0] + cosTheta * r.getDirection()[1];
		ray<float> rayRotated(origin, direction);
		if (hitablePtr->hit(rayRotated, tMin, tMax, rec)) {
			// ��xyƽ������ת��ײ��
			vec3f pos = rec.p;
			vec3f normal = rec.normal;
			pos[0] = cosTheta * rec.p[0] - sinTheta * rec.p[1];
			pos[1] = sinTheta * rec.p[0] + cosTheta * rec.p[1];
			normal[0] = cosTheta * rec.normal[0] - sinTheta * rec.normal[1];
			normal[1] = sinTheta * rec.normal[0] + cosTheta * rec.normal[1];
			rec.p = pos;
			rec.normal = normal;
			return true;
		}
		else {
			return false;
		}
	}

	// ֻ������ת���࣬����x����ת
	class rotateX : public hitable {
	private:
		hitable *hitablePtr;
		float sinTheta;
		float cosTheta;
		bool hasbox;
		aabb _box;
	public:
		rotateX(hitable *p, float angle);
		virtual bool hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			box = _box;
			return hasbox;
		}
	};

	rotateX::rotateX(hitable *p, float angle) :hitablePtr(p) {
		float theta = (PI / 180.0)*angle;
		sinTheta = sin(theta);
		cosTheta = cos(theta);
		hasbox = hitablePtr->boundingBox(0, 1, _box);
		// �����ת����µİ�Χ��
		vec3f minPos(T_MAX, T_MAX, T_MAX);
		vec3f maxPos(-T_MAX, -T_MAX, -T_MAX);
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					float x = i * _box.getMax().getX() + (1 - i)*_box.getMin().getX();
					float y = j * _box.getMax().getY() + (1 - j)*_box.getMin().getY();
					float z = k * _box.getMax().getZ() + (1 - k)*_box.getMin().getZ();
					float newy = cosTheta * y - sinTheta * z;
					float newz = sinTheta * y + cosTheta * z;
					vec3f tester(x, newy, newz);
					for (int c = 0; c < 3; c++) {
						if (tester[c] > maxPos[c]) {
							maxPos[c] = tester[c];
						}
						if (tester[c] < minPos[c]) {
							minPos[c] = tester[c];
						}
					}
				}
			}
		}
		_box = aabb(minPos, maxPos);
	}

	bool rotateX::hit(ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// ���ű任���ߣ�����Ƿ��뵱ǰ�������ཻ��֮���ٱ任��ײ��
		vec3f origin = r.getOrigin();
		vec3f direction = r.getDirection();
		origin[1] = cosTheta * r.getOrigin()[1] + sinTheta * r.getOrigin()[2];
		origin[2] = -sinTheta * r.getOrigin()[1] + cosTheta * r.getOrigin()[2];
		direction[1] = cosTheta * r.getDirection()[1] + sinTheta * r.getDirection()[2];
		direction[2] = -sinTheta * r.getDirection()[1] + cosTheta * r.getDirection()[2];
		ray<float> rayRotated(origin, direction);
		if (hitablePtr->hit(rayRotated, tMin, tMax, rec)) {
			// ��xzƽ������ת��ײ��
			vec3f pos = rec.p;
			vec3f normal = rec.normal;
			pos[1] = cosTheta * rec.p[1] - sinTheta * rec.p[2];
			pos[2] = sinTheta * rec.p[1] + cosTheta * rec.p[2];
			normal[1] = cosTheta * rec.normal[1] - sinTheta * rec.normal[2];
			normal[2] = sinTheta * rec.normal[1] + cosTheta * rec.normal[2];
			rec.p = pos;
			rec.normal = normal;
			return true;
		}
		else {
			return false;
		}
	}
}

#endif // !HIT_RECORD
