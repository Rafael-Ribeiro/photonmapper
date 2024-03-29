#include <omp.h>

#include "Engine.hpp"

#include <vector>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

const Vector Engine::top = Vector(0, 1, 0);
const Vector Engine::right = Vector(1, 0, 0);

int Engine::MAX_PHOTONS = Engine::DEFAULT_MAX_PHOTONS;
int Engine::MAX_PHOTON_BOUNCE = Engine::DEFAULT_MAX_PHOTON_BOUNCE;
int Engine::MAX_RAY_BOUNCE = Engine::DEFAULT_MAX_RAY_BOUNCE;
double Engine::MAX_GATHER_DISTANCE = Engine::DEFAULT_MAX_GATHER_DISTANCE;
double Engine::MAX_GATHER_DISTANCE_SQRD = Engine::DEFAULT_MAX_GATHER_DISTANCE_SQRD;
double Engine::EXPOSURE = Engine::DEFAULT_EXPOSURE;
double Engine::EPS = Engine::DEFAULT_EPS;
double Engine::ANTIALIAS_THRESHOLD = Engine::DEFAULT_ANTIALIAS_THRESHOLD;

Engine::Engine()
{
}

Engine::Engine(Scene& scene)
	: scene(scene)
{
	this->nPhotons = Engine::MAX_PHOTONS;
	this->nPhotonBounce = Engine::MAX_PHOTON_BOUNCE;
}

void Engine::setup(Scene& scene)
{
	this->scene = scene;
	this->nPhotons = Engine::MAX_PHOTONS;
	this->nPhotonBounce = Engine::MAX_PHOTON_BOUNCE;
}

bool Engine::parse(const JsonBox::Value &engineVal)
{
	JsonBox::Object engineObj;
	JsonBox::Value configVal;
	JsonBox::Object configObj;

	JsonBox::Value tempVal;

	if (engineVal.isNull())
	{
		cerr << "(warning: using default Engine constants, since description file doesn't implement Engine)" << endl;
		return false;
	}

	engineObj = engineVal.getObject();

	configVal = engineObj["Configurations"];
	if (configVal.isNull())
	{
		cerr << "(warning: using default Engine constants, since description file doesn't implement Engine:Configurations)" << endl;
		return false;
	}

	configObj = configVal.getObject();

	tempVal = configObj["MAX_PHOTONS"];
	if (tempVal.isInteger())
		Engine::MAX_PHOTONS = tempVal.getInt();

	tempVal = configObj["MAX_PHOTON_BOUNCE"];
	if (tempVal.isInteger())
		Engine::MAX_PHOTON_BOUNCE = tempVal.getInt();

	tempVal = configObj["MAX_RAY_BOUNCE"];
	if (tempVal.isInteger())
		Engine::MAX_RAY_BOUNCE = tempVal.getInt();

	tempVal = configObj["MAX_GATHER_DISTANCE"];
	if (tempVal.isNumber())
	{
		Engine::MAX_GATHER_DISTANCE = tempVal.getNumber();
		Engine::MAX_GATHER_DISTANCE_SQRD = Engine::MAX_GATHER_DISTANCE * Engine::MAX_GATHER_DISTANCE;
	}

	tempVal = configObj["EXPOSURE"];
	if (tempVal.isNumber())
		Engine::EXPOSURE = tempVal.getNumber();

	tempVal = configObj["EPS"];
	if (tempVal.isNumber())
		Engine::EPS = tempVal.getNumber();

	tempVal = configObj["ANTIALIAS_THRESHOLD"];
	if (tempVal.isNumber())
		Engine::ANTIALIAS_THRESHOLD = tempVal.getNumber();

	return true;
}

