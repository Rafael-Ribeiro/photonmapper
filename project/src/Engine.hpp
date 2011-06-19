#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"
#include "Camera.hpp"

struct Engine
{
	static int MAX_PHOTONS;
	static int MAX_PHOTON_BOUNCE;
	static int MAX_RAY_BOUNCE;
	static double MAX_GATHER_DISTANCE;
	static double MAX_GATHER_DISTANCE_SQRD;
	static double EXPOSURE;
	static double EPS;
	static double ANTIALIAS_THRESHOLD;

	static const int DEFAULT_MAX_PHOTONS = 2000000;
	static const int DEFAULT_MAX_PHOTON_BOUNCE = 10;
	static const int DEFAULT_MAX_RAY_BOUNCE = 50;
	static const double DEFAULT_MAX_GATHER_DISTANCE = 2;
	static const double DEFAULT_MAX_GATHER_DISTANCE_SQRD = 4;
	static const double DEFAULT_EXPOSURE = 0.02;
	static const double DEFAULT_EPS = 1e-10;
	static const double DEFAULT_ANTIALIAS_THRESHOLD = 0.5;

	static const Vector top; /* used for internal calculations, this is not the camera's top */
	static const Vector right; /* used for internal calculations, this is not the camera's right */

	Scene scene;
	int nPhotons;
	int nPhotonBounce;
	Color* pixels;

	Engine();
	Engine(Scene& scene);
	void setup(Scene& scene);

	bool parse(const JsonBox::Value &engineVal);
	Color* render(const Camera& camera);

	void antialias(const Camera& camera);
};

#endif
