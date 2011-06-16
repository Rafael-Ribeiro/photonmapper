#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"
#include "Camera.hpp"

struct Engine
{
	static const int MAX_PHOTONS = 1500;
	static const int MAX_PHOTON_BOUNCE = 5;
	static const int MAX_RAY_BOUNCE = 10;
	static const double MAX_GATHER_DISTANCE = 10.0;
	static const double EXPOSURE = 0.016;
	static const double EPS = 1e-10;
	static const double CONSTANT_LIGHT_ATTENUATION = 1.0;
	static const double LINEAR_LIGHT_ATTENUATION = 0.04;
	static const double QUADRATIC_LIGHT_ATTENUATION = 0.0009;
	static const double ANTIALIAS_THRESHOLD = 0.5;
	static const int MAX_DIFFUSE_RAYS = 100;

	static const Vector top; /* used for internal calculations, this is not the camera's top */
	static const Vector right; /* used for internal calculations, this is not the camera's right */

	Scene scene;
	int nPhotons;
	int nPhotonBounce;
	Color* pixels;

	Engine();
	Engine(Scene& scene);

	Color* render(const Camera& camera);

	void antialias(const Camera& camera);
};

#endif
