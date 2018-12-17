#ifndef AABB
#define AABB

#include "ray.h"

namespace yph {
	inline float ffmax(float a, float b) { return a > b ? a : b; }
	inline float ffmin(float a, float b) { return a > b ? b : a; }
	
	class aabb {
	private:
		vec3f _min;
		vec3f _max;
	public:
		aabb() {}
		aabb(const vec3f& a,const vec3f&b) :_min(a),_max(b){}
		vec3f getMin() { return _min; }
		vec3f getMax() { return _max; }
		bool hit(ray<float> r,float tmin,float tmax) const {
			//在三个维度判断光线和包围盒的交点，然后计算overlap部分是否合法
			// 一个trick就是有NAN的判断一定是false，所以不用单另处理NAN
			for (int i = 0; i < 3;++i) {
				float t0 = ffmin((_min[i]-r.getOrign()[i])/r.getDirection()[i], (_max[i] - r.getOrign()[i]) / r.getDirection()[i]);
				float t1 = ffmax((_min[i] - r.getOrign()[i]) / r.getDirection()[i], (_max[i] - r.getOrign()[i]) / r.getDirection()[i]);
				tmin = ffmax(tmin, t0);
				tmax = ffmin(tmax, t1);
				if(tmin >= tmax) return false;
			}
			return true;
		}
	};

	aabb mergeAabb(aabb &box1, aabb &box2) {
		vec3f lowLeft(ffmin(box1.getMin().getX(), box2.getMin().getX()),
			ffmin(box1.getMin().getY(), box2.getMin().getY()),
			ffmin(box1.getMin().getZ(), box2.getMin().getZ()));
		vec3f highRight(ffmax(box1.getMax().getX(), box2.getMax().getX()),
			ffmax(box1.getMax().getY(), box2.getMax().getY()),
			ffmax(box1.getMax().getZ(), box2.getMax().getZ()));
		return aabb(lowLeft, highRight);
	}
};

#endif // !AABB
