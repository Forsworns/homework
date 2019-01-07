#pragma once
#ifndef TRIANGLE
#define TRIANGLE

#include "hitable.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <fstream>

namespace yph
{
class triangle : public hitable
{
  public:
	vec3f points[3];
	vec3f normal; // 面的法向量
	material *materialPtr;

  public:
	triangle()
	{
		points[0] = {0, 0, 0};
		points[1] = {0, 0, 0};
		points[2] = {0, 0, 0};
		normal = {0, 0, 0};
	}

	triangle(vec3f *p, material *mat) : materialPtr(mat)
	{
		points[0] = p[0];
		points[1] = p[1];
		points[2] = p[2];
		normal = cross(points[0] - points[1], points[2] - points[0]); // 直接计算法向量
	}

	triangle(vec3f p0, vec3f p1, vec3f p2, material *mat) : materialPtr(mat)
	{
		points[0] = p0;
		points[1] = p1;
		points[2] = p2;
		normal = cross(points[0] - points[1], points[2] - points[0]); // 直接计算法向量
	}

	// 计算光线和物体碰撞
	virtual bool hit(const ray<float> &r, float tMin, float tMax, hitRecord &rec) const;
	// 获取aabb包围盒
	virtual bool boundingBox(float t0, float t1, aabb &box) const
	{
		vec3f minPoint(T_MAX, T_MAX, T_MAX);
		vec3f maxPoint(-T_MAX, -T_MAX, -T_MAX);
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				minPoint[i] = std::min(points[j][i], minPoint[i]);
				maxPoint[i] = std::max(points[j][i], maxPoint[i]);
			}
		}
		box = aabb(minPoint, maxPoint);
		return true;
	}
};

// 采用大作业AG中的同款光线——三角形相交检测算法, 引用自: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool triangle::hit(const ray<float> &r, float tMin, float tMax, hitRecord &rec) const
{
	vec3f edge1 = points[1] - points[0];
	vec3f edge2 = points[2] - points[0];

	vec3f pvec = cross(r.getDirection(), edge2);
	float determinant = dot(edge1, pvec);

	// 平行
	if (fabs(determinant) < T_MIN)
		return false;

	vec3f tvec = r.getOrigin() - points[0];
	float u = dot(tvec, pvec) / determinant;
	if (u < 0.f || u > 1.f)
		return false;

	vec3f qvec = cross(tvec, edge1);
	float v = dot(r.getDirection(), qvec) / determinant;
	if (v < 0.f || u + v > 1.f)
		return false;

	float t = dot(edge2, qvec) / determinant;

	if (t > tMin && t < tMax)
	{
		aabb box;
		boundingBox(0, 0, box);
		rec.materialPtr = materialPtr;
		rec.t = t;
		rec.p = r.getPosWithPara(t);
		if (dot(r.getDirection(), normal) < 0)
		{
			rec.normal = normal;
		}
		else
		{
			rec.normal = -normal;
		}
		rec.u = (rec.p.getX() - box.getMin().getX()) / (box.getMax().getX() - box.getMin().getX());
		rec.v = (rec.p.getY() - box.getMin().getY()) / (box.getMax().getY() - box.getMin().getY());
		return true;
	}

	return false;
}

// -----------------------------------------------处理mesh模型输入--------------------------------------------------

int countTriNumOff(std::string modelFile)
{

	std::ifstream fin(modelFile);

	char tmpchar[100];

	int tmpnum, triNum;

	fin >> tmpchar;

	fin >> tmpnum >> triNum >> tmpnum;

	fin.close();

	return triNum;
}

// 把mesh model文件读入，存入一个三角形列表中

void loadMeshModelOff(std::string fileName, material *materialPtr, triangle **triList)
{

	std::ifstream fin(fileName);

	std::vector<triangle> triVector;

	std::vector<vec3f> vertices;

	char tmpchar[100];

	float tmpnum;

	int nVer, nFace;

	fin >> tmpchar;

	fin >> nVer >> nFace >> tmpnum;

	// 读入点集

	for (int i = 0; i < nVer; ++i)
	{

		vec3f vertex;

		fin >> vertex;

		vertices.push_back(vertex);
	}

	std::cout << "读入顶点" << std::endl;

	// 建立三角面信息

	for (int i = 0; i < nFace; ++i)
	{

		vec3i face;

		fin >> tmpnum >> face;

		triList[i] = new triangle(vertices[face.getX()], vertices[face.getY()], vertices[face.getZ()], materialPtr);
	}

	std::cout << "读入三角面" << std::endl;

	fin.close();
}

