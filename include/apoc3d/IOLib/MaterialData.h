#pragma once
#ifndef APOC3D_MATERIALDATA_H
#define APOC3D_MATERIALDATA_H

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

#include "apoc3d/Graphics/GraphicsCommon.h"
#include "apoc3d/Graphics/MaterialTypes.h"
#include "apoc3d/Math/Color.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Math;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace IO
	{
		/** Defines all the data in one material for serialization. */
		class APAPI MaterialData
		{
		public:
#if _DEBUG
			String DebugName;
#endif

			String ExternalRefName;		/** Name for retiring material reference  */


			HashMap<int, String> EffectName;

			CustomParamTable CustomParametrs;
			HashMap<int, String> TextureName;

			uint64 PassFlags;

			int32 Priority;  // [0, 31] Lower the higher the priority
			
			bool UsePointSprite;

			Blend SourceBlend;
			Blend DestinationBlend;
			BlendFunction BlendFunction;
			bool IsBlendTransparent;

			CullMode Cull;

			bool AlphaTestEnabled;
			uint32 AlphaReference;

			bool DepthWriteEnabled;
			bool DepthTestEnabled;

			
			Color4 Ambient;		/**  the ambient component of this material  */
			Color4 Diffuse;		/**  the diffuse component of this material */
			Color4 Emissive;	/**  the emissive component of this material */
			Color4 Specular;	/** the specular component of this material */
			float Power;		/** the specular shininess */


			MaterialData();
			~MaterialData();

			MaterialData(const MaterialData& other) = default;

			void AddCustomParameter(const MaterialCustomParameter& value);

			void SetDefaults();

			void LoadData(TaggedDataReader *data);
			TaggedDataWriter* SaveData();


			void Load(const ResourceLocation& rl);
			void Save(Stream& strm);
			
			void Parse(const ConfigurationSection* sect, const String& baseName = L"", FunctorReference<Color4(const String&)> colorParser = nullptr);

		private:

			/** Load with format version 3 */
			void LoadV3(TaggedDataReader* data);

			void ParseMaterialCustomParams(const String& value, FunctorReference<Color4(const String&)> colorParser);

			void CheckObsoleteProps();
		};

	}
}
#endif