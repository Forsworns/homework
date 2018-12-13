#pragma once
#ifndef MATERIAL
#define MATERIAL
#include "ray.h"

namespace yph {
	struct hitRecord;

	class material {
	protected:
		vec3<float> randomInUnitSphere() const {	//随机取点于单位球，球为照射到的点的单位法向量作为半径确定的单位圆
			vec3<float> p;
			do {
				p = vec3<float>((rand() % 10) / float(10), (rand() % 10) / float(10), (rand() % 10) / float(10));
				//从包围着球的cube中选取一个点，如果在球内则返回该点
			} while (p.squaredLength() >= 1.0f);
			return p;
		}

		vec3<float> reflect(vec3<float> rayIn, vec3<float> normal) const {	//反射
			return rayIn + (-2 * dot(rayIn, normal) * normal);			// dot将入射光线在法向量方向的长度投影出来
		}

		// 根据角度不同给一个折射的概率
		float schlick(float cosine, float refractionRadio) const {
			float r0 = (1 - refractionRadio) / (1 + refractionRadio);
			r0 = r0 * r0;
			return r0 + (1 - r0)*pow((1 - cosine), 5);
		}

		bool refract(vec3<float> rIn, vec3<float> &rOut, vec3<float> normal, float nInOverOut) const { //折射
			// normal为折射点的法向量，nInOverOut为折射率的比值（入射比折射）等价于sin(theta')/sin(theta)
			vec3<float> rInUnit = rIn.makeUnitVector();
			float cosTheta = dot(rInUnit, normal);		//单位向量乘积即为入射角的余弦值
			float discriminant = 1 - nInOverOut * nInOverOut*(1 - cosTheta * cosTheta);	//理论上是cosTheta'（折射角余弦）的平方
			if (discriminant>0) {
				rOut = nInOverOut * (rInUnit - normal * cosTheta) - normal * sqrt(discriminant);	// 构造折射光线，前一半为水平分量，后一半为竖直方向分量，用减号是因为法向量反向
				return true;
			}
			else {
				return false;
			}
		}
	public:
		virtual bool scatter(const ray<float>& rIn,const hitRecord& rec, vec3<float>& attenuation,ray<float>& scattered)const = 0;
	};

	class lambertian : public material {	// 理想散射
	private:
		vec3<float> albedo;					// 反射率
	public:
		lambertian() {}
		lambertian(const vec3<float>& a) : albedo(a) {}
		virtual bool scatter(const ray<float>& rIn, const hitRecord& rec, vec3<float>& attenuation, ray<float>& scattered)const {
			vec3<float> target = rec.p + rec.normal + randomInUnitSphere();	// p+n确定的是球体的球心，在该球心的单位球体中选一点定为target
			scattered = ray<float>(rec.p,target-rec.p);
			attenuation = albedo;
			return true;
		}
	};

	class metal : public material {	// 金属镜面反射
	private:
		vec3<float> albedo;
		float fuzz;
	public:
		metal() {}
		metal(const vec3<float>& a) : albedo(a),fuzz(0) {}
		metal(const vec3<float>& a, float fuzzier) : albedo(a) {
			if (fuzzier > 1) {
				fuzz = 1;
			}
			else {
				fuzz = fuzzier;
			}
		}
		virtual bool scatter(const ray<float>& rIn, const hitRecord& rec, vec3<float>& attenuation, ray<float>& scattered)const {
			vec3<float> direction = reflect(rIn.getDirection(),rec.normal);
			scattered = ray<float>(rec.p,direction + fuzz * randomInUnitSphere());
			attenuation = albedo;
			return dot(direction,rec.normal)>0;
		}
	};

	// if dielectric sphere is with a negative radius, the surface normal is inward, so it becomes a bubble
	class dielectric : public material {
	private:
		float refractionRadio;
	public:
		dielectric() {}
		dielectric(float ref) : refractionRadio(ref){}
		virtual bool scatter(const ray<float>& rIn, const hitRecord& rec, vec3<float>& attenuation, ray<float>& scattered)const {
			vec3<float> outwardNormal;
			vec3<float> reflected = reflect(rIn.getDirection(), rec.normal); // 反射光线
			float niOverNt;	// n_i/n_t
			attenuation = vec3<float>(1.0, 1.0, 1.0);
			vec3<float> refracted; // 折射光线
			float reflectProb;
			float cosine; // 入射角余弦值
			if (dot(rIn.getDirection(),rec.normal)>0) {
				outwardNormal = -rec.normal;
				niOverNt = refractionRadio;
				cosine = refractionRadio * dot(rIn.getDirection(), rec.normal) / rIn.getDirection().length();
			}
			else {
				outwardNormal = rec.normal;
				niOverNt = 1.0 / refractionRadio;
				cosine = -dot(rIn.getDirection(), rec.normal) / rIn.getDirection().length();
			}
			if (refract(rIn.getDirection(), refracted, outwardNormal, niOverNt)) {
			// 若可以发生折射则计算折射的概率
				reflectProb = schlick(cosine, refractionRadio);
			}
			else {
			// 否则只考虑反射
				scattered = ray<float>(rec.p, reflected);
				reflectProb = 1;
			}
			if ((rand()%10)/10.0 < reflectProb) {
				scattered = ray<float>(rec.p, reflected);
			}
			else {
				scattered = ray<float>(rec.p, refracted);
			}
			return true;
		}
	};
}

#endif // !MATERIAL
