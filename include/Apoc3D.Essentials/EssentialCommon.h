/*
-----------------------------------------------------------------------------
This source file is part of Apoc3D Engine

Copyright (c) 2009+ Tao Xin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  if not, write to the Free Software Foundation, 
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.txt.

-----------------------------------------------------------------------------
*/

#ifndef APOC3DEX_ESSENTIALCOMMON_H
#define APOC3DEX_ESSENTIALCOMMON_H

#include "apoc3d/Common.h"

#include <queue>

#ifndef APOC3D_DYNLIB
#	define APEXAPI
#else
#	ifdef APOC3D_ESS_DYLIB_EXPORT
#		define APEXAPI __declspec( dllexport )
#	else
#		define APEXAPI __declspec( dllimport )
#	endif
#endif

#pragma comment (lib, "Apoc3d.lib")

namespace Apoc3DEx
{
	namespace AI
	{
		class AStarNode;
		class PathFinderField;
		class PathFinder;
		class PathFinderResult;
		class AStarNode3;
		class VolumePathFinderResult;
		class VolumePathFinder;
		class VolumePathFinderField;
		struct VolumePathFinderResultPoint;
	};
}

#endif