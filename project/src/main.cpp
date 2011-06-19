#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>

#include "Vector.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include "utils.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Quad.hpp"

#include "jsonbox/inc/JsonBox.h"

/*
 * Scene assumes:
 *
 *			^ y
 *			|
 *			|    ^ z
 *			|   /
 *			|  /
 *			| /
 *			|/
 * 			+------------> x
 */

int main()
{
	Engine engine;
	Scene scene;

	vector<Camera> cameras;
	vector<Camera>::iterator cameraIt;

	Camera camera;

	JsonBox::Object descriptionJSONObject;
	JsonBox::Value descriptionJSONValue;
	JsonBox::Value engineJSONValue;
	JsonBox::Value sceneJSONValue;
	JsonBox::Value camerasJSONValue;

	Color* pixels;

	ofstream outputFile;

	descriptionJSONValue.loadFromStream(std::cin);
	descriptionJSONObject = descriptionJSONValue.getObject();

	engineJSONValue = descriptionJSONObject["Engine"];
	sceneJSONValue = descriptionJSONObject["Scene"];
	camerasJSONValue = descriptionJSONObject["Cameras"];

	engine.parse(engineJSONValue);
	if (!scene.parse(sceneJSONValue))
	{
		cerr << "Errors while parsing Scene. Aborting." << endl;
		return -1;
	}

	if (!camerasJSONValue.isArray())
	{
		cerr << "Errors while parsing Cameras. Aborting." << endl;
		return -1;
	}

	for (JsonBox::Array::const_iterator it = camerasJSONValue.getArray().begin(); it != camerasJSONValue.getArray().end(); it++)
	{
		if (!Camera::parse(*it, camera))
		{
			cerr << "Error: invalid Camera format." << endl;
			return -1;
		}

		cameras.push_back(camera);
	}

	//Camera(Point(0, 1, -40.02), Vector(0,0,1), Vector(0,1,0), M_PI/4, WIDTH, HEIGHT)

	/* Parsing was successful. */

	engine.setup(scene);

	for (cameraIt = cameras.begin(); cameraIt != cameras.end(); cameraIt++)
	{
		pixels = engine.render(*cameraIt);

		if (cameraIt->filename.compare("stdout") != 0)
			outputFile.open(cameraIt->filename.c_str());

		writePPM(pixels, cameraIt->width, cameraIt->height, (cameraIt->filename.compare("stdout") == 0 ? std::cout : outputFile));

		if (cameraIt->filename.compare("stdout") != 0)
			outputFile.close();

		delete pixels;
	}

	return 0;
}
