#!/usr/bin/python
# -*- coding:utf-8 -*-

import math

FRAMES = 120
RADIUS = 30

scene = \
"""{
	"Scene" :
	{
		"Materials" :
		[
			{ "name" : "light",				"color" : [255, 255, 255],	"roughness" : 1.00,		"absorvance" : 1.00,	"emittance" : 1.0,	"n" : 1.492 },

			{ "name" : "mirror",			"color" : [255, 255, 255],	"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : "infinity" },

			{ "name" : "glass",				"color" : [255, 255, 255],	"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },

			{ "name" : "red_glass",			"color" : [255, 0, 0],		"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },
			{ "name" : "orange_glass",		"color" : [255, 165, 0],	"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },
			{ "name" : "yellow_glass",		"color" : [255, 255, 0],	"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },
			{ "name" : "green_glass",		"color" : [0, 255, 0],		"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },
			{ "name" : "blue_glass",		"color" : [0, 0, 255],		"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },
			{ "name" : "indigo_glass",		"color" : [75, 0, 130],		"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },
			{ "name" : "viollet_glass",		"color" : [238, 130, 238],	"roughness" : 0.02,		"absorvance" : 0.02,	"emittance" : 0.0,	"n" : 1.492 },

			{ "name" : "white_concrete",	"color" : [255, 255, 255],	"roughness" : 1.00,		"absorvance" : 0.40,	"emittance" : 0.0,	"n" : "infinity" },
			{ "name" : "black_concrete",	"color" : [10, 10, 10],	"roughness" : 1.00,		"absorvance" : 0.60,	"emittance" : 0.0,	"n" : "infinity" },
		],

		"Lights" :
		[
			{
				"type" : "Sphere",
				"material" : "light",
				"center" : [%f, 15, %f],
				"radius" : 3,
				"name" : "ceilingAreaLight",
				"collidable" : false
			}
		],

		"Primitives" :
		[
			{
				"type" : "Sphere",
				"material" : "red_glass",
				"center" : [-30, -20.0, 0],
				"radius" : 4.5,
				"name" : "sphere1"
			},
			{
				"type" : "Sphere",
				"material" : "orange_glass",
				"center" : [-20.0, -20.0, 0],
				"radius" : 4.5,
				"name" : "sphere2"
			},
			{
				"type" : "Sphere",
				"material" : "yellow_glass",
				"center" : [-10, -20.0, 0],
				"radius" : 4.5,
				"name" : "sphere3"
			},
			{
				"type" : "Sphere",
				"material" : "green_glass",
				"center" : [0.0, -20.0, 0],
				"radius" : 4.5,
				"name" : "sphere4"
			},
			{
				"type" : "Sphere",
				"material" : "blue_glass",
				"center" : [10.0, -20.0, 0],
				"radius" : 4.5,
				"name" : "sphere5"
			},
			{
				"type" : "Sphere",
				"material" : "indigo_glass",
				"center" : [20.0, -20.0, 0],
				"radius" : 4.5,
				"name" : "sphere6"
			},
			{
				"type" : "Sphere",
				"material" : "viollet_glass",
				"center" : [30.0, -20.0, 0],
				"radius" : 4.5,
				"name" : "sphere7"
			},

			{ "type" : "Plane", "material" : "white_concrete",	"point" : [0.0, -40.0, 0.0],	"normal" : [0.0, 1.0, 0.0],		"name" : "floorPlane" },
			{ "type" : "Plane", "material" : "black_concrete",	"point" : [0.0, 20.0, 0.0],		"normal" : [0.0, -1.0, 0.0],	"name" : "ceilingPlane" },
			{ "type" : "Plane", "material" : "white_concrete",	"point" : [-40.0, 0.0, 0.0],	"normal" : [1.0, 0.0, 0.0],		"name" : "leftPlane" },
			{ "type" : "Plane", "material" : "white_concrete",	"point" : [40.0, 0.0, 0.0],		"normal" : [-1.0, 0.0, 0.0],	"name" : "rightPlane" },
			{ "type" : "Plane", "material" : "white_concrete",	"point" : [0.0, 0.0, 40],		"normal" : [0.0, 0.0, -1.0],	"name" : "frontPlane" },
			{ "type" : "Plane", "material" : "white_concrete",	"point" : [0.0, 0.0, -80],		"normal" : [0.0, 0.0, 1.0],		"name" : "backPlane" }
		]
	},

	"Cameras" :
	[
		{
			"origin" : [0, -20, -40.0],
			"direction" : [0, 0, 1],
			"top" : [0, 1, 0],
			"fovy" : 45.0,
			"width" : 1280,
			"height" : 800,
			"filename" : "../renders/frame_%d.ppm"
		}
	],

	"Engine" :
	{
		"Configurations" :
		{
			"MAX_PHOTONS" : 4500000,
			"MAX_PHOTON_BOUNCE" : 10,
			"MAX_RAY_BOUNCE" : 10,
			"MAX_GATHER_DISTANCE" : 2.5,
			"EXPOSURE" : 0.0035,
			"EPS" : 1e-10,
			"ANTIALIAS_THRESHOLD" : 0.5
		}
	}
}
"""

angle = math.pi*2/FRAMES

for i in xrange(FRAMES):
	f = open('frame_%d.json' % (i, ), 'w')
	s = scene % (RADIUS*math.cos(i*angle), RADIUS*math.sin(i*angle), i)
	f.write(s)
	f.close()

