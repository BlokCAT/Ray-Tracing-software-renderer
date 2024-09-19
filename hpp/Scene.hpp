#pragma once
#include "Vector.hpp"
#include "Ray.hpp"
#include "Object.hpp"
#include "Boll.hpp"
#include <vector>

class Scene
{
public:
	int w = 500, h = 500;
	float fov = 40;
	Scene ( int a , int b):w(a),h(b){}

	Vector3f PathTracing(  Ray &ray, int depth)const ;
	void Add( Object *t);
	void FindHit( Ray &ray ,  HitPoint &hp) const;
	void sampleLight(HitPoint &hp, float &pdf)const ;
private :
	std::vector< Object* > objs ;
};