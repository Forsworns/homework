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
	hitable *floorInit(const aabb &sceneBox) {
		return new sphere(sceneBox.getMin() - vec3f(0, 10000, 0), 10000.0f, new lambertian(new checkerTexture(new constantTexture(vec3f(0.2,0.6,0.2)),new constantTexture(vec3f(1,1,1)))));	// 棋盘地板
		// return new sphere(sceneBox.getMin() - vec3f(0, 10000, 0), 10000.0f, new lambertian(new constantTexture(vec3f(0.2,0.6,0.2))));													// 纯色地板
	}

	// 不同材质球体的场景
	hitableList *scene1() {
		hitable **list = new hitable*[100];
		int itemNum = 0;
		list[itemNum++] = new sphere();
		list[itemNum++] = new sphere(vec3f(0, 2, 0), 2, new lambertian());
		list[itemNum++] = new sphere(vec3f(0, 7, 0), 2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		list[itemNum++] = new xyRectangle(3, 5, 1, 3, -2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		return new hitableList(list, itemNum);
	}

	// 正方形场景
	hitableList *scene2() {
		hitable **list = new hitable*[100];
		int itemNum = 0;
		list[itemNum++] = new sphere();
		list[itemNum++] = new sphere(vec3f(0, 2, 0), 2, new lambertian());
		list[itemNum++] = new sphere(vec3f(0, 7, 0), 2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		list[itemNum++] = new xyRectangle(3, 5, 1, 3, -2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		return new hitableList(list, itemNum);
	}


	// 光线阴影场景
	hitableList *scene3() {
		texture *pertext = new noiseTexture(4);
		hitable **list = new hitable*[4];
		list[0] = new sphere(vec3f(0, -1000, 0), 1000, new lambertian(pertext));
		list[1] = new sphere(vec3f(0, 2, 0), 2, new lambertian(pertext));
		list[2] = new sphere(vec3f(0, 7, 0), 2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		list[3] = new xyRectangle(3, 5, 1, 3, -2, new diffuseLight(new constantTexture(vec3f(4, 4, 4))));
		return new hitableList(list, 4);
	}

	// 图片材质映射
	hitableList *scene4() {
	
	}

	// 康奈尔盒子场景
	hitableList *scene5() {
		hitable **list = new hitable*[30];
		int i = 0;
		material *red = new lambertian(new constantTexture(vec3f(0.65, 0.05, 0.05)));
		material *white = new lambertian(new constantTexture(vec3f(0.73, 0.73, 0.73)));
		material *green = new lambertian(new constantTexture(vec3f(0.12, 0.45, 0.15)));
		material *light = new diffuseLight(new constantTexture(vec3f(7, 7, 7)));
		//list[i++] = new sphere(vec3f(260, 50, 145), 50,mat);
		list[i++] = new flipNormals(new yzRectangle(0, 555, 0, 555, 555, green));
		list[i++] = new yzRectangle(0, 555, 0, 555, 0, red);
		list[i++] = new xzRectangle(123, 423, 147, 412, 554, light);
		list[i++] = new flipNormals(new xzRectangle(0, 555, 0, 555, 555, white));
		list[i++] = new xzRectangle(0, 555, 0, 555, 0, white);
		list[i++] = new flipNormals(new xyRectangle(0, 555, 0, 555, 555, white));
		list[i++] = new translation(new rotateZ(new box(vec3f(0, 0, 0), vec3f(165, 165, 165), white), -18), vec3f(130, 0, 65));
		list[i++] = new translation(new rotateX(new box(vec3f(0, 0, 0), vec3f(165, 330, 165), white), 15), vec3f(265, 0, 295));
		return new hitableList(list, i);
	}

	// 导入模型场景 
	hitableList *scene6() {
		std::string modelFile = "C:\\Users\\Kyrie\\Desktop\\rt_1\\models\\dinosaur.obj";
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