int countTriNumPly(std::string modelFile)
{

	std::ifstream fin(modelFile);

	char tmpchar[100];

	int triNum;

	fin >> tmpchar;

	while (strcmp(tmpchar, "face") != 0)
	{

		fin >> tmpchar;
	}

	fin >> triNum;

	fin.close();

	return triNum;
}

void loadMeshModelPly(std::string fileName, material *materialPtr, triangle **triList)
{

	std::ifstream fin(fileName);

	std::vector<triangle> triVector;

	std::vector<vec3f> vertices;

	char tmpchar[100];

	int nVer, nFace;

	float tmpnum;

	fin >> tmpchar;

	while (strcmp(tmpchar, "vertex") != 0)
	{

		fin >> tmpchar;
	}

	fin >> nVer;

	int properNum = 0;

	fin >> tmpchar;

	while (strcmp(tmpchar, "face") != 0)
	{

		if (strcmp(tmpchar, "property") == 0)
		{

			properNum++;
		}

		fin >> tmpchar;
	}

	fin >> nFace;

	while (strcmp(tmpchar, "end_header") != 0)
	{

		fin >> tmpchar;
	}

	for (int i = 0; i < nVer; ++i)
	{

		vec3f vertex;

		fin >> vertex;

		vertices.push_back(vertex);

		if (properNum > 3)
		{

			for (int j = 3; j < properNum; ++j)
			{

				fin >> tmpnum;
			}
		}
	}

	std::cout << "读入顶点" << nVer << std::endl;
	for (int i = 0; i < nFace; ++i)
	{
		vec3i face;
		fin >> tmpnum >> face;
		triList[i] = new triangle(vertices[face.getX()], vertices[face.getY()], vertices[face.getZ()], materialPtr);
	}
	std::cout << "读入三角面" << nFace << std::endl;
	fin.close();
}

int countTriNumObj(std::string modelFile)
{
	std::ifstream fin(modelFile);
	char tmpchar[100];
	int tmpnum, triNum;
	fin >> tmpchar;
	fin >> tmpnum >> triNum;
	fin.close();
	return triNum;
}

// 把mesh model文件读入，存入一个三角形列表中

void loadMeshModelObj(std::string fileName, material *materialPtr, triangle **triList)
{
	std::ifstream fin(fileName);
	std::vector<triangle> triVector;
	std::vector<vec3f> vertices;
	char tmpchar[100];
	float tmpnum;
	int nVer, nFace;
	fin >> tmpchar;
	fin >> nVer >> nFace;

	// 读入点集

	for (int i = 0; i < nVer; ++i)
	{
		fin >> tmpchar;
		vec3f vertex;
		fin >> vertex;
		vertices.push_back(vertex);
	}
	std::cout << "读入顶点" << nVer << std::endl;

	// 建立三角面信息
	for (int i = 0; i < nFace; ++i)
	{
		fin >> tmpchar;
		vec3i face;
		fin >> face;
		triList[i] = new triangle(vertices[face.getX() - 1], vertices[face.getY() - 1], vertices[face.getZ() - 1], materialPtr);
	}
	std::cout << "读入三角面" << nFace << std::endl;
	fin.close();
}

#define PLY 1
#define OFF 2
#define OBJ 3

int analyzeFileType(std::string modelFile)
{
	for (int i = 0; i < modelFile.length(); ++i)
	{
		if (modelFile[i] != '.')
		{
			continue;
		}
		else
		{
			if (modelFile[i + 1] == 'p')
			{
				return PLY;
			}
			else
			{
				if (modelFile[i + 2] == 'f')
				{
					return OFF;
				}
				else
				{
					return OBJ;
				}
			}
		}
	}
}

int countTriNum(std::string modelFile)
{
	int triNum;
	int fileType = analyzeFileType(modelFile);
	if (fileType == PLY)
	{
		triNum = countTriNumPly(modelFile);
	}
	else if (fileType == OFF)
	{
		triNum = countTriNumOff(modelFile);
	}
	else
	{
		triNum = countTriNumObj(modelFile);
	}
	return triNum;
}

// 把mesh model文件读入，存入一个三角形列表中

void loadMeshModel(std::string fileName, material *materialPtr, triangle **triList)
{
	int fileType = analyzeFileType(fileName);
	if (fileType == PLY)
	{
		loadMeshModelPly(fileName, materialPtr, triList);
	}
	else if (fileType == OFF)
	{
		loadMeshModelOff(fileName, materialPtr, triList);
	}
	else
	{
		loadMeshModelObj(fileName, materialPtr, triList);
	}
}
} // namespace yph
#endif // TRIANGLE
