#pragma once

#include"Scene.hpp"
#include "Tool.hpp"
#include <cstdio>


void Scene::sampleLight(HitPoint &hp, float &pdf) const
{
	float gs = RandomFloat();
	if (gs == 1)gs = 0.9;
	int aimidx = gs * objs.size();
	objs[aimidx]->SampleLight(hp, pdf);
	return;
}

void Scene::FindHit( Ray &ray ,  HitPoint &hp) const
{
	//Post - update of BVH acceleration structure
	for (int i = 0; i < objs.size(); i++)
	{
		//std::cout << "遍历每一个物体ing " << std::endl;
		objs[i]->getHitPoint(ray , hp);
		if (hp.happened == false)continue;
		else break;
	}
	//std::cout << " 遍历结束 " << std::endl;
	return;
}

void Scene::Add( Object *t) { 
	objs.push_back(t); 
}



//Path Tracing
Vector3f Scene::PathTracing( Ray &ray, int depth)const
{
	Vector3f L_dir(0.0), L_indir(0.0);
	//if (depth >= 1)return L_dir; 

	HitPoint hit_to_scene;
	hit_to_scene.happened = false;
	Scene::FindHit(ray , hit_to_scene);

	if (!hit_to_scene.happened) return Vector3f(0);
	if (hit_to_scene.m->islight) return hit_to_scene.m->lightIntensity;


	//取出所有光线在场景的第一个交点的信息
	Vector3f hit_pos = hit_to_scene.hitcoord;
	Vector3f N = hit_to_scene.hitN.normalized();
	Material * mat = hit_to_scene.m;
	Vector3f wi = (ray.dir * -1).normalized();

	switch (mat->mtype)
	{
	case REFLC:
	{
		//计算间接光照
		float gs = RandomFloat();
		{
			Vector3f futureDir = mat->GetFutureDir(wi, N);
			Ray newRay(hit_pos, futureDir);
			HitPoint test;
			FindHit(newRay, test);
			if (test.happened )
			{
				Vector3f brdf_ = mat->GetBRDF(wi, futureDir, N);
				float costheta3 = dotProduct(futureDir, N);
				float pdf_ = mat->pdf(wi, futureDir, N);
				if ( pdf_ > 0.0001)
					L_indir = PathTracing(newRay, depth + 1) * brdf_ * costheta3 / pdf_ / RussianRoulette;
			}
		}
		break;
	}
	case MIRCO:
	case DIFFUSE:
	{
		//直接光照，随机采样光源
		float pdf_L = 0;
		HitPoint sample_light;
		sampleLight(sample_light, pdf_L);

		Vector3f sample_hit = sample_light.hitcoord;
		Vector3f sampleRayDir = (sample_hit - hit_pos).normalized();
		float d = (hit_pos - sample_hit).len();

		Ray sampleRay(hit_pos, sampleRayDir);
		HitPoint hit_to_sample;
		Scene::FindHit(sampleRay, hit_to_sample);

		if (hit_to_sample.happened && hit_to_sample.m->islight)
		{
			Vector3f L_i = hit_to_sample.m->lightIntensity;
			Vector3f brdf1 = mat->GetBRDF(wi, sampleRayDir, N);
			float cos_theta1 = clamp(0.0,1.0, dotProduct(N, sampleRayDir));
			float cos_theta2 = clamp(0.0, 1.0, dotProduct(hit_to_sample.hitN, sampleRayDir * -1));
			if (pdf_L > 0.0001)
				L_dir = (L_i * brdf1 * cos_theta1 * cos_theta2 / (d * d)) / pdf_L;
			//printf("像素距离光源的距离%.3f\n", d);
		}

		//计算间接光照
		float gs = RandomFloat();
		if (gs < RussianRoulette)
		{
			Vector3f futureDir = mat->GetFutureDir(wi, N);
			Ray newRay(hit_pos, futureDir);
			HitPoint test;
			FindHit(newRay, test);
			if (test.happened && !test.m->islight)
			{
				Vector3f brdf_ = mat->GetBRDF(wi, futureDir, N);
				float costheta3 = clamp(0.0, 1.0, dotProduct(futureDir, N));
				float pdf_ = mat->pdf(wi, futureDir, N);
				L_indir = PathTracing(newRay, depth + 1) * brdf_ * costheta3 / pdf_ / RussianRoulette;
			}
		}
		break;
	}
	default:
		break;
	}
	
	
	Vector3f res = L_dir + L_indir;
	res.x = clamp(0.0, 14, res.x);
	res.y = clamp(0.0, 14, res.y);
	res.z = clamp(0.0, 14, res.z);
	return res;
}
	


