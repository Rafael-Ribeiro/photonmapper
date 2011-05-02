#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Scene.hpp"
#include "Vector.hpp"
#include "Color.hpp"

struct Engine
{
	Scene scene;
	
	Engine(Scene& scene);

	Color* render(Point origin, Vector direction, Vector top, double apperture, int width, int height);
};

#endif
