#pragma once

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H
#include "Vector.hpp"

enum  MaterialType
{
	DIFFUSE , REFLC , MIRCO
};

class Material
{
private:

	float D_GGX(Vector3f &h, float &r, const  Vector3f& N)
	{
		float r2 = r * r;
		float NdotH = clamp(0.0, 1.0, dotProduct(N, h));
		float NdotH2 = NdotH * NdotH;
		float res = r2 / ((M_PI * (NdotH2 * (r2 - 1) + 1) * (NdotH2 * (r2 - 1) + 1))) + 0.0001;
		return res;
	}

	float G_s(float &NdotV, float &a)
	{
		float k = (a + 1) * (a + 1) / 8;
		return NdotV / (NdotV * (1 - k) + k);
	}

	float G(const Vector3f &i, const Vector3f &o, const Vector3f &N, float &a)
	{
		float NdotI = clamp(0.0, 1.0, dotProduct(N, i));
		float NdotO = clamp(0.0, 1.0, dotProduct(N, o));
		return G_s(NdotI, a) * G_s(NdotO, a);
	}
	//games101 7
	Vector3f toWorld(const Vector3f& a, const Vector3f& N) {
		Vector3f B, C;
		if (std::fabs(N.x) > std::fabs(N.y)) {
			float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
			C = Vector3f(N.z * invLen, 0.0f, -N.x * invLen);
		}
		else {
			float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
			C = Vector3f(0.0f, N.z * invLen, -N.y * invLen);
		}
		B = crossProduct(C, N);
		return(B * a.x) + (C * a.y) + (N * a.z);
	}

	Vector3f reflect(const Vector3f &in, const Vector3f &N)
	{
		return (in * -1 + ((N * dotProduct(in, N)) * 2.0)).normalized();
	}

	void fresnel(const Vector3f &in, const Vector3f &N, float &ior, float &kr) //kr是反射的比例
	{
		in.normalized();
		N.normalized();
		float Ro = ((1.0 - ior) / (1.0 + ior)) * ((1.0 - ior) / (1.0 + ior));
		float costheta = clamp( 0.0f , 1.0 ,  dotProduct(N, in));
		float t = (1.0 - costheta) * (1.0 - costheta) * (1.0 - costheta) * (1.0 - costheta) * (1.0 - costheta);
		kr = Ro + (1 - Ro) * t;
		return;
	}

public:
	Material( Vector3f kd, MaterialType  t) :
		Kd(kd),
		mtype(t)
	{}
	MaterialType mtype;
	Vector3f Kd;
	Vector3f lightIntensity;
	float ior; //这个材质的折射率
	bool islight;
	float roughness;


	void SetLight(const Vector3f &light_intensity)
	{
		this->islight = true;
		this->lightIntensity = light_intensity;
	}


	Vector3f GetFutureDir(const Vector3f &wi, const Vector3f &N)
	{
		switch (this->mtype)
		{
		case MIRCO:
		case DIFFUSE:
		{
			// uniform sample on the hemisphere
			float x_1 = RandomFloat(), x_2 = RandomFloat();
			float z = std::fabs(1.0f - 2.0f * x_1);
			float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
			Vector3f localRay(r * std::cos(phi), r * std::sin(phi), z);
			return toWorld(localRay, N);
			break;
		}
		case REFLC :
		{
			Vector3f localRay = reflect(wi, N);
			return localRay;
			break;
		}
		default:
			break;
		}
	}

	float pdf(const Vector3f &wi, const Vector3f &wo, const Vector3f &N)
	{
		switch (this->mtype)
		{
		case MIRCO:
		case DIFFUSE:
		{
			float ans = dotProduct(wo, N);
			if (ans > 0.0f)
			{
				return 0.5f / M_PI;
			}
			return 0.0f;
			break;
		}
		case REFLC:
		{
			float ans = dotProduct(wo, N);
			if (ans > 0.0)
			{
				return 1.0;
			}
			return 0.0f;
			break;
		}
		default:
			break;
		}
	}




	Vector3f GetBRDF(const Vector3f &wi, const Vector3f &wo, const Vector3f &N)
	{
		switch (this->mtype)
		{
		case DIFFUSE:
		{
			float ans = dotProduct(wo, N);
			if (ans > 0.0f)
			{
				Vector3f res = Kd / M_PI;
				return res ;
			}
			return Vector3f(0.0f);
			break;
		}
		case REFLC:
		{
			float ans = dotProduct(wo, N);
			if (ans > 0.0f)
			{
				Vector3f res = 1.0 / ans;
				//fresnel(wi, N, ior, K);
				res = res * Kd;
				return res;
			}
			return Vector3f(0);
			break;
		}
		case MIRCO:
		{
			float cosalpha = dotProduct(N, wo); //wo是观测方向
			if (cosalpha > 0.0f) {
				Vector3f h = (wi + wo).normalized();
				float F = 0.f;
				fresnel(wi, N, ior, F);
				float down = 4 * fabs(clamp(0.0, 1.0, dotProduct(N, wi)) * clamp(0.0, 1.0, dotProduct(N, wi))) + 0.00001;
				float up = F * G(wi, wo, N, roughness) * D_GGX(h, roughness, N); //F * G * D
				return  Kd * (up / down); //加上Kd就实现了颜色，当然主函数记得给材质设置Kd
			}
			else
				return Vector3f(0.0f);
			break;


		}
		default:
			break;
		}
	}

};
#endif

