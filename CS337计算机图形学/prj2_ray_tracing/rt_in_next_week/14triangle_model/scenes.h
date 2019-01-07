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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace yph {
	const std::string IMAGE = "models\\watermelon.jpg";
	const std::string MODEL = "models\\cube.obj";
	const float FLOOR_SIZE = 10000;

	hitable *floorInit(const aabb &sceneBox, std::string imageFile = IMAGE, int nx = 400, int ny = 400) {
		int nn;
		unsigned char* imgData = stbi_load(imageFile.c_str(), &nx, &ny, &nn, 0);
		return new sphere(sceneBox.getMin() - vec3f(0, FLOOR_SIZE, 0), FLOOR_SIZE, new lambertian(new imageTexture(imgData, nx, ny)));
	}

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
	

	// 球体场景
	hitableList *scene1(int nx = 400,int ny = 400, std::string imageFile = IMAGE) {
		int nn;
		unsigned char* imgData = stbi_load(imageFile.c_str(), &nx, &ny, &nn, 0);
		hitable **list = new hitable*[100];
		int itemNum = 0;
		list[itemNum++] = new sphere();
		list[itemNum++] = new sphere(vec3f(5, 0, 12), 2, new lambertian(new constantTexture(vec3f(0.6, 0.3, 0.2))));
		list[itemNum++] = new sphere(vec3f(8, 0, 0), 2, new lambertian(new noiseTexture(4)));
		list[itemNum++] = new sphere(vec3f(0, 0, 8), 4, new lambertian(new imageTexture(imgData, nx, ny)));
		list[itemNum++] = new sphere(vec3f(13, 0, 4), 2, new metal(vec3f(0.2,0.5,0.3),1.0));
		list[itemNum++] = new sphere(vec3f(5, 0, 17), 2, new metal(vec3f(0.3,0.4,0.6),0.5));
		list[itemNum++] = new sphere(vec3f(1, 0, 2), 4, new dielectric(1.5));
		list[itemNum++] = new sphere(vec3f(1, 0, 2), -3.9, new dielectric(1.5));
		return new hitableList(list, itemNum);
	}


	// 光线阴影场景
	hitableList *scene2() {
		hitable **list = new hitable*[100];
		int itemNum = 0;
		texture *pertext = new noiseTexture(4);
		list[itemNum++] = new sphere(vec3f(0, 2, 1), 2, new lambertian(pertext));
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
		list[itemNum++] = new translation(new rotateZ(new box(vec3f(20, 20, 20), vec3f(125, 125, 125), white), -18), vec3f(65, 0, 65));
		list[itemNum++] = new translation(new rotateX(new box(vec3f(20, 20, 20), vec3f(125, 270, 125), white), 15), vec3f(265, 0, 295));
		list[itemNum++] = new translation(new rotateY(new box(vec3f(0, 0, 0), vec3f(120, 120, 120), white), 15), vec3f(130, 40, 300));
		hitable *tempBox = new translation(new rotateY(new box(vec3f(0, 0, 0), vec3f(160, 160, 160), new dielectric(1.5)), -18), vec3f(20, 0, 65));
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

}

#endif // !SCENES

