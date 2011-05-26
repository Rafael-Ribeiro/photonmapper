#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"

#define MAX_PHOTONS			((unsigned int)1000)
#define MAX_PHOTON_BOUNCE	((unsigned int)10)
#define MAX_RAY_BOUNCE		10

struct Engine
{
	Scene scene;
	int nPhotons;
	int nPhotonBounce;

	Engine(Scene& scene);

	Color* render(Point origin, Vector direction, Vector top, double fovy, int width, int height);
};

#endif
