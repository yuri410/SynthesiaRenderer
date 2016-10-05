#pragma once
#ifndef APOC3D_INICONFIGURATIONFORMAT_H
#define APOC3D_INICONFIGURATIONFORMAT_H

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

#include "Configuration.h"
#include "apoc3d/Collections/List.h"

using namespace Apoc3D::VFS;


namespace Apoc3D
{
	namespace Config
	{
		/** INI format configuration storing as described in Configuration */
		class APAPI IniConfigurationFormat : public ConfigurationFormat
		{
		public:
			static IniConfigurationFormat Instance;

			virtual void Load(const ResourceLocation& rl, Configuration* result) override;
			virtual void Save(Configuration* config, Stream& strm) override;

			List<String> GetSupportedFileSystemExtensions() override
			{
				return { L"ini" };
			}

		};
	}
}

#endif