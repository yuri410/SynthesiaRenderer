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
#ifndef D3D9RENDERTARGET_H
#define D3D9RENDERTARGET_H

#include "D3D9Common.h"
#include "VolatileResource.h"

#include "apoc3d/Graphics/RenderSystem/RenderTarget.h"
#include "apoc3d/Graphics/GraphicsCommon.h"
#include "D3D9RenderDevice.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::VFS;
using namespace Apoc3D::IO;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			class D3D9RenderTarget final : public RenderTarget, public VolatileResource
			{
				RTTI_DERIVED(D3D9RenderTarget, RenderTarget);

			public:
				D3D9RenderTarget(D3D9RenderDevice* device, int32 width, int32 height, PixelFormat format, const String& multisampleMode);
				D3D9RenderTarget(D3D9RenderDevice* device, int32 width, int32 height, PixelFormat format, const D3D9Capabilities::AAProfile* explicitAAMode);

				~D3D9RenderTarget();

				virtual Texture* GetColorTexture() override;

				virtual void PrecacheLockedData() override;

				virtual void ReleaseVolatileResource() override;
				virtual void ReloadVolatileResource() override;

				void NotifyDirty()
				{
					m_isResolvedRTDirty = true; 
					m_isLockSurfaceDirty = true;
				}

				IDirect3DSurface9* getColorSurface() const { return m_colorSurface; }
				D3D9Texture* getTextureRef() const { return m_colorWrapperTex; }
			private:
				/** Resolve multisample RT */
				void Resolve();
				void EnsureResolvedTexture();

				virtual DataRectangle lock(LockMode mode, const Apoc3D::Math::Rectangle& rect) override;
				virtual void unlock() override;

				void CreateLockingSurface();

				D3D9RenderDevice* m_device;

				IDirect3DTexture9* m_color = nullptr;			// This is either the render target itself or the resolved multisample render target
				IDirect3DSurface9* m_colorSurface = nullptr;
				D3D9Texture* m_colorWrapperTex = nullptr;

				IDirect3DSurface9* m_offscreenPlainSurface = nullptr;

				bool m_isResolvedRTDirty = false;
				bool m_isLockSurfaceDirty = true;

				const D3D9Capabilities::AAProfile* m_explicitAAMode = nullptr;

			};

			class D3D9CubemapRenderTarget final : public CubemapRenderTarget, public VolatileResource
			{
				RTTI_DERIVED(D3D9CubemapRenderTarget, CubemapRenderTarget);
			public:
				class RefRenderTarget : public RenderTarget
				{
					RTTI_DERIVED(RefRenderTarget, RenderTarget);
				public:
					RefRenderTarget(D3D9RenderDevice* device, int32 width, int32 height, PixelFormat fmt, IDirect3DSurface9* s, D3D9CubemapRenderTarget* parent);
					~RefRenderTarget();

					virtual Texture* GetColorTexture() override { assert(0); return nullptr; }

					virtual void PrecacheLockedData() override { assert(0); }

					IDirect3DSurface9* getColorSurface() const { return m_colorSurface; }
					D3D9CubemapRenderTarget* getParent() const { return m_parent; }
				private:

					virtual DataRectangle lock(LockMode mode, const Apoc3D::Math::Rectangle& rect) override { assert(0); return DataRectangle(0, 0, 0, 0, FMT_Unknown); }
					virtual void unlock() override { assert(0); }

					IDirect3DSurface9* m_colorSurface = nullptr;

					D3D9CubemapRenderTarget* m_parent;
				};

				D3D9CubemapRenderTarget(D3D9RenderDevice* device, int32 length, PixelFormat format);
				~D3D9CubemapRenderTarget();

				virtual void ReleaseVolatileResource() override;
				virtual void ReloadVolatileResource() override;

				virtual Texture* GetColorTexture() override;

			private:
				
				D3D9RenderDevice* m_device;

				D3DTextureCube* m_cubemap;
				D3D9Texture* m_cubeMapWrapper = nullptr;
			};
		}
	}
}
#endif