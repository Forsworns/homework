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
		for (int i = 0; i < listSize; ++i) {			// �����ҵ��ǰ�ģ����������䵽
			if (list[i]->hit(r,tMin, closestHitted, tempRecord)) {
				hitAnything = true;
				closestHitted = tempRecord.t;			// ���ﲻ�ñȽϴ�С����Ϊ�Ѿ����������ļ��߽磬������ײ��tһ���Ǹ�С��
				rec = tempRecord;
			}
		}
		return hitAnything;
	}
	bool hitableList::boundingBox(float t0, float t1, aabb& box) const {
		// ���������б�õ������������干ͬ��aabb��Χ��
		if (listSize < 1) return false;
		aabb tempBox;
		if (list[0]->boundingBox(t0,t1,tempBox)) {
			// �����һ����aabb box�浽temp box��
			box = tempBox;
		}
		else {
			return false;
		}
		for (int i = 1; i < listSize;++i) {
			// ֮���������Ҫ��֮ǰ�ĺϲ�
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
