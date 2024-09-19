#pragma once

#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include "Object.hpp"
#include "Vector.hpp"
#include "Material.hpp"
#include <iostream>


class Plane :public Object
{

private:
	float half_len;
	float ara;
	Vector3f Z;
	Vector3f node1, node2, node3, node4;
	Vector3f e1 , e2 , e3 , e4;
public:
	Vector3f cen, N , up;
	float len = 0.0;
	
	Plane(Vector3f c, Vector3f n, Vector3f u, float l,Material* mm)
	{
		cen = c;
		N = n;
		up = u;
		m = mm;
		len = l;
		half_len = len / 2.0;
		N.normalized();
		up.normalized();
		Z = crossProduct(N, up).normalized();


		node1 = cen + (Z * half_len) + (up * half_len);
		node2 = cen - (Z * half_len) + (up * half_len);
		node3 = cen - (Z * half_len) - (up * half_len);
		node4 = cen + (Z * half_len) - (up * half_len);
		e1 = node2 - node1; //起点node1的边向量
		e2 = node3 - node2;
		e3 = node4 - node3;
		e4 = node1 - node4;
		ara = len * len;
	}

	

	bool isInPlane(Vector3f &pos)const 
	{
		Vector3f p1 = pos - node1;
		Vector3f p2 = pos - node2;
		Vector3f p3 = pos - node3;
		Vector3f p4 = pos - node4;

		Vector3f t1 = crossProduct(e1, p1);
		Vector3f t2 = crossProduct(e2, p2);
		Vector3f t3 = crossProduct(e3, p3);
		Vector3f t4 = crossProduct(e4, p4);

		ChangeO(t1, t2, t3, t4);
		/*std::cout << "t1:" << t1.x << " " << t1.y << " " << t1.z << "\n";
		std::cout << "t2:" << t2.x << " " << t2.y << " " << t2.z << "\n";
		std::cout << "t3:" << t3.x << " " << t3.y << " " << t3.z << "\n";
		std::cout << "t4:" << t3.x << " " << t4.y << " " << t4.z << "\n";*/

		float d1 = dotProduct(t1, N);
		float d2 = dotProduct(t2, N);
		float d3 = dotProduct(t3, N);
		float d4 = dotProduct(t4, N);
		if( (d1 >=0 && d2 >= 0 && d3 >=0 &&d4 >= 0 ) || (d1 <= 0 && d2 <= 0 && d3 <= 0 && d4 <= 0))
			return true;
		return false;
	}

	void getHitPoint(Ray &ray, HitPoint &res)const
	{
		float ans = dotProduct(ray.dir, N);
		if ( ans >= 0 ) return;
		Vector3f L = cen - ray.pos;
		float t = dotProduct(L, N) / ans;
		Vector3f hit_pos = ray.pos + (ray.dir * t);

		if (!isInPlane(hit_pos))return;
		if (res.distance > t)
		{
			res.distance = t;
			res.happened = true;
			res.hitcoord = hit_pos;
			res.hitN = N;
			res.m = m;
		}
		return;
	}
	float getAra()
	{
		return ara;
	}
	void SampleLight(HitPoint &hp, float &pdf_L)
	{
		float dir_z = len * RandomFloat() , dir_u = len *  RandomFloat();
		Vector3f sameple_node = node3 + (Z * dir_z) + (up * dir_u);
		hp.happened = true;
		hp.hitcoord = sameple_node;
		hp.hitN = N;
		hp.m = m;
		pdf_L = 1 / getAra();
		if (pdf_L < 0.001)pdf_L = 0.001;
	}
};
#endif