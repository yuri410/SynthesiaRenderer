#pragma once
#ifndef APOC3D_TEXTUREDATA_H
#define APOC3D_TEXTUREDATA_H

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
#include "apoc3d/Collections/List.h"
#include "apoc3d/Graphics/PixelFormat.h"
#include "apoc3d/Graphics/GraphicsCommon.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace IO
	{
		/** 
		 * Defines one entire texture level's data stored in binary form and procedures to load/save them.
		 */
		class APAPI TextureLevelData
		{
		public:
			int32 Width;
			int32 Height;
			int32 Depth;
			int32 LevelSize;		/**  The size of the content data in bytes. */

			char* ContentData = nullptr;		/** Packed pixel data. */

			TextureLevelData();
			~TextureLevelData();
			
			TextureLevelData(TextureLevelData&& rhs);
			TextureLevelData& operator=(TextureLevelData&& rhs);

			TextureLevelData(const TextureLevelData&) = delete;
			TextureLevelData& operator=(const TextureLevelData&) = delete;

			void LoadData(BinaryReader& br, bool doNotLoadContent = false, int32 flags = 0);
			void SaveData(BinaryWriter& bw, int32 flags = 0) const;

			void LoadData(TaggedDataReader* data, bool doNotLoadContent = false, int32 flags = 0);
			void SaveData(TaggedDataWriter* data, int32 flags = 0) const;

		};

		/** 
		 *  Defines one entire texture's data stored in binary form and procedures to load/save them
		 *  from ResourceLocations and Streams.
		 */
		class APAPI TextureData
		{
		public:
			static const int32 FileID1 = 'A' << 24 | 'T' << 16 | 'E' << 8 | 'X';
			static const int32 FileID2 = '1TEX';

			enum TextureDataFlags
			{
				TDF_None = 0,
				TDF_RLECompressed = 1U << 0,
				TDF_LZ4Compressed = 1U << 1
			};

			TextureType Type;
			List<TextureLevelData> Levels;
			PixelFormat Format;
			int32 ContentSize;
			int32 LevelCount;
			uint32 Flags;

			TextureData();
			~TextureData();
			
			TextureData(TextureData&& rhs);
			TextureData& operator=(TextureData&& rhs);

			TextureData(const TextureData&) = delete;
			TextureData& operator=(const TextureData&) = delete;

			void Load(const ResourceLocation& rl, bool doNotLoadLevel = false, bool doNotLoadContent = false);
			void Save(Stream& strm) const;
			void SaveAsTagged(Stream& strm) const;

			void ConvertInPlace(PixelFormat fmt);
			void ResizeInPlace(int32 newWidth, int32 newHeight);

		};
	}
}

#endif
