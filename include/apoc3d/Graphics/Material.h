#pragma once
#ifndef APOC3D_MATERIAL_H
#define APOC3D_MATERIAL_H

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

#include "GraphicsCommon.h"
#include "MaterialTypes.h"

#include "apoc3d/IOLib/MaterialData.h"

using namespace Apoc3D::Graphics::EffectSystem;
using namespace Apoc3D::Core;
using namespace Apoc3D::Math;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace Graphics
	{
		/** 
		 *  Defines colors, textures, effect and etc. for a geometry.
		 *
		 *  Material have multiple Effects. Scene will be rendered in single or
		 *  multiple pass. Each pass has sequence number and its corresponding mask value.
		 * 
		 *  First the mask value will do bitwise AND with pass flags of the material. If that passes, 
		 *  a pass effect will be retrieved from the effect map inside the material,
		 *  with the pass sequence number as an index.
		 */
		class APAPI Material
		{
		public:
#if _DEBUG
			String DebugName;
#endif

			String ExternalReferenceName;

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

			Color4 Ambient;		/** the ambient component of this material */
			Color4 Diffuse;		/** the diffuse component of this material */
			Color4 Emissive;	/** the emissive component of this material */
			Color4 Specular;	/** the specular component of this material */
			float Power;		/** the specular shininess */


			const MaterialCustomParameter* getCustomParameter(const String& usage) const;
			void AddCustomParameter(const MaterialCustomParameter& value);

			const String& getPassEffectName(int index);
			void setPassEffectName(int index, const String& en);
			/** 
			 *  Gets the material's texture's name at given index.
			 * 
			 *  The texture name is the file name. The engine will try to locate
			 *  the texture file with the rule "FileLocateRule::Textures".
			 */
			const String& getTextureName(int index) const;
			/** 
			 *  Sets the material's texture's name at given index.
			 *
			 *  The texture name is the file name. The engine will try to locate
			 *  the texture file with the rule "FileLocateRule::Textures".
			 */
			void setTextureName(int index, const String& name);

			/** 
			 *  Get the first effect appeared when passing the mtrl's effect 
			 *  table from beginning to end.
			 */
			Effect* GetFirstValidEffect() const;

			/** Get the effect at the given index in the mtrl's effect table. */
			Effect* getPassEffect(int index) const;
			void setPassEffect(int index, Effect* eff) { m_effects[index] = eff; }

			/** Gets the texture at texture layer idx */
			ResourceHandle<Texture>* getTexture(int idx) const { return m_tex[idx]; }

			/** Sets the texture at texture layer idx */
			void setTexture(int idx, ResourceHandle<Texture>* value) { m_tex[idx] = value; }

			/** 
			 *  Gets the priority of this material. The legal range should be from 0 to BatchData::MaxPriority.
			 *  Higher priority will make the geometry assigned drawn earlier before the other.
			 */
			const uint32 getPriority() const { return m_priority; }
			void setPriority(uint32 value) { m_priority = value; }

			/** 
			 *  Gets the pass flags.
			 *  The pass flag is a bit field used for selecting material when rendering.
			 *  If a bit is one, objects with this material will be rendered when the 
			 *  scene render script has requested that bit.
			 */
			uint64 getPassFlags() const { return m_passFlags; }
			void setPassFlags(uint64 val) { m_passFlags = val; }


			void Load(const MaterialData& data);
			void Save(MaterialData& data);

			void LoadReferencedMaterial(const String& mtrlName);

			void Load(TaggedDataReader* data);

			/** Packs the material to a MaterialData. Then save it as TaggedData. */
			TaggedDataWriter* Save();

			Material(const Material& m);
			Material(RenderDevice* device);
			~Material();

			/** Reloads all textures in this material if their name have been changed. */
			void Reload();

		private:
			RenderDevice* m_device;

			/** 
			 *  An array of effect, initialized from m_effectName, is use to store multiple effect
			 *  for the material
			 */
			Effect* m_effects[MaxScenePass];
			// @@: it is found that an array of Strings will use up considerable
			// amount of memory as the Stl String will initialize their inner objects. 
			// Thousands of Materials are space consuming if not using map.
			HashMap<int, String> m_effectName;			/** A map of effect names */

			CustomParamTable m_customParametrs;
			ResourceHandle<Texture>* m_tex[MaxTextures];
			HashMap<int, String> m_texName;
			bool m_texDirty[MaxTextures];

			/** 
			 *  The bit field used for object selection during scene rendering, 
			 *  as mentioned in the Material class's description.
			 */
			uint64 m_passFlags;

			int32 m_priority;

			void LoadTexture(int32 index);
			void LoadEffect(int32 index);
		};
	};
};
#endif