//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>
#include <string>

#include "Recast.h"
#include "RecastDebugDraw.h"
#include "InputGeom.h"
#include "Sample_TileMesh.h"
#include "Sample_SoloMesh.h"

#ifdef WIN32
#	define snprintf _snprintf
#	define putenv _putenv
#endif

using std::string;
using std::vector;

int main(int argc, char** argv)
{
	InputGeom* geom = 0;
	Sample* sample = 0;
	BuildContext ctx;

	// create sample to use
	sample = new Sample_TileMesh();
	// sample = new Sample_SoloMesh();
	sample->setContext(&ctx);

	// set the agent radius from arguments
	if (argc > 2)
	{
		sample->m_agentRadius = (float) atof(argv[2]);
	}

	// load level
	string path = argv[1];
	geom = new InputGeom();
	printf("Loading '%s'...\n", path.c_str());
	if (!geom->load(&ctx, path))
	{
		delete geom;
		geom = 0;

		// Destroy the sample if it already had geometry loaded, as we've just deleted it!
		if (sample && sample->getInputGeom())
		{
			delete sample;
			sample = 0;
		}
	}
	if (sample && geom)
	{
		sample->handleMeshChanged(geom);
	}

	// build
	if (geom && sample)
	{
		// filename (replace .obj extension by .bin)
		string filename(argv[1]);
		size_t pos = filename.rfind(".", string::npos);
		if (pos != string::npos)
		{
			filename = filename.substr(0, pos);
		}
		filename += ".bin";
		// build
		sample->handleSettings();
		printf("Building...\n");
		sample->handleBuild();
		printf("Saving...\n");
		sample->saveAll(filename.c_str(), sample->m_navMesh);
	}

	// save all
	delete sample;
	delete geom;

	return 0;
}
