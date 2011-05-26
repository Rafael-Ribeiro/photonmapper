#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"
#include "Camera.hpp"

#define MAX_PHOTONS			1000
#define MAX_PHOTON_BOUNCE	10
#define MAX_RAY_BOUNCE		10

struct Engine
{
	Scene scene;
	int nPhotons;
	int nPhotonBounce;

	Engine(Scene& scene);

	Color* render(Camera camera,int width, int height);
};

#endif
