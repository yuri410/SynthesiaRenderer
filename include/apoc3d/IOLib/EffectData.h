#pragma once
#ifndef APOC3D_EFFECTDATA_H
#define APOC3D_EFFECTDATA_H

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

#include "apoc3d/Collections/List.h"
#include "apoc3d/Graphics/EffectSystem/EffectParameter.h"

using namespace Apoc3D::Graphics::EffectSystem;
using namespace Apoc3D::Collections;
using namespace Apoc3D::VFS;
using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace IO
	{
		class APAPI EffectProfileData
		{
		public:
			static const char* Imp_HLSL;
			static const char* Imp_GLSL;

			char ImplementationType[5];

			/** The major version of shader model expected. */
			int MajorVer;
			int MinorVer;

			/** The length of shader code in bytes. */
			int VSLength = 0;
			int PSLength = 0;
			int GSLength = 0;

			/**
			 *  The shader code. Could be compiled HLSL microcode or GLSL source 
			 *  depends on the ImplementationType
			 */
			char* VSCode = nullptr;
			char* PSCode = nullptr;
			char* GSCode = nullptr;


			List<EffectParameter> Parameters;

			EffectProfileData();
			~EffectProfileData();
			
			EffectProfileData(EffectProfileData&& other);
			EffectProfileData& operator=(EffectProfileData&& other);

			EffectProfileData(const EffectProfileData&) = delete;
			EffectProfileData& operator=(const EffectProfileData&) = delete;

			void LoadV5(const String& name, BinaryReader* br);
			void SaveV5(BinaryWriter* bw);

			bool MatchImplType(const char* str) const;
			void SetImplType(const char* str);
			void SetImplType(const std::string& str);
		};

		/** 
		 *  Defines one entire effect's data stored in binary form and procedures to load/save them.
		 */
		class APAPI EffectData
		{
		public:
			String Name;

			List<EffectProfileData> Profiles;

			/**
			 *  Indicates whether this effect is from a AFX or CFX. 
			 *  AFX is loaded by AutomaticEffect, while CFX is the custom one.
			 */
			bool IsCFX = false;

			EffectData();
			~EffectData();

			EffectData(const EffectData&) = delete;
			EffectData& operator=(const EffectData&) = delete;

			void Load(const ResourceLocation& rl);
			void Save(Stream& strm) const;

			void SortProfiles();
		private:
			void LoadFXV5(BinaryReader* br);

		};
	}
}

#endif