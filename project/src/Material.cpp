#include "Material.hpp"

#include <math.h>
#include <iostream>
#include <limits>

#include "jsonbox/inc/JsonBox.h"

using namespace std;

Material::Material()
{
}

Material::Material(string name, const Color& color, double roughness, double absorvance, double emittance, double n)
	: name(name), color(color), absorvance(absorvance), roughness(roughness), emittance(emittance), n(n)
{
}

bool Material::parse(const JsonBox::Value &materialVal, Material &material)
{
	JsonBox::Object materialObj;

	double infinity = numeric_limits<double>::infinity();


	if (!materialVal.isObject())
	{
		cerr << "Error: material is not an Object." << endl;
		return false;
	}

	materialObj = materialVal.getObject();

	if (!materialObj["name"].isString())
	{
		cerr << "Error: material's name is not a string." << endl;
		return false;
	}

	material.name = materialObj["name"].getString();

	if
	(
		!materialObj["color"].isArray()
		||
		(!materialObj["color"].getArray()[0].isInteger() && !materialObj["color"].getArray()[0].isDouble())
		||
		(!materialObj["color"].getArray()[1].isInteger() && !materialObj["color"].getArray()[1].isDouble())
		||
		(!materialObj["color"].getArray()[2].isInteger() && !materialObj["color"].getArray()[2].isDouble())
	)
	{
		/* color is not an array or one of the components is neither an integer nor a double */
		cerr << "Error: material's color is not an (valid) array." << endl;
		return false;
	}

	material.color.r = (materialObj["color"].getArray()[0].isInteger() ? materialObj["color"].getArray()[0].getInt() : materialObj["color"].getArray()[0].getDouble());
	material.color.g = (materialObj["color"].getArray()[1].isInteger() ? materialObj["color"].getArray()[1].getInt() : materialObj["color"].getArray()[1].getDouble());
	material.color.b = (materialObj["color"].getArray()[2].isInteger() ? materialObj["color"].getArray()[2].getInt() : materialObj["color"].getArray()[2].getDouble());

	if
	(
		(!materialObj["roughness"].isInteger() && !materialObj["roughness"].isDouble())
		||
		(!materialObj["absorvance"].isInteger() && !materialObj["absorvance"].isDouble())
		||
		(!materialObj["emittance"].isInteger() && !materialObj["emittance"].isDouble())
		||
		(!materialObj["n"].isInteger() && !materialObj["n"].isDouble() && (!materialObj["n"].isString() || materialObj["n"].getString().compare("infinity") != 0))
	)
	{
		cerr << "Error: at least one of the material's parameters (roughness, absorvance, emittance or n) is invalid." << endl;
		return false;
	}

	material.roughness = (materialObj["roughness"].isInteger() ? materialObj["roughness"].getInt() : materialObj["roughness"].getDouble());
	material.absorvance = (materialObj["absorvance"].isInteger() ? materialObj["absorvance"].getInt() : materialObj["absorvance"].getDouble());
	material.emittance = (materialObj["emittance"].isInteger() ? materialObj["emittance"].getInt() : materialObj["emittance"].getDouble());
	material.n = (materialObj["n"].isInteger() ? materialObj["n"].getInt() : (materialObj["n"].isDouble() ? materialObj["n"].getDouble() : infinity));

	return true;
}

double Material::reflectance(const Vector &direction, const Vector &normal, const Material &fromMaterial) const
{
	/*
	 * Reflectance according to http://www.bramz.net/data/writings/reflection_transmission.pdf
	 */

	if (this->n == numeric_limits<double>::infinity())
		return 1.0;

	double n = fromMaterial.n / this->n;
	double cosI = -(normal.dot(direction));
	double sinT2 = n * n * (1.0 - cosI * cosI);

	if (sinT2 > 1.0) /* inside TIR */
		return 1.0;

	double cosT = sqrt(1.0 - sinT2);
	double rOrth = (fromMaterial.n * cosI - this->n * cosT) / (fromMaterial.n * cosI + this->n * cosT);
	double rPar = (this->n * cosI - fromMaterial.n * cosT) / (this->n * cosI + fromMaterial.n * cosT);

	return (rOrth * rOrth + rPar * rPar) / 2.0;
}

Vector Material::reflectionDirection(const Vector &direction, const Vector &normal) const
{
	return direction - normal * 2 * direction.dot(normal);
}

Vector Material::refractionDirection(const Vector &direction, const Vector &normal, const Material &fromMaterial) const
{
	/*
	 * Refraction direction according to http://www.bramz.net/data/writings/reflection_transmission.pdf
	 */
	double n = fromMaterial.n / this->n;
	double cosI = -(normal.dot(direction));
	double sinT2 = n * n * (1.0 - cosI * cosI);

	if (sinT2 > 1.0) /* inside TIR's range: 100% reflection MUST occur */
	{
		cerr << "ERROR: Invalid refraction (check Material.cpp: " << __LINE__ << ")" << endl;
		return Vector(0.0, 0.0, 0.0);
	}

	double cosT = sqrt(1.0 - sinT2);

	return (direction * n + normal * (n * cosI - cosT)).normalized();
}
