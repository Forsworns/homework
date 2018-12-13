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
		virtual bool hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const;
	};
	bool hitableList::hit(const ray<float>& r, float tMin, float tMax, hitRecord& rec) const {
		hitRecord tempRecord;
		bool hitAnything = false;
		float closestHitted = tMax;
		for (int i = 0; i < listSize; ++i) {	// 遍历找到最靠前的，让它被照射到
			if (list[i]->hit(r,tMin, closestHitted, tempRecord)) {
				hitAnything = true;
				closestHitted = tempRecord.t;			// 这里不用比较大小，因为已经更改了最大的检测边界，若还碰撞到t一定是更小的
				rec = tempRecord;
			}
		}
		return hitAnything;
	}
}

#endif // !HITABLE_LIST
