#pragma once
#ifndef TRIANGLE
#define TRIANGLE

#include "hitable.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <fstream>

namespace yph {
	class triangle : public hitable {
	public:
		vec3f points[3];
		vec3f normal; // 面的法向量
		material *materialPtr;
	public:
		triangle() {
			points[0] = {0,0,0};
			points[1] = {0,0,0};
			points[2] = {0,0,0};
			normal = { 0,0,0 };
		}
		
		triangle(vec3f *p, material *mat) : materialPtr(mat) {
			points[0] = p[0];
			points[1] = p[1];
			points[2] = p[2];
			normal = cross(points[1] - points[0],points[1] - points[0]); // 直接计算法向量
		}

		triangle(vec3f p0, vec3f p1, vec3f p2, material *mat) : materialPtr(mat) {
			points[0] = p0;
			points[1] = p1;
			points[2] = p2;
			normal = cross(points[1] - points[0], points[1] - points[0]); // 直接计算法向量
		}
		
		// 计算光线和物体碰撞
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		// 获取aabb包围盒
		virtual bool boundingBox(float t0, float t1, aabb& box) const {
			vec3f minPoint(T_MAX, T_MAX, T_MAX);
			vec3f maxPoint(-T_MAX, -T_MAX, -T_MAX);
			for (int i = 0; i < 3;++i) {
				for (int j = 0; j < 3;++j) {
					minPoint[i] = std::min(points[j][i],minPoint[i]);
					maxPoint[i] = std::max(points[j][i],maxPoint[i]);
				}
			}
			box = aabb(minPoint,maxPoint);
			return true;
		}
	};
	
	// 采用大作业AG中的同款光线——三角形相交检测算法, 引用自: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	bool triangle::hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		// 存在问题，不能相交就直接取了，有遮挡关系
		//V1,V2,V3 denote the points
		//Find vectors for two edges sharing V1
		vec3f e1 = points[1] - points[0];
		vec3f e2 = points[2] - points[0];

		//Begin calculating determinant - also used to calculate u parameter
		vec3f P = cross(r.getDirection(), e2);

		//if determinant is near zero, ray lies in plane of triangle
		float det = dot(e1, P);

		//NOT CULLING
		if (det > -T_MIN && det < T_MIN)
			return false;
		float inv_det = 1.f / det;

		// calculate distance from V1 to ray origin
		vec3f T = r.getOrigin() - points[0];
		//Calculate u parameter and test bound
		float u = dot(T, P) * inv_det;
		//The intersection lies outside of the triangle
		if (u < 0.f || u > 1.f)
			return false;

		//Prepare to test v parameter
		vec3f Q = cross(T, e1);
		//Calculate v parameter and test bound
		float v = dot(r.getDirection(), Q) * inv_det;
		//The intersection lies outside of the triangle
		if (v < 0.f || u + v > 1.f)
			return false;

		float t = dot(e2, Q) * inv_det;

		if (t > tMin && t < tMax)
		{ // ray intersection
			aabb box;
			boundingBox(0, 0, box);
			rec.materialPtr = materialPtr;
			rec.t = t;
			rec.materialPtr = materialPtr;
			rec.p = r.getPosWithPara(t);
			rec.normal = normal;
			rec.u = (rec.p.getX() -box.getMin().getX()) / (box.getMax().getX() - box.getMin().getX());
			rec.v = (rec.p.getY() -box.getMin().getY()) / (box.getMax().getY() - box.getMin().getY());
			return true;
		}

		// No hit, no win
		return false;
	}
	
	// 把mesh model文件读入，存入一个三角形列表中
	void loadMeshModel(std::string fileName, material* materialPtr, triangle** triList) {
		std::ifstream fin(fileName);
		std::vector<triangle> triVector;
		std::vector<vec3f> vertices;
		char tmpchar[100];
		int tmpnum;
		int nVer, nFace;

		fin >> tmpchar;
		fin >> nVer >> nFace >> tmpnum;
		// 读入点集
		for (int i = 0; i < nVer; ++i) {
			vec3f vertex;
			fin >> vertex;
			vertices.push_back(vertex);
		}
		// 建立三角面信息
		for (int i = 0; i < nFace; ++i) {
			vec3<int> face;
			fin >> tmpnum >> face;
			triList[i] = new triangle(vertices[face.getX()], vertices[face.getY()], vertices[face.getZ()], materialPtr);
		}
		fin.close();
	}

	int countTriNum(std::string modelFile) {
		std::ifstream fin(modelFile);
		char tmpchar[100];
		int tmpnum, triNum;
		fin >> tmpchar;
		fin >> tmpnum >> triNum >> tmpnum;
		fin.close();
		return triNum;
	}
	
}

#endif // TRIANGLE
