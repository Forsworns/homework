#ifndef BVH
#define BVH

#include "ray.h"
#include "hitable.h"
// 利用aabb做二叉树，加速碰撞检测
namespace yph {
	#define RAND ((rand()%10)/10.0)

	int boxCompareX(const void *a,const void *b) {
		aabb boxLeft, boxRight;
		hitable *ah = *(hitable**)a;
		hitable *bh = *(hitable**)b;
		if (ah->boundingBox(0,0,boxLeft)&&bh->boundingBox(0,0,boxRight)) {
			if (boxLeft.getMin().getX()<boxRight.getMin().getX()) {
				return -1;
			}
			else {
				return 1;
			}
		}
	}

	int boxCompareY(const void *a, const void *b) {
		aabb boxLeft, boxRight;
		hitable *ah = *(hitable**)a;
		hitable *bh = *(hitable**)b;
		if (ah->boundingBox(0, 0, boxLeft) && bh->boundingBox(0, 0, boxRight)) {
			if (boxLeft.getMin().getY() < boxRight.getMin().getY()) {
				return -1;
			}
			else {
				return 1;
			}
		}
	}

	int boxCompareZ(const void *a, const void *b) {
		aabb boxLeft, boxRight;
		hitable *ah = *(hitable**)a;
		hitable *bh = *(hitable**)b;
		if (ah->boundingBox(0, 0, boxLeft) && bh->boundingBox(0, 0, boxRight)) {
			if (boxLeft.getMin().getZ() < boxRight.getMin().getZ()) {
				return -1;
			}
			else {
				return 1;
			}
		}
	}


	class bvhNode :public hitable {
	private:
		aabb _box;
		hitable *left;
		hitable *right;
	public:
		bvhNode() {}
		bvhNode(hitable **list, int n, float time0, float time1);
		virtual bool hit(ray<float> &r,float tMin,float tMax,hitRecord &rec ) const;
		virtual bool boundingBox(float tMin,float tMax,aabb &box) const;
	};
	bvhNode::bvhNode(hitable **list, int n, float time0, float time1) {
		// 随机选取一个轴向，排序
		int axis = int(3 * RAND);
		if (axis == 0) {
			qsort(list, n, sizeof(hitable *), boxCompareX);
		}
		else if (axis == 1) {
			qsort(list, n, sizeof(hitable *), boxCompareY);
		}
		else {
			qsort(list, n, sizeof(hitable *), boxCompareZ);
		}
		// 分治，递归构造出树
		if (n == 1) {
			left = right = list[0];
		}
		else if (n == 2) {
			left = list[0];
			right = list[1];
		}
		else {
			left = new bvhNode(list,n/2, time0, time1);
			right = new bvhNode(list+n/2, n - n / 2, time0, time1);
		}
		aabb boxLeft, boxRight;
		if (left->boundingBox(time0, time1, boxLeft) && right->boundingBox(time0, time1, boxRight)) {
			_box = mergeAabb(boxLeft, boxRight);
		}
	}
	bool bvhNode::hit(ray<float> &r, float tMin, float tMax, hitRecord &rec) const {
		if (_box.hit(r, tMin, tMax)) {
			hitRecord leftRec, rightRec;
			bool hitLeft = left->hit(r,tMin,tMax,leftRec);
			bool hitRight = right->hit(r, tMin, tMax, rightRec);
			if (hitLeft&&hitRight) {
				if (leftRec.t<rightRec.t) {
					// 左边子树先碰撞到的光线，取左边
					rec = leftRec;
				}
				else {
					rec = rightRec;
				}
				return true;
			}
			else if (hitLeft) {
				rec = leftRec;
				return true;
			}
			else if (hitRight) {
				rec = rightRec;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	bool bvhNode::boundingBox(float tMin, float tMax, aabb &box) const {
		box = _box;
		return true;
	}
}

#endif // !BVH

