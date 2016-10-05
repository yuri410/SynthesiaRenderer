#pragma once
#ifndef APOC3D_MATH_H
#define APOC3D_MATH_H

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

#include "MathCommon.h"
#include "Point.h"
#include "Rectangle.h"

namespace Apoc3D
{
	namespace Math
	{
		uint16 R32ToR16I(uint32 value);
		uint32 R16ToR32I(uint16 value);

		inline uint16 R32ToR16(float value) { return R32ToR16I(reinterpret_cast<const uint32&>(value)); }

		// once a floating point value is formed, bits maybe altered implicitly due to floating point behavior
		inline float R16ToR32(uint16 value)
		{
			uint32 iv = R32ToR16I(value);
			return reinterpret_cast<const float&>(iv);
		}

		inline Point Round(PointF p)
		{
			return Point(Round(p.X), Round(p.Y));
		}

		inline Apoc3D::Math::Rectangle Round(RectangleF r)
		{
			return { Round(r.X), Round(r.Y), Round(r.Width), Round(r.Height) };
		}

	}
}

#endif