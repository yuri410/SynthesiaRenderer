#pragma once
#ifndef APOC3D_MATERIALTPYES_H
#define APOC3D_MATERIALTPYES_H

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

#include "apoc3d/Math/Vector.h"
#include "apoc3d/Graphics/EffectSystem/EffectParameter.h"

using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{
		const int32 MaxTextures = 16;

		/** Defines custom material parameters. */
		struct APAPI MaterialCustomParameter
		{
			EffectSystem::CustomEffectParameterType Type;		/** The data type of the parameter. */
			uint Value[16];

			void* RefValue;

			/** The usage of this parameter. Effect check this for auto binding effect parameters. */
			String Usage;


			MaterialCustomParameter() { }
			MaterialCustomParameter(bool value, const String& usage = L"")
				: Type(EffectSystem::CEPT_Boolean), Usage(usage)
			{
				*reinterpret_cast<bool*>(Value) = value;
			}
			MaterialCustomParameter(float value, const String& usage = L"")
				: Type(EffectSystem::CEPT_Float), Usage(usage)
			{
				*reinterpret_cast<float*>(Value) = value;
			}
			MaterialCustomParameter(int value, const String& usage = L"")
				: Type(EffectSystem::CEPT_Integer), Usage(usage)
			{
				*reinterpret_cast<int*>(Value) = value;
			}
			MaterialCustomParameter(Vector2 value, const String& usage = L"")
				: Type(EffectSystem::CEPT_Vector2), Usage(usage)
			{
				*reinterpret_cast<Vector2*>(Value) = value;
			}
			MaterialCustomParameter(Vector4 value, const String& usage = L"")
				: Type(EffectSystem::CEPT_Vector4), Usage(usage)
			{
				*reinterpret_cast<Vector4*>(Value) = value;
			}
		};
		typedef HashMap<String, MaterialCustomParameter> CustomParamTable;
	}
}

#endif