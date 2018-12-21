#pragma once
#ifndef HIT_RECORD
#define HIT_RECORD
#include "ray.h"
#include "aabb.h"
#include "texture.h"

namespace yph {
	class material;

	const float T_MIN = 0.001f;
	const float T_MAX = 65535;
	struct hitRecord {
		float t;	// 产生接触时的光路上的t值
		float u, v; // 读入图片后图片上的相对坐标
		vec3f p;	// 接触点位置
		vec3f normal; // 法向量
		material *materialPtr;
	};

	// 碰撞体的虚基类
	class hitable {
	public:
		// 计算光线和物体碰撞
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const = 0;
		// 获取aabb包围盒
		virtual bool boundingBox(float t0, float t1, aabb& box) const = 0;
	};

	// 只用于翻转面法向量的类
	class flipNormals : public hitable {
	private:
		hitable *hitablePtr;
	public:
		flipNormals(hitable *ptr) :hitablePtr(ptr) {}
		// 计算光线和物体碰撞
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
			if (hitablePtr->hit(r, tMin, tMax, rec)) {
				rec.normal = -rec.normal;
				return true;
			}
			else {
				return false;
			}
		}
		// 获取aabb包围盒
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			return hitablePtr->boundingBox(t0, t1, box);
		}
	};

	// 只用于平移的类
	class translation : public hitable {
	private:
		hitable *hitablePtr;
		vec3f offset;
	public:
		translation(hitable *ptr, const vec3f& displacement) :hitablePtr(ptr), offset(displacement) {}
		// 计算光线和物体碰撞
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
			ray<float> rMoved(r.getOrigin() - offset, r.getDirection());							// 移动平移后位置的光线到平移前处判断相交情况，之后再将碰撞点加上这样一个偏移
			if (hitablePtr->hit(rMoved, tMin, tMax, rec)) {
				rec.p += offset;
				return true;
			}
			else {
				return false;
			}
		}
		// 获取aabb包围盒
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

	// 只用于旋转的类，沿着y轴旋转
	class rotateY : public hitable {
	private:
		hitable *hitablePtr;
		float sinTheta;
		float cosTheta;
		bool hasbox;
		aabb _box;
	public:
		rotateY(hitable *p, float angle);
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
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
		// 求解旋转后的新的包围盒
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

	bool rotateY::hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// 反着变换光线，检测是否与当前的物体相交，之后再变换碰撞点
		vec3f origin = r.getOrigin();
		vec3f direction = r.getDirection();
		origin[0] = cosTheta* r.getOrigin()[0] - sinTheta* r.getOrigin()[2];
		origin[2] = sinTheta* r.getOrigin()[0] + cosTheta* r.getOrigin()[2];
		direction[0] = cosTheta* r.getDirection()[0] - sinTheta* r.getDirection()[2];
		direction[2] = sinTheta* r.getDirection()[0] + cosTheta* r.getDirection()[2];
		ray<float> rayRotated(origin,direction);
		if (hitablePtr->hit(rayRotated,tMin,tMin,rec)) {
			// 在xz平面上旋转碰撞点
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
}

#endif // !HIT_RECORD
