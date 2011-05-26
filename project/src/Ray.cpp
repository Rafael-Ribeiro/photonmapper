#include "Ray.hpp"
#include "Scene.hpp"

Ray::Ray()
{
}

Ray::Ray(Point origin, Vector direction)
	: origin(origin), direction(direction)
{
}

Color Ray::getColor(const Scene& scene, int maxdepth) const
{
	Color c;
	Intersection inter;

	if (maxdepth == 0)
		return Color(0,0,0);

	if (!scene.intersect(*this, inter))
		return Color(0,1,0); /* TODO: foreground color */

	/* get color here */
	
	/* material color */
	return inter.prim->mat.color;	
}
