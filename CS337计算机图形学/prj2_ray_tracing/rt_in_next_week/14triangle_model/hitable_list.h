#pragma once
#ifndef HITABLE_LIST
#define HITABLE_LIST
#include "hitable.h"
namespace yph{
	class hitableList :public hitable {
	private:
		int listSize;
		hitable **list;
	public:
		hitableList() {}
		hitableList(hitable **l,int size) {
			listSize = size;
			list = l;
		}
		void push_back(hitable *item) {
			hitable **tempList = new hitable*[listSize++];
			for (int i = 0; i < listSize - 1; ++i) {
				tempList[i] = list[i];
			}
			tempList[listSize - 1] = item;
			list = new hitable*[listSize];
			for (int i = 0; i < listSize; ++i) {
				list[i] = tempList[i];
			}
		}
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
		virtual bool boundingBox(float t0,float t1,aabb& box) const;
	};
	bool hitableList::hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		hitRecord tempRecord;
		bool hitAnything = false;
		float closestHitted = tMax;
		for (int i = 0; i < listSize; ++i) {			// 遍历找到最靠前的，让它被照射到
			if (list[i]->hit(r,tMin, closestHitted, tempRecord)) {
				hitAnything = true;
				closestHitted = tempRecord.t;			// 这里不用比较大小，因为已经更改了最大的检测边界，若还碰撞到t一定是更小的
				rec = tempRecord;
			}
		}
		return hitAnything;
	}
	bool hitableList::boundingBox(float t0, float t1, aabb& box) const {
		// 遍历整个列表得到其中所有物体共同的aabb包围盒
		if (listSize < 1) return false;
		aabb tempBox;
		if (list[0]->boundingBox(t0,t1,tempBox)) {
			// 构造第一个的aabb box存到temp box中
			box = tempBox;
		}
		else {
			return false;
		}
		for (int i = 1; i < listSize;++i) {
			// 之后的物体需要和之前的合并
			if (list[i]->boundingBox(t0, t1, tempBox)) {
				box = mergeAabb(tempBox, box);
			}
			else {
				return false;
			}
		}
		return true;
	}
}

#endif // !HITABLE_LIST
