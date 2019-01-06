#pragma once
#ifndef SCENES
#define SCENES

#include "hitable.h"
#include "material.h"
#include "sphere.h"
#include "rectangle.h"
#include "camera.h"
#include "texture.h"
#include "BVH.h"
#include "constant_medium.h"
#include "hitable_list.h"

namespace yph {
	const std::string IMAGE = "models\\ground.jpg";
	const std::string MODEL = "models\\cube.obj";
	const float FLOOR_SIZE = 10000;

	hitable *floorInit(const aabb &sceneBox, int choseOne) {
		if (choseOne == 1) {
			// 棋盘地板
			texture *t = new checkerTexture(new constantTexture(vec3f(0.2, 0.6, 0.2)), new constantTexture(vec3f(1, 1, 1)));
			return new sphere(sceneBox.getMin() - vec3f(0, FLOOR_SIZE, 0), FLOOR_SIZE, new lambertian(t));	
		}
		else if (choseOne == 2) {
			// 纯色地板
			return new sphere(sceneBox.getMin() - vec3f(0, FLOOR_SIZE, 0), FLOOR_SIZE, new lambertian(new constantTexture(vec3f(0.2,0.6,0.2))));															
		}
		else if (choseOne == 3){
			// 噪声地板
			return new sphere(sceneBox.getMin() - vec3f(0, FLOOR_SIZE, 0), FLOOR_SIZE, new lambertian(new noiseTexture(4)));																				
		}
		else {
			// 图片地板
			floorInit(sceneBox);
		}
	}
	
	hitable *floorInit(const aabb &sceneBox, int nx = 400, int ny=400, std::string imageFile = IMAGE) {
		int nn;
		unsigned char* imgData = stbi_load(imageFile.c_str(), &nx, &ny, &nn, 0);
		return new sphere(sceneBox.getMin() - vec3f(0, FLOOR_SIZE, 0), FLOOR_SIZE, new lambertian(new imageTexture(imgData, nx, ny)));																		
	}

