#pragma once

#ifndef APOC3D_ABCCONFIGURATIONFORMAT_H
#define APOC3D_ABCCONFIGURATIONFORMAT_H

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
#include "Configuration.h"

using namespace Apoc3D::IO;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Config
	{
		/** 
		 *  An Apoc3d Binary Configuration format. Due to the text requirements of ConfigurationSection,
		 *  values are stored as Strings. Compared to XML config, no XML parsing needed and cant be edited using text editors.
		 */
		class APAPI ABCConfigurationFormat : public ConfigurationFormat
		{
		public:
			static ABCConfigurationFormat Instance;

			virtual void Load(const ResourceLocation& rl, Configuration* result) override;
			virtual void Save(Configuration* config, Stream& strm) override;

			List<String> GetSupportedFileSystemExtensions() override
			{
				return { L"abc" };
			}
		private:
			/** 
			 *  Saves a ConfigurationSection sub tree to a section of tagged data, recursively.
			 */
			void SaveNode(ConfigurationSection* parent, BinaryWriter* bw);
			/** 
			 *  Read tagged data section recursively, while add data into ConfigurationSection
			 */
			void BuildNode(Configuration* config, const std::string& sectionName, BinaryReader* br, ConfigurationSection* parent);
			void BuildHierarchy(Configuration* config, TaggedDataReader* doc);
		};
		
	}
}

#endif