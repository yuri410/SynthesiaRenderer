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
#ifndef D3D9DEPTHBUFFER_H
#define D3D9DEPTHBUFFER_H

#include "D3D9Common.h"
#include "VolatileResource.h"
#include "apoc3d/Graphics/RenderSystem/RenderTarget.h"
#include "D3D9RenderDevice.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			class D3D9DepthBuffer final : public DepthStencilBuffer, public VolatileResource
			{
				RTTI_DERIVED(D3D9DepthBuffer, DepthStencilBuffer);

			public:
				D3D9DepthBuffer(D3D9RenderDevice* device, int32 width, int32 height, DepthFormat depthFormat, const String& multisampleMode);
				D3D9DepthBuffer(D3D9RenderDevice* device, int32 width, int32 height, DepthFormat depthFormat, const D3D9Capabilities::AAProfile* aamode);

				~D3D9DepthBuffer();

				IDirect3DSurface9* getD3DBuffer() const { return m_depthSurface; }

				virtual void ReleaseVolatileResource() override;
				virtual void ReloadVolatileResource() override;

			private:
				D3D9RenderDevice* m_device;
				IDirect3DSurface9* m_depthSurface = nullptr;

				const D3D9Capabilities::AAProfile* m_explicitAAMode = nullptr;
			};
		}
	}
}

#endif