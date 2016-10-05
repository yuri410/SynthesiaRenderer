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
#ifndef D3D9TEXTURE_H
#define D3D9TEXTURE_H

#include "D3D9Common.h"
#include "VolatileResource.h"

#include "apoc3d/Graphics/RenderSystem/Texture.h"
#include "apoc3d/Graphics/GraphicsCommon.h"

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
			class D3D9Texture final : public Apoc3D::Graphics::RenderSystem::Texture, public VolatileResource
			{
			public:
				D3D9Texture(D3D9RenderDevice* device, D3DTexture2D* tex2D);
				D3D9Texture(D3D9RenderDevice* device, D3DTexture3D* tex3D);
				D3D9Texture(D3D9RenderDevice* device, D3DTextureCube* texCube);

				D3D9Texture(D3D9RenderDevice* device, const ResourceLocation& rl, TextureUsage usage, bool managed);
				D3D9Texture(D3D9RenderDevice* device, int32 width, int32 height, int32 depth, int32 level, 
					PixelFormat format, TextureUsage usage);

				D3D9Texture(D3D9RenderDevice* device, int32 length, int32 level, PixelFormat format, TextureUsage usage);

				~D3D9Texture();

				virtual void Save(Stream& strm) override;

				virtual DataRectangle lock(int32 surface, LockMode mode, const Apoc3D::Math::Rectangle& rect) override;
				virtual DataBox lock(int32 surface, LockMode mode, const Box& box) override;
				virtual DataRectangle lock(int32 surface, CubeMapFace cubemapFace, LockMode mode, 
					const Apoc3D::Math::Rectangle& rect) override;

				virtual void unlock(int32 surface) override;
				virtual void unlock(CubeMapFace cubemapFace, int32 surface) override;

				/** Loads the content of the texture from a ResourceLocation.
				 *  This is called by the Resource class when the Resource need to load.
				 *  See Resource for more information.
				 */
				virtual void load() override;
				/** Release the hardware resources used: delete the d3d9 texture.
				 */
				virtual void unload() override;

				D3DTexture2D* getInternal2D() const { return m_tex2D; }
				D3DTexture3D* getInternal3D() const { return m_tex3D; }
				D3DTextureCube* getInternalCube() const { return m_cube; }

				D3DBaseTexture* getBaseTexture() const
				{
					if (m_tex2D)
						return m_tex2D;
					if (m_tex3D)
						return m_tex3D;
					if (m_cube)
						return m_cube;
					return 0;
				}

				void ClearInternalTextureRef();
				void SetInternal2D(D3DTexture2D* tex, int32 newWidth, int32 newHeight, int32 newLevelCount, PixelFormat newFormat);
				void SetInternalCube(D3DTextureCube* tex, int32 newLength, int32 newLevelCount, PixelFormat newFormat);

			private:
				void ReleaseVolatileResource() override;
				void ReloadVolatileResource() override;

				void LoadTexture(const ResourceLocation* rl);

				String getResourceLocationName(const ResourceLocation* rl);

				/** As the specification of the Apoc3D::Graphics::RenderSystem::Texture class,
				 *  a Texture can be one of the following: 1D, 2D, 3D, Cube.
				 *  Now they are all listed below in case of use; 
				 *  because the texture type might be determined at run time when 
				 *  loading from a file, generalization is not a good idea. 
				 *
				 *  There is no 1D texture in D3D9. If the 2D texture has a width or height of
				 *  1, it will be treated as an 1D texture at the render system's texture level.
				 *  Anyway the inner workings are still the same to make it work.
				 */
				D3DTexture2D* m_tex2D = nullptr;
				D3DTexture3D* m_tex3D = nullptr;
				D3DTextureCube* m_cube = nullptr;
				D3D9RenderDevice* m_renderDevice;

				/** Keep this to specify the parameter for D3D's cube map unlock operation.
				 *  It need to know which exactly face was locked.
				 */
				D3DCUBEMAP_FACES m_lockedCubeFace;


				char* m_tempData = nullptr;
			};
		}
	}
}

#endif