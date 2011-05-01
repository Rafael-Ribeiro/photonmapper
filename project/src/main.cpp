#include <iostream>

#include "Vector.hpp"

using namespace std;

int main()
{
	Point p = Point(1.0, 2.0, 3.0);
	Point q = Point(3.0, 2.0, 1.0);
	
	cout << (p.dot(q)).norm() << endl;

	return 0;
}
