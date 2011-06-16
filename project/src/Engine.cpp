#include <omp.h>

#include "Engine.hpp"

#include <vector>
#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

const Vector Engine::top = Vector(0, 1, 0);
const Vector Engine::right = Vector(1, 0, 0);

Engine::Engine()
{
}

Engine::Engine(Scene& scene)
	: scene(scene)
{
	this->nPhotons = Engine::MAX_PHOTONS;
	this->nPhotonBounce = Engine::MAX_PHOTON_BOUNCE;
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
	int i, j;
	int pixelCounter = 0, nPixels = camera.height * camera.width;

	vector<Photon>::iterator it;

	this->pixels = new Color[camera.width * camera.height];

	cerr << "Building Photon Map" << endl;
	this->scene.buildPhotonMap(nPhotons, nPhotonBounce);

	//cerr << scene.photonMap.size() << endl;
	
	#pragma omp parallel for private(j)
	for (i = 0; i < camera.height; i++)
	{
		for (j = 0; j < camera.width; j++)
			this->pixels[i * camera.width + j] = camera.rayThrough(j, i).getColor(scene, Engine::MAX_RAY_BOUNCE, 1.0);

		#pragma omp critical
		{
			cerr << "\r                                \r"; /* 30 spaces to cleanup the line and assure cursor is on last_char_pos + 1 */
			cerr << "Raytracing (progress: " << (pixelCounter += camera.width) * 100.0 / nPixels << "%)";
			cerr.flush();
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
