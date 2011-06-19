#include <iostream>

#include "../Vector.hpp"

int main()
{
	Vector n = Vector(0,1,0);
	Vector p;

	cout << "[" << endl;
	for (int i = 0; i < 10000; i++)
	{
		p = n.noise(1.0);

		cout << "[" << p.x << ", " << p.y << ", " << p.z << "]," << endl;
	}

	cout << "]" << endl;
	return 0; 
}
