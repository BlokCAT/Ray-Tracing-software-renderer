#pragma once

#include "Scene.hpp"

class Renderer
{
public:
	int spp;
	Renderer ( int s):spp(s){}
	static float Crad(float deg);
	void Render(Scene &scene);

};
