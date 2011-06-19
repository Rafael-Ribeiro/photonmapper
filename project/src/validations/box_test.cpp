#include <iostream>

#include "../Box.hpp"
#include "../Material.hpp"
#include "../Photon.hpp"

int main()
{
	Material light = Material(Color(255, 255, 255),	0.5, 1.0, 1.0, 1.492);
	Box b1	= Box(light, Point( -20, -39.99, 20), Vector(0, 40.0, 0), Vector(-15, 0.0, 10), Vector(10, 0, 15)); 
	Photon p;
	
	cout << "[" << endl;
	for (int i = 0; i < 10000; i++)
	{
		p = b1.randomPhoton();

		cout << "[" << p.ray.origin.x << ", " << p.ray.origin.y << ", " << p.ray.origin.z << "]," << endl;
	}

	cout << "]" << endl;
	return 0; 
}
