#pragma once

#include "Scene.hpp"

class Renderer
{
public:

	static float Crad(float deg);
	void Render(const Scene &scene);

};
