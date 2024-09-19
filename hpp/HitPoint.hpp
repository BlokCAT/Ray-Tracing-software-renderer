#pragma once 
#include "Vector.hpp"
#include "Material.hpp"

#ifndef RAYTRACING_INTERSECTION_H
#define RAYTRACING_INTERSECTION_H
class HitPoint
{
public:
	HitPoint() {}
	Vector3f hitcoord;
	Material* m;  //材质可以返回diffuse颜色或者 纹理的颜色
	Vector3f hitN;
	float distance = 9999;
	bool happened = false;
};
#endif