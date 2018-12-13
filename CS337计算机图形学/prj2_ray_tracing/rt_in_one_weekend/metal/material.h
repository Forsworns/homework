#pragma once
#ifndef MATERIAL
#define MATERIAL
#include "ray.h"

namespace yph {
	struct hitRecord;

	class material {
	protected:
		vec3<float> randomInUnitSphere() const {	//���ȡ���ڵ�λ����Ϊ���䵽�ĵ�ĵ�λ��������Ϊ�뾶ȷ���ĵ�λԲ
			vec3<float> p;
			do {
				p = vec3<float>((rand() % 10) / float(10), (rand() % 10) / float(10), (rand() % 10) / float(10));
				//�Ӱ�Χ�����cube��ѡȡһ���㣬����������򷵻ظõ�
			} while (p.squaredLength() >= 1.0f);
			return p;
		}

		vec3<float> reflect(vec3<float> rayIn, vec3<float> normal) const {	//����
			return rayIn + (-2 * dot(rayIn, normal) * normal);			// dot����������ڷ���������ĳ���ͶӰ����
		}

		// ���ݽǶȲ�ͬ��һ������ĸ���
		float schlick(float cosine, float refractionRadio) const {
			float r0 = (1 - refractionRadio) / (1 + refractionRadio);
			r0 = r0 * r0;
			return r0 + (1 - r0)*pow((1 - cosine), 5);
		}

		bool refract(vec3<float> rIn, vec3<float> &rOut, vec3<float> normal, float nInOverOut) const { //����
			// normalΪ�����ķ�������nInOverOutΪ�����ʵı�ֵ����������䣩�ȼ���sin(theta')/sin(theta)
			vec3<float> rInUnit = rIn.makeUnitVector();
			float cosTheta = dot(rInUnit, normal);		//��λ�����˻���Ϊ����ǵ�����ֵ
			float discriminant = 1 - nInOverOut * nInOverOut*(1 - cosTheta * cosTheta);	//��������cosTheta'����������ң���ƽ��
			if (discriminant>0) {
				rOut = nInOverOut * (rInUnit - normal * cosTheta) - normal * sqrt(discriminant);	// ����������ߣ�ǰһ��Ϊˮƽ��������һ��Ϊ��ֱ����������ü�������Ϊ����������
				return true;
			}
			else {
				return false;
			}
		}
	public:
		virtual bool scatter(const ray<float>& rIn,const hitRecord& rec, vec3<float>& attenuation,ray<float>& scattered)const = 0;
	};

	class lambertian : public material {	// ����ɢ��
	private:
		vec3<float> albedo;					// ������
	public:
		lambertian() {}
		lambertian(const vec3<float>& a) : albedo(a) {}
		virtual bool scatter(const ray<float>& rIn, const hitRecord& rec, vec3<float>& attenuation, ray<float>& scattered)const {
			vec3<float> target = rec.p + rec.normal + randomInUnitSphere();	// p+nȷ��������������ģ��ڸ����ĵĵ�λ������ѡһ�㶨Ϊtarget
			scattered = ray<float>(rec.p,target-rec.p);
			attenuation = albedo;
			return true;
		}
	};

	class metal : public material {	// �������淴��
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
			vec3<float> reflected = reflect(rIn.getDirection(), rec.normal); // �������
			float niOverNt;	// n_i/n_t
			attenuation = vec3<float>(1.0, 1.0, 1.0);
			vec3<float> refracted; // �������
			float reflectProb;
			float cosine; // ���������ֵ
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
			// �����Է����������������ĸ���
				reflectProb = schlick(cosine, refractionRadio);
			}
			else {
			// ����ֻ���Ƿ���
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
