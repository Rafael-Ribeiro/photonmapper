#include "Primitive.hpp"

#include "Plane.hpp"
#include "Quad.hpp"
#include "Sphere.hpp"

#include <map>

#include "jsonbox/inc/JsonBox.h"

Primitive::Primitive()
{
}

Primitive::Primitive(const Material& mat)
	: mat(mat)
{
}

Primitive *Primitive::parse(const JsonBox::Value &primitiveVal, map<string,Material>& materials)
{
	Primitive *prim = NULL;
	JsonBox::Object primitive;
	Material mat;

	if (!primitiveVal.isObject())
	{
		cerr << "Error: Primitive must be an Object." << endl;
		return NULL;
	}

	primitive = primitiveVal.getObject();

	if (!primitive["type"].isString())
	{
		cerr << "Error: Primitive's type must be a string." << endl;
		return NULL;
	}

	if (!primitive["material"].isString() || materials.find(primitive["material"].getString()) == materials.end())
	{
		cerr << "Error: Primitive's material is invalid or undefined." << endl;
		return NULL;
	}

	mat = materials[primitive["material"].getString()];

	if (primitive["type"].getString().compare("Box") == 0)
	{
		/* TODO/FIXME */
		cerr << "Error: Box type not implemented yet." << endl;
		return NULL;
	} else if (primitive["type"].getString().compare("Plane") == 0)
	{
		prim = Plane::parse(mat, primitiveVal);
		if (!prim)
		{
			cerr << "Error: invalid Plane format." << endl;
			return NULL;
		}

	} else if (primitive["type"].getString().compare("Quad") == 0)
	{
		prim = Quad::parse(mat, primitiveVal);
		if (!prim)
		{
			cerr << "Error: invalid Quad format." << endl;
			return NULL;
		}

	} else if (primitive["type"].getString().compare("Sphere") == 0)
	{
		prim = Sphere::parse(mat, primitiveVal);
		if (!prim)
		{
			cerr << "Error: invalid Sphere format." << endl;
			return NULL;
		}

	} else
	{
		cerr << "Error: Primitive's type (" << primitive["type"].getString() << ") not implemented." << endl;
		return NULL;
	}

	return prim;
}
