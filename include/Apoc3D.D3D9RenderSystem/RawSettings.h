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

#ifndef APOC3D_D3D9DEVICESETTINGS_H
#define APOC3D_D3D9DEVICESETTINGS_H

#include "D3D9Common.h"

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			struct RawSettings
			{
				uint32 AdapterOrdinal = 0;
				D3DFORMAT AdapterFormat = D3DFMT_UNKNOWN;
				D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

				uint32 CreationFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE;
				D3DPRESENT_PARAMETERS PresentParameters;

				RawSettings()
				{
					memset(&PresentParameters, 0, sizeof(PresentParameters));

					PresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
					PresentParameters.BackBufferCount = 1;
					PresentParameters.MultiSampleQuality = D3DMULTISAMPLE_NONE;
					PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
					PresentParameters.EnableAutoDepthStencil = TRUE;
					PresentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
					PresentParameters.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
					PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
					PresentParameters.Windowed = TRUE;
				}
			};


			/*struct DeviceSettings
			{
				uint32 AdapterOrdinal = 0;
				D3DFORMAT AdapterFormat = D3DFMT_UNKNOWN;

				D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

				int32 BackBufferWidth = 0;
				int32 BackBufferHeight = 0;
				int32 BackBufferCount = 0;
				D3DFORMAT BackBufferFormat = D3DFMT_UNKNOWN;

				D3DFORMAT DepthStencilFormat = D3DFMT_UNKNOWN;

				bool Windowed = true;
				bool EnableVSync = true;
				bool Multithreaded = false;

				D3DMULTISAMPLE_TYPE MultiSampleType = D3DMULTISAMPLE_NONE;
				int MultiSampleQuality = 0;

				int32 RefreshRate = 0;


				DeviceSettings() { }
			};*/
		}
	};
};
#endif