#ifndef PERLIN
#define PERLIN

#include "vec3.h"
#define RAND ((rand() % 10) / 10.0)
namespace yph{
	inline float trilinearInterp(vec3f c[2][2][2],float u,float v,float w) {
		float accum = 0;
		// 用hermite cubic 优化插值
		float uu = u * u*(3 - 2 * u);
		float vv = v * v*(3 - 2 * v);
		float ww = w * w*(3 - 2 * w);
		for (int i = 0; i < 2;++i) {
			for (int j = 0; j < 2;++j) {
				for (int k = 0; k < 2;++k) {
					vec3f weight(u-i,v-j,w-k);
					accum += (i*uu + (1 - i)*(1 - uu))*
							(j*vv + (1 - j)*(1 - vv))*
							(k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k],weight);
				}
			}
		}
		return accum;
	}

	class perlin {
	public:
		static vec3f *ranvec;
		static int *permX;
		static int *permY;
		static int *permZ;

		float noise(const vec3f& pos) const {
			float u = pos.getX() - floor(pos.getX());
			float v = pos.getY() - floor(pos.getY());
			float w = pos.getZ() - floor(pos.getZ()); // 描述了更靠近哪个(i,j,k)位置
			int i = floor(pos.getX());
			int j = floor(pos.getY());
			int k = floor(pos.getZ());
			vec3f c[2][2][2]; // 取附近八个块做平均
			for (int di = 0; di < 2;++di) {
				for (int dj = 0; dj < 2; ++dj) {
					for (int dk = 0; dk < 2; ++dk) {
						c[di][dj][dk] = ranvec[permX[(i + di) & 255]^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]];
					}
				}
			}
			return trilinearInterp(c, u, v, w); // 该位置经附近几个平均后的结果
		}

		float turb(const vec3f &pos, int depth = 7) const {
			float accum = 0;
			vec3f tempPos = pos;
			float weight = 1.0;
			for (int i = 0; i < depth; ++i) {
				accum += weight * noise(tempPos);
				tempPos *= 2;
				weight *= 0.5;
			}
			return fabs(accum);
		}
	};

	static vec3f *generatePerlin() {
		vec3f *p = new vec3f[256];
		for (int i = 0; i < 256;++i) {
			p[i] = vec3f(RAND * 2 - 1, RAND * 2 - 1, RAND * 2 - 1).makeUnitVector();
		}

		return p;
	}

	static void permute(int *p,int n) {
		for (int i = 0; i < n;++i) {
			int idx = int(rand() % n);
			float temp = p[idx];
			p[idx] = p[i];
			p[i] = temp;
		}
	}
	
	static int *generatePerlinPerm() {
		int *p = new int[256];
		for (int i = 0; i < 256; ++i) {
			p[i] = i;
		}
		permute(p, 256);
		return p;
	}

	vec3f *perlin::ranvec = generatePerlin();
	int *perlin::permX = generatePerlinPerm();
	int *perlin::permY = generatePerlinPerm();
	int *perlin::permZ = generatePerlinPerm();
};

#endif // !PERLIN

