#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"

#define MAX_PHOTONS			((unsigned int)1000)
#define MAX_PHOTON_BOUNCE	((unsigned int)1)
 
struct Engine
{
	Scene scene;
	unsigned int nPhotons;
	unsigned int nPhotonBounce;

	Engine(Scene& scene);

	Color* render(Point origin, Vector direction, Vector top, double apperture, unsigned int width, unsigned int height);
};

#endif
