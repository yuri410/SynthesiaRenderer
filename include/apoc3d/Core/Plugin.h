#pragma once
#ifndef APOC3D_PLUGIN_H
#define APOC3D_PLUGIN_H

/**
 * -----------------------------------------------------------------------------
 * This source file is part of Apoc3D Engine
 * 
 * Copyright (c) 2009+ Tao Xin
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  if not, write to the Free Software Foundation, 
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
 * http://www.gnu.org/copyleft/gpl.txt.
 * 
 * -----------------------------------------------------------------------------
 */

#include "apoc3d/Common.h"

#ifndef APOC3D_DYNLIB
#	define PLUGINAPI
#else
#	ifdef APOC3D_PLUGIN_DYLIB_EXPORT
#		define PLUGINAPI __declspec( dllexport )
#	else
#		define PLUGINAPI __declspec( dllimport )
#	endif
#endif

namespace Apoc3D
{
	namespace Core
	{
		class APAPI Plugin
		{
		public:
			virtual ~Plugin() { }

			virtual void Load() = 0;
			virtual void Unload() = 0;

			virtual String GetName() = 0;
		};
	}
}

#endif