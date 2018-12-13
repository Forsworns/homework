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
		for (int i = 0; i < listSize; ++i) {	// �����ҵ��ǰ�ģ����������䵽
			if (list[i]->hit(r,tMin, closestHitted, tempRecord)) {
				hitAnything = true;
				closestHitted = tempRecord.t;			// ���ﲻ�ñȽϴ�С����Ϊ�Ѿ����������ļ��߽磬������ײ��tһ���Ǹ�С��
				rec = tempRecord;
			}
		}
		return hitAnything;
	}
}

#endif // !HITABLE_LIST
