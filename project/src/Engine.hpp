#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"
#include "Camera.hpp"

struct Engine
{
	static const int MAX_PHOTONS = 100;
	static const int MAX_PHOTON_BOUNCE = 10;
	static const int MAX_RAY_BOUNCE = 10;
	static const double MAX_GATHER_DISTANCE = 50.0;
	static const double EXPOSURE = 0.1;

	Scene scene;
	int nPhotons;
	int nPhotonBounce;

	Engine();
	Engine(Scene& scene);

	Color* render(Camera camera);
};

#endif
