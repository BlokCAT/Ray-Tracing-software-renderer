
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Vector.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "Boll.hpp"
#include "Plane.hpp"
#include "Object.hpp"
#include <iostream>
#include <chrono>
#include "AABB.hpp"
#undef spp
#define spp 350
int main()
{
	Vector3f v1(1 , 2 , 3);

	Scene scene(300 , 300 ,	BVH);//w  h
	//建立材质
	Material* red = new Material(Vector3f(0.8, 0.1, 0.1), DIFFUSE);
	red->ior = 1.3;
	red->roughness = 0.2;

	Material* green = new Material(Vector3f(0.14f, 0.45f, 0.091f), DIFFUSE); 
	green->ior = 5;
	green->roughness = 0.5;

	Material* blue = new Material(Vector3f(0.14f, 0.15f, 0.51f), DIFFUSE);
	green->ior = 5;
	green->roughness = 0.5;

	Material* yellow= new Material(Vector3f(0.34f, 0.35f, 0.02f), DIFFUSE);
	green->ior = 5;
	green->roughness = 0.5;

	Material *micro1 = new Material(Vector3f(0.6, 0.1, 0.1), MIRCO);
	micro1->ior = 4;
	micro1->roughness = 0.3;

	Material* micro2 = new Material(Vector3f(0.14f, 0.35f, 0.091f), MIRCO);
	micro2->ior = 18;
	micro2->roughness = 0.3;

	Material* white = new Material(Vector3f(0.725f, 0.71f, 0.63f), DIFFUSE);

	Material* light1 = new Material(Vector3f(2, 2, 2), DIFFUSE);
	light1->SetLight(Vector3f(90 , 90 , 80));

	Material* jinzi = new Material(Vector3f(0.6, 0.1, 0.1), REFLC);

	Material* jinzi2 = new Material(Vector3f(0.14f, 0.30f, 0.091f), REFLC);

	Material* redRefract = new Material(Vector3f(0.7, 0.1, 0.1), REFRACT);
	redRefract->ior = 2.0;


	//建立物体（球 ， 面 ， 光源）
	Boll b1(Vector3f(0,  -2.0 + 0.3, 40.0), 5.3, redRefract);
	Boll b4(Vector3f(6.0, -4.0 + 0.3, 40.0), 5.3, green);
	Plane L(Vector3f(0.0, 13.99, 42.0), Vector3f(0.0, -1.0, 0.0), Vector3f(0, 0, 1.0), 6, light1);
	Plane down(Vector3f(0.0, -12 , 40.0), Vector3f(0.0, 1.0, 0.0), Vector3f(0, 0, 1.0), 26, white);
	Plane back(Vector3f(0.0, 1, 53.0), Vector3f(0.0, 0.0, -1.0), Vector3f(0, 1, 0.0), 26, yellow);
	Plane right(Vector3f(13.0, 1, 40.0), Vector3f(-1.0, 0.0, 0.0), Vector3f(0, 1, 0.0), 26, red);
	Plane left(Vector3f(-13.0, 1, 40.0), Vector3f(1.0, 0.0, 0.0), Vector3f(0, 1, 0.0), 26, green);
	Plane top(Vector3f(0.0, 14, 40.0), Vector3f(0.0, -1.0, 0.0), Vector3f(0, 0, 1.0), 26, white);


	//将物体加入场景
	scene.Add(&b1);
	//scene.Add(&b4);
	scene.Add(&L);
	scene.Add(&down);
	scene.Add(&back);
	scene.Add(&right);
	scene.Add(&left);
	scene.Add(&top);

	scene.BuildAccl();
	Renderer r(spp);

	auto start = std::chrono::system_clock::now();
	r.Render(scene);
	auto stop = std::chrono::system_clock::now();


	std::cout << "渲染耗时:" << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";
	return 0;
}