	// 正方形场景
	hitableList *scene1(int nx = 400,int ny = 400, std::string imageFile = IMAGE) {
		int nn;
		unsigned char* imgData = stbi_load(imageFile.c_str(), &nx, &ny, &nn, 0);
		hitable **list = new hitable*[100];
		int itemNum = 0;
		list[itemNum++] = new sphere();
		list[itemNum++] = new sphere(vec3f(0, 0, 3), 2, new lambertian(new constantTexture(vec3f(0.6, 0.3, 0.2))));
		list[itemNum++] = new sphere(vec3f(0, 0, 0), 2, new lambertian(new noiseTexture(4)));
		list[itemNum++] = new sphere(vec3f(0, 0, 0), 2, new lambertian(new imageTexture(imgData, nx, ny)));
		list[itemNum++] = new sphere(vec3f(3, 0, 4), 2, new metal(vec3f(0.2,0.5,0.3),1.0));
		list[itemNum++] = new sphere(vec3f(5, 0, 7), 2, new metal(vec3f(0.3,0.4,0.6),0.5));
		list[itemNum++] = new sphere(vec3f(1, 0, 2), 2, new dielectric(1.5));
		list[itemNum++] = new sphere(vec3f(1, 0, 2), -1.9, new dielectric(1.5));
		list[itemNum++] = new sphere(vec3f(0, 0, 7), 2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		return new hitableList(list, itemNum);
	}


	// 光线阴影场景
	hitableList *scene2() {
		hitable **list = new hitable*[100];
		int itemNum = 0;
		texture *pertext = new noiseTexture(4);
		list[itemNum++] = new sphere(vec3f(0, 2, 0), 2, new lambertian(pertext));
		list[itemNum++] = new sphere(vec3f(0, 7, 0), 2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		list[itemNum++] = new xyRectangle(3, 5, 1, 3, -2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		return new hitableList(list, itemNum);
	}

	// 康奈尔盒子场景
	hitableList *scene3() {
		hitable **list = new hitable*[100];
		int itemNum = 0;
		material *red = new lambertian(new constantTexture(vec3f(0.65, 0.05, 0.05)));
		material *white = new lambertian(new constantTexture(vec3f(0.73, 0.73, 0.73)));
		material *green = new lambertian(new constantTexture(vec3f(0.12, 0.45, 0.15)));
		material *light = new diffuseLight(new constantTexture(vec3f(4, 4, 4)));
		list[itemNum++] = new flipNormals(new yzRectangle(0, 555, 0, 555, 555, green));
		list[itemNum++] = new yzRectangle(0, 555, 0, 555, 0, red);
		list[itemNum++] = new xzRectangle(123, 423, 147, 412, 554, light);
		list[itemNum++] = new flipNormals(new xzRectangle(0, 555, 0, 555, 555, white));
		list[itemNum++] = new xzRectangle(0, 555, 0, 555, 0, white);
		list[itemNum++] = new flipNormals(new xyRectangle(0, 555, 0, 555, 555, white));
		list[itemNum++] = new translation(new rotateZ(new box(vec3f(20, 20, 20), vec3f(125, 125, 125), white), -18), vec3f(130, 0, 65));
		list[itemNum++] = new translation(new rotateX(new box(vec3f(20, 20, 20), vec3f(125, 270, 125), white), 15), vec3f(265, 0, 295));
		list[itemNum++] = new translation(new rotateY(new box(vec3f(0, 0, 0), vec3f(120, 120, 120), white), 15), vec3f(130, 40, 300));
		hitable *tempBox = new translation(new rotateY(new box(vec3f(0, 0, 0), vec3f(80, 80, 80), new dielectric(1.5)), -18), vec3f(130, 0, 65));
		list[itemNum++] = tempBox;
		list[itemNum++] = new constantMedium(tempBox, 0.2, new constantTexture(vec3f(0.9, 0.9, 0.9)));
		return new hitableList(list, itemNum);
	}

	// 导入模型场景 
	hitableList *scene4(std::string modelFile = MODEL) {
		int fileType = analyzeFileType(modelFile);
		int triNum = countTriNum(modelFile);

		triangle **triList = new triangle*[triNum];
		loadMeshModel(modelFile, new metal(vec3f(0.5, 0.2, 0.1), 0.5), triList);

		hitable **list = new hitable*[triNum];
		for (int i = 0; i < triNum; ++i) {
			list[i] = triList[i];
		}

		return new hitableList(list, triNum);
	}

	// 随机场景
	hitableList *scene5() {
		int nb = 5;
		hitable **list = new hitable*[30];
		hitable **boxlist = new hitable*[1000];
		hitable **boxlist2 = new hitable*[1000];
		material *white = new lambertian(new constantTexture(vec3f(0.73, 0.73, 0.73)));
		material *ground = new lambertian(new constantTexture(vec3f(0.48, 0.83, 0.53)));
		int b = 0;
		for (int i = 0; i < nb; i++) {
			for (int j = 0; j < nb; j++) {
				float w = 10;
				float x0 = -100 + i * w;
				float z0 = -100 + j * w;
				float y0 = 0;
				float x1 = x0 + w;
				float y1 = 10 * (RAND + 0.01);
				float z1 = z0 + w;
				boxlist[b++] = new box(vec3f(x0, y0, z0), vec3f(x1, y1, z1), ground);
			}
		}

		int l = 0;
		list[l++] = new bvhNode(boxlist, b, 0, 1);
		material *light = new diffuseLight(new constantTexture(vec3f(7, 7, 7)));
		list[l++] = new xzRectangle(12, 42, 14, 41, 55, light);
		vec3f center(40, 40, 20);
		list[l++] = new sphere(vec3f(260, 150, 45), 50, new dielectric(1.5));
		list[l++] = new sphere(vec3f(0, 150, 145), 50, new metal(vec3f(0.8, 0.8, 0.9), 10.0));
		hitable *boundary = new sphere(vec3f(360, 150, 145), 70, new dielectric(1.5));
		list[l++] = boundary;
		list[l++] = new constantMedium(boundary, 0.2, new constantTexture(vec3f(0.2, 0.4, 0.9)));
		boundary = new sphere(vec3f(0, 0, 0), 5000, new dielectric(1.5));
		list[l++] = new constantMedium(boundary, 0.0001, new constantTexture(vec3f(1.0, 1.0, 1.0)));

		material *emat = new lambertian(new constantTexture(vec3f(0.2, 0.4, 0.9)));
		list[l++] = new sphere(vec3f(40, 20, 40), 10, emat);
		texture *pertext = new noiseTexture(0.1);
		list[l++] = new sphere(vec3f(22, 28, 30), 8, new lambertian(pertext));
		int ns = 100;
		for (int j = 0; j < ns; j++) {
			boxlist2[j] = new sphere(vec3f(25 * RAND, 25 * RAND, 25 * RAND), 5, white);
		}
		list[l++] = new translation(new rotateY(new bvhNode(boxlist2, ns, 0.0, 1.0), 15), vec3f(-100, 270, 395));
		return new hitableList(list, l);
	}
}

#endif // !SCENES

