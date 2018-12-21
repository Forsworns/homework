#pragma once
#ifndef RECTANGLE
#define RECTANGLE

#include "hitable.h"
namespace yph {
	const float HALF_THICKNESS = 0.0001;

	// xy平面上的矩形
	class xyRectangle : public hitable {
	private:
		float x0, x1, y0, y1, k; // 在z = k处建立一个厚度为0.0001的薄板矩形
		material *materialPtr;
	public:
		xyRectangle() {}
		xyRectangle(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) :x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), materialPtr(mat) {}
		virtual bool hit(const ray<float>& r, float t0,float t1, hitRecord &rec) const;
		virtual bool boundingBox(float t0, float t1, aabb &box) const { 
			box = aabb(vec3f(x0, y0, k - HALF_THICKNESS), vec3f(x1, y1, k + HALF_THICKNESS));
			return true;
		}
	};
	bool xyRectangle::hit(const ray<float>& r, float t0, float t1, hitRecord &rec) const {
		float t = (k - r.getOrigin().getZ())/r.getDirection().getZ();
		//std::cout << "与xy长方体交点" << t << "光线起点" << r.getOrigin() << "方向"<< r.getDirection()<< std::endl;
		if (t<t0 || t>t1) { return false; }	// 交点光线方向t值超过限制
		float x = r.getOrigin().getX() + t * r.getDirection().getX();
		float y = r.getOrigin().getY() + t * r.getDirection().getY(); // 光线与长方体的交点
		//std::cout << "交点" << x << " " << y << "长方体" << x0 << " " << x1 << " " << y0 << " " << y1 << std::endl;
		if (x<x0 || x>x1 || y<y0 || y>y1) { return false; }
		rec.u = (x - x0) / (x1 - x0);
		rec.v = (y - y0) / (y1 - y0);
		rec.t = t;
		rec.materialPtr = materialPtr;
		rec.p = r.getPosWithPara(t);
		rec.normal = vec3f(0, 0, 1);
		return true;
	}

	// xz平面上的矩形
	class xzRectangle : public hitable {
	private:
		float x0, x1, z0, z1, k; // 在y = k处建立一个厚度为0.0001的薄板矩形
		material *materialPtr;
	public:
		xzRectangle() {}
		xzRectangle(float _x0, float _x1, float _z0, float _z1, float _k, material *mat) :x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), materialPtr(mat) {}
		virtual bool hit(const ray<float>& r, float t0, float t1, hitRecord &rec) const;
		virtual bool boundingBox(float t0, float t1, aabb &box) const {
			box = aabb(vec3f(x0, k - HALF_THICKNESS, z0), vec3f(x1, k + HALF_THICKNESS, z1));
			return true;
		}
	};
	bool xzRectangle::hit(const ray<float>& r, float t0, float t1, hitRecord &rec) const {
		float t = (k - r.getOrigin().getY()) / r.getDirection().getY();
		if (t<t0 || t>t1) { return false; }	// 交点光线方向t值超过限制
		float x = r.getOrigin().getX() + t * r.getDirection().getX();
		float z = r.getOrigin().getZ() + t * r.getDirection().getZ(); // 光线与长方体的交点
		if (x<x0 || x>x1 || z<z0 || z>z1) { return false; }
		rec.u = (x - x0) / (x1 - x0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.materialPtr = materialPtr;
		rec.p = r.getPosWithPara(t);
		rec.normal = vec3f(0, 1, 0);
		return true;
	}

	// yz平面上的矩形
	class yzRectangle : public hitable {
	private:
		float y0, y1, z0, z1, k; // 在x = k处建立一个厚度为0.0001的薄板矩形
		material *materialPtr;
	public:
		yzRectangle() {}
		yzRectangle(float _y0, float _y1, float _z0, float _z1, float _k, material *mat) :y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), materialPtr(mat) {}
		virtual bool hit(const ray<float>& r, float t0, float t1, hitRecord &rec) const;
		virtual bool boundingBox(float t0, float t1, aabb &box) const {
			box = aabb(vec3f(k - HALF_THICKNESS, y0, z0), vec3f(k + HALF_THICKNESS, y1, z1));
			return true;
		}
	};
	bool yzRectangle::hit(const ray<float>& r, float t0, float t1, hitRecord &rec) const {
		float t = (k - r.getOrigin().getX()) / r.getDirection().getX();
		if (t<t0 || t>t1) { return false; }	// 交点光线方向t值超过限制
		float y = r.getOrigin().getY() + t * r.getDirection().getY();
		float z = r.getOrigin().getZ() + t * r.getDirection().getZ(); // 光线与长方体的交点
		if (y<y0 || y>y1 || z<z0 || z>z1) { return false; }
		rec.u = (y - y0) / (y1 - y0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.materialPtr = materialPtr;
		rec.p = r.getPosWithPara(t);
		rec.normal = vec3f(1, 0, 0);
		return true;
	}

	// 长方体
	class box :public hitable {
	private:
		hitable *boxPtr;
		vec3f posMin, posMax;
	public:
		box() {}
		box(const vec3f& p0, const vec3f& p1, material* mat);
		virtual bool hit(const ray<float>& r, float t0, float t1, hitRecord &rec) const {
			return boxPtr->hit(r, t0, t1, rec);
		}
		virtual bool boundingBox(float t0, float t1, aabb &box) const {
			box = aabb(posMin,posMax);
			return true;
		}
	};
	box::box(const vec3f& p0, const vec3f& p1, material* mat) {
		posMin = p0;
		posMax = p1;
		hitable **list = new hitable*[6];
		list[0] = new xyRectangle(p0.getX(), p1.getX(), p0.getY(), p1.getY(), p1.getZ(), mat);
		list[1] = new flipNormals(new xyRectangle(p0.getX(), p1.getX(), p0.getY(), p1.getY(), p0.getZ(), mat));
		list[2] = new xzRectangle(p0.getX(), p1.getX(), p0.getZ(), p1.getZ(), p1.getY(), mat);
		list[3] = new flipNormals(new xzRectangle(p0.getX(), p1.getX(), p0.getZ(), p1.getZ(), p0.getY(), mat));
		list[4] = new yzRectangle(p0.getY(), p1.getY(), p0.getZ(), p1.getZ(), p1.getX(), mat);
		list[5] = new flipNormals(new yzRectangle(p0.getY(),p1.getY(),p0.getZ(),p1.getZ(),p0.getX(),mat));
		boxPtr = new hitableList(list,6);
	}
}

#endif // !RECTANGLE
