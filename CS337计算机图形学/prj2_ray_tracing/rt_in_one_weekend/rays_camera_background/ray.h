#ifndef RAY
#define RAY
#include "vec3.h"

namespace yph {
	// ����
	template<class T>
	class ray {
	private:
		vec3<T> originPoint;		// ���λ��
		vec3<T> direcition;			// Ŀ�귽��
	public:
		ray(const vec3<T>& A, const vec3<T>& B) {
			originPoint = A;
			direcition = B;
		}
		vec3<T> getOrign() const {
			return originPoint;
		}
		vec3<T> getDirection() const {
			return direcition;
		}
		vec3<T> getPosWithPara(T t) {
			return originPoint + t * direcition;	
		}
		~ray() {}
	};

}

#endif // !RAY
