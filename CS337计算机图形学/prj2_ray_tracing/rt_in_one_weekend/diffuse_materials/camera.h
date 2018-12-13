#pragma once
#ifndef CAMERA
#define CAMERA

#include "ray.h"

namespace yph {
	class camera {
	private:
		vec3<float> origin;
		vec3<float> lowerLeftCorner;
		vec3<float> vertical;
		vec3<float> horizontal;
	public:
		camera() {
			lowerLeftCorner = vec3<float>(-2.0, -1.0, -1.0);
			horizontal = vec3<float>(4.0, 0.0, 0.0);
			vertical = vec3<float>(0.0, 2.0, 0.0);
			origin = vec3<float>(0.0, 0.0, 0.0);
		}
		ray<float> getRay(float x,float y) {
			return ray<float>(origin, lowerLeftCorner + x * horizontal + y * vertical - origin);
		}
	};
}

#endif