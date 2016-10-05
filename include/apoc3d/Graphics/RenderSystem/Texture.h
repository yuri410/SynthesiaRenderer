#pragma once
#ifndef APOC3D_TEXTURE_H
#define APOC3D_TEXTURE_H

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

#include "apoc3d/Graphics/LockData.h"
#include "apoc3d/Core/Resource.h"
#include "apoc3d/Math/ColorValue.h"

using namespace Apoc3D::VFS;
using namespace Apoc3D::Core;
using namespace Apoc3D::IO;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			/**
			 *  A texture, which can be one of the following: 1D, 2D, 3D, Cube.
			 *  @remark:
			 *   1D textures are those with either width or height of 1.
			 *  
			 *  To access the content of the texture, 
			 */
			class APAPI Texture : public Resource
			{
			public:
				virtual ~Texture();
				virtual uint32 getSize() { return static_cast<uint32>(m_contentSize); }

				DataRectangle Lock(int32 surface, LockMode mode, CubeMapFace cubemapFace, const Apoc3D::Math::Rectangle& rect);
				DataRectangle Lock(int32 surface, LockMode mode, CubeMapFace cubemapFace);
				DataRectangle Lock(int32 surface, LockMode mode, const Apoc3D::Math::Rectangle& rect);
				/**
				 *  Locks the whole area on the 2D texture. 
				 *  The assumption that this is 2D is made when calling this.
				 */
				DataRectangle Lock(int32 surface, LockMode mode);
				DataBox Lock(int32 surface, LockMode mode, const Box& box);

				void FillColor(ColorValue cv);
				void FillColor(ColorValue cv, const Apoc3D::Math::Rectangle& rect);

				/** Unlocks the lock part. Works for all types of textures. */
				void Unlock(int32 surface);
				/** Unlocks the cube map face. */
				void Unlock(CubeMapFace cubemapFace, int32 surface);

				/** Save the texture as a TextureData into a Stream. */
				virtual void Save(Stream& strm) = 0;

				
				bool isLocked() const { return m_isLocked; }
				
				TextureType getType() const { return m_type; }

				/**
				 *  Gets the width of the texture. 
				 *  If the texture is a cube map, this will return the cube's length. 
				 *  In this case, no other properties are recommended to retrieve the length.
				 *  Just use this one.
				 */
				int32 getWidth() const { return m_width; }
				int32 getHeight() const { return m_height; }
				/**
				 *  Gets the num of slices of the texture.
				 *  This will always return 1 for 1D and 2D textures.
				 */
				int32 getDepth() const { return m_depth; }
				/** Gets the estimated total number of bytes of all mip levels. */
				int32 getContentSize() const { return m_contentSize; }
				/** Gets the number of mip level. */
				int32 getLevelCount() const { return m_levelCount; }
				TextureUsage getUsage() const { return m_usage; }
				PixelFormat getFormat() const { return m_format; }


				RenderDevice* getRenderDevice() const { return m_renderDevice; }
				const ResourceLocation* getResourceLocation() const { return m_resourceLocation; }

			protected:
				/**
				 *  Fill the texture object's properties like width, format,
				 *  using the data  provided.
				 */
				void UpdateInfo(const TextureData& data);
				
				void UpdateProperties(TextureType type, int width, int height, int depth, int levelCount, PixelFormat format, TextureUsage usage);

				Texture(RenderDevice* device, const ResourceLocation& rl, TextureUsage usage, bool managed);
				Texture(RenderDevice* device, int32 width, int32 height, int32 depth, 
					int32 levelCount, PixelFormat format, TextureUsage usage);
				Texture(RenderDevice* device, int32 length, int32 levelCount, TextureUsage usage, PixelFormat format);
				
				/** [2DTexture] Implement this at the API render system. */
				virtual DataRectangle lock(int32 surface, LockMode mode, const Apoc3D::Math::Rectangle& rectangle) = 0;

				/** [3DTexture] Implement this at the API render system. */
				virtual DataBox lock(int32 surface, LockMode mode, const Box& box) = 0;

				/** [CubeMap] Implement this at the API render system. */
				virtual DataRectangle lock(int32 surface, CubeMapFace cubemapFace, LockMode mode, 
					const Apoc3D::Math::Rectangle& rectangle) = 0;

				/** [All] Implement this at the API render system. */
				virtual void unlock(int32 surface) = 0;

				/** [CubeMap] Implement this at the API render system. */
				virtual void unlock(CubeMapFace cubemapFace, int32 surface) = 0;

				
			private:
				RenderDevice* m_renderDevice;
				ResourceLocation* m_resourceLocation = nullptr;
				TextureType m_type;
				int32 m_width = 0;
				int32 m_height = 0;
				int32 m_depth = 0;
				int32 m_contentSize = 0;
				int32 m_levelCount = 0;
				TextureUsage m_usage;
				PixelFormat m_format;

				bool m_isLocked = false;

				void RecalculateContentSize();
				
			};
		}

	}
}
#endif