#pragma once
#ifndef APOC3D_MESHSIMPLIFIER_H
#define APOC3D_MESHSIMPLIFIER_H

#include "ModelTypes.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace MeshSimplifier
		{
			APAPI void ProgressiveMesh(const List<Vector3>& vert, const List<MeshFace>& tri, List<int>& map, List<int>& permutation);
		}
	}
}

#endif