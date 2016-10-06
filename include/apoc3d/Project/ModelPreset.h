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

#ifndef APOC3D_MODELPRESET_H
#define APOC3D_MODELPRESET_H

#include "apoc3d/Common.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Graphics/RenderSystem/VertexElement.h"
#include "apoc3d/Math/Color.h"

using namespace Apoc3D::Config;
using namespace Apoc3D::Collections;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace ProjectSystem
	{
		struct APAPI ModelPresetMaterial
		{
			String NamePattern;
			IO::MaterialData* Data = nullptr;

			ModelPresetMaterial() { }
			~ModelPresetMaterial();

			ModelPresetMaterial(ModelPresetMaterial&&);
			ModelPresetMaterial& operator=(ModelPresetMaterial&&);

			ModelPresetMaterial(const ModelPresetMaterial&) = delete;
			ModelPresetMaterial& operator=(const ModelPresetMaterial&) = delete;

		};

		class APAPI ModelPreset
		{
		public:
			bool UseTextureNameConversion = false;
			bool CopyEntireMaterial = false;

			bool CollapseMeshs = false;
			bool UseVertexFormatConversion = false;
			
			List<VertexElement> ConversionVertexElements;

			List<ModelPresetMaterial> BaseData;
			
			void Parse(Configuration& conf);

			IO::MaterialData* SearchMaterial(const String& name) const;

			ModelPreset() { }
			ModelPreset(const ModelPreset&) = delete;
			ModelPreset& operator=(const ModelPreset&) = delete;
		private:

		};
	}
}

#endif