void Engine::antialias(const Camera& camera)
{
	/*
	 * Sobel Operator: http://en.wikipedia.org/wiki/Sobel_operator
	 */

	int x, y;
	int i, j;
	int heightLimit = camera.height - 2, widthLimit = camera.width - 2;

	int pixelCounter = 0, nPixels = (heightLimit - 1) * (widthLimit - 1);

	double sumX, sumY;

	double pixelAvg;
	double g;

	Color *pixelsCpy = new Color[camera.height * camera.width];
	Color pixelColorAvg;

	/*
	 * Matrixes are inverted for memory access
	 */
	int gx[3][3] =
	{
		{	 1,	 0,	-1	},
		{	 2,	 0,	-2	},
		{	 1,	 0,	-1	}
	};

	int gy[3][3] =
	{
		{	 1,	 2,	 1	},
		{	 0,	 0,	 0	},
		{	-1,	-2,	-1	}
	};

	/*
	 * Backup original image since Sobel operator modifies this->pixels directly.
	 */
	memcpy(pixelsCpy, this->pixels, camera.height * camera.width * sizeof(Color));

	/*
	 * Sobel operator:
	 * Convolution is being applied pixel by pixel.
	 */
	#pragma omp parallel for private(x, i,j, sumX,sumY, g, pixelColorAvg)
	for (y = 1; y < heightLimit; y++)
	{
		for (x = 1; x < widthLimit; x++)
		{
			sumX = 0, sumY = 0;

			for (i = -1; i <= 1; i++)
			{
				for (j = -1; j <= 1; j++)
				{
					/* Calculate pixel's average color intensity */
					pixelAvg = (pixelsCpy[(y + i) * camera.width + x + j].r + pixelsCpy[(y + i) * camera.width + x + j].g + pixelsCpy[(y + i) * camera.width + x + j].b) / 3.0;

					sumX += pixelAvg * gx[i + 1][j + 1];
					sumY += pixelAvg * gy[i + 1][j + 1];
				}
			}

			g = sqrt(sumX * sumX + sumY * sumY);

			if (g > Engine::ANTIALIAS_THRESHOLD * 255)
			{
				/* cast rays to the pixel's corners */
				/* pixel's color becomes the average */
				pixelColorAvg = camera.rayThrough(x - 0.25, y - 0.25).getColor(scene, Engine::MAX_RAY_BOUNCE, 1.0);
				pixelColorAvg = pixelColorAvg + camera.rayThrough(x - 0.25, y + 0.25).getColor(scene, Engine::MAX_RAY_BOUNCE, 1.0);
				pixelColorAvg = pixelColorAvg + camera.rayThrough(x + 0.25, y - 0.25).getColor(scene, Engine::MAX_RAY_BOUNCE, 1.0);
				pixelColorAvg = pixelColorAvg + camera.rayThrough(x + 0.25, y + 0.25).getColor(scene, Engine::MAX_RAY_BOUNCE, 1.0);

				pixelColorAvg = pixelColorAvg / 4;

				this->pixels[y * camera.width + x] = pixelColorAvg;
			}
		}

		#pragma omp critical
		{
			cerr << "\r                                     \r"; /* 35 spaces to cleanup the line and assure cursor is on last_char_pos + 1 */
			cerr << "Anti-Aliasing (progress: " << (pixelCounter += widthLimit - 1) * 100.0 / nPixels << "%)";
			cerr.flush();
		}
	}
	cerr << endl;

	delete pixelsCpy;
}

Color* Engine::render(const Camera& camera)
{
	int i;
	int pixelCounter = 0, nPixels = camera.height * camera.width;

	vector<Photon>::iterator it;

	this->pixels = new Color[camera.width * camera.height];

	this->scene.buildPhotonMap(nPhotons, nPhotonBounce);

	#pragma omp parallel for
	for (i = 0; i < nPixels; i++)
	{
		this->pixels[i] = camera.rayThrough(i % camera.width, i / camera.width).getColor(scene, Engine::MAX_RAY_BOUNCE, 1.0);

		if (!(i % camera.width))
		{
			#pragma omp critical
			{
				cerr << "\r                                \r"; /* 30 spaces to cleanup the line and assure cursor is on last_char_pos + 1 */
				cerr << "Raytracing (progress: " << (pixelCounter += camera.width) * 100.0 / nPixels << "%)";
				cerr.flush();
			}
		}
	}
	cerr << endl;
	
	/*
	for (i = 0; i < camera.height; i++)
	{
		for (j = 0; j < camera.width; j++)
			this->pixels[i*camera.width + j] = Color(0,0,0);
	}

	for (it = this->scene.photonMap.begin(); it != this->scene.photonMap.end(); it++)
	{
		cerr 	<< it->ray.origin.x << " " << it->ray.origin.y << " " << it->ray.origin.z << " | "
				<< it->color.r << " " << it->color.g << " " << it->color.b << " | " <<endl;

		int x = (it->ray.origin.x + 60)*512.0/200;
		int y = (it->ray.origin.z + 120)*512.0/200;

		if (x>= 0 && y >= 0 && x < camera.width && y < camera.height)
			pixels[y*camera.width+x] = it->color;
	}
	
	*/

	this->antialias(camera);

	return pixels;
}
