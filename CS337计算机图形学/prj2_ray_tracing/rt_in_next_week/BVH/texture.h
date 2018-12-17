#ifndef TEXTURE
#define TEXTURE

#include "vec3.h"
#include "perlin.h"

namespace yph {
	// ����
	class texture {
	public:
		virtual vec3f value(float u, float v, const vec3f& pos) const = 0;
	};
	// �̶���һ��ɫ
	class constantTexture :public texture {
	private:
		vec3f color;
	public:
		constantTexture() {}
		constantTexture(vec3f c) :color(c){}
		virtual vec3f value(float u, float v, const vec3f& pos) const { return color; }
	};
	// ���̲���
	class checkerTexture :public texture {
	private:
		texture* even;
		texture* odd;
	public:
		checkerTexture() {}
		// ��̬�ԣ�ʵ�ʳ�ʼ��ʱ��Ҫ��texture�࣬�ü̳к������
		checkerTexture(texture *t1,texture *t2) :even(t1),odd(t2){}
		virtual vec3f value(float u,float v,const vec3f& pos) const {
			// �������Ǻ���������
			float flag = sin(pos.getX())*sin(pos.getY())*sin(pos.getZ());
			if (flag < 0) {
				return odd->value(u, v, pos);
			}
			else {
				return even->value(u, v, pos);
			}
		}
	};
	class noiseTexture : public texture{
	private:
		perlin perl;
		float scale;
	public:
		noiseTexture(float s) : scale(s){}
		virtual vec3f value(float u, float v, const vec3f& pos) const {
//			return vec3f(1, 1, 1)*perl.noise(scale * pos);	// ɫ������
//			return vec3f(1, 1, 1)*perl.turb(scale * pos);	// ��������״
//			return vec3f(1, 1, 1)*0.5*(1 + perl.turb(pos*scale)); // �⻬����ʯ
			return vec3f(1, 1, 1)*0.5*(1 + sin(scale*pos.getZ() + 10 * perl.turb(pos))); // ����ʯ 
		}
	};

	// ���ǽ���������ӳ�䵽ͼƬ�����ǽ�ͼƬ����ӳ�䵽����
	class imageTexture : public texture {
	private:
		unsigned char *data;
		int nx, ny;
	public:
		imageTexture() {}
		imageTexture(unsigned char* pixels,int x,int y) : data(pixels), nx(x), ny(y) {}
		virtual vec3f value(float u, float v, const vec3f& pos) const {
			int i = u * nx;
			int j = (1 - v)*ny - 0.001;
			if (i < 0) { i = 0; }
			if (i > nx - 1) { i = nx - 1; }
			if (j < 0) { j = 0; }
			if (j > ny - 1) { j = ny - 1; }
			float r = int(data[3 * i + 3 * nx*j]) / 255.0;
			float g = int(data[3 * i + 3 * nx*j + 1]) / 255.0;
			float b = int(data[3 * i + 3 * nx*j + 2]) / 255.0;
			return vec3f(r, g, b);
		}
	};
};
#endif // !TEXTURE
