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
#ifndef D3D9GRAPHICSAPIFACTORY_H
#define D3D9GRAPHICSAPIFACTORY_H

#include "D3D9Common.h"
#include "apoc3d/Graphics/RenderSystem/GraphicsAPI.h"

using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			class D3D9GraphicsAPIFactory final : public GraphicsAPIFactory
			{
			public:
				D3D9GraphicsAPIFactory()
					: GraphicsAPIFactory(GetDescription())
				{ }

				virtual DeviceContext* CreateDeviceContext();
			private:
				static APIDescription GetDescription();

			};
		}
	}
}

#endif