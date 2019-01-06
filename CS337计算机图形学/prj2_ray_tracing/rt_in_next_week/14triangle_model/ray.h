#ifndef RAY
#define RAY
#include "vec3.h"

namespace yph {
	constexpr auto PI = 3.1415926;
	// 光线
	template<class T>
	class ray {
	private:
		vec3<T> originPoint;		// 起点位置
		vec3<T> direcition;			// 目标方向
	public:
		bool isSurface;					// 照射是里还是外，在hit时翻转bool值，对三角形比较重要
		ray() {}
		ray(const vec3<T>& A, const vec3<T>& B) {
			originPoint = A;
			direcition = B;
		}
		vec3<T> getOrigin() const {
			return originPoint;
		}
		vec3<T> getDirection() const {
			return direcition;
		}
		vec3<T> getPosWithPara(T t) const {
			return originPoint + t * direcition;	
		}
		~ray() {}
	};
}

#endif // !RAY
