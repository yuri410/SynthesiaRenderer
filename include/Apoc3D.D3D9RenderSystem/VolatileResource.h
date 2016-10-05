#pragma once
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

#ifndef APOC3D_D3D9_VOLATILERESOURCE_H
#define APOC3D_D3D9_VOLATILERESOURCE_H

#include "D3D9Common.h"

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			/** Provides an interface to release and restore those resource needed to handle when the device is lost. 
			*/
			class VolatileResource
			{
			private:
				D3D9RenderDevice* m_rawDevice;
			public:
				VolatileResource(D3D9RenderDevice* device);
				virtual ~VolatileResource();

				/** called when OnDeviceLost
				*/
				virtual void ReleaseVolatileResource() = 0;
				/** called when OnDeviceReset
				*/
				virtual void ReloadVolatileResource() = 0;
			};
		}
	}
}

#endif