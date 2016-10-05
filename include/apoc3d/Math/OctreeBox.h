#pragma once
#ifndef APOC3D_OCTREEBOX_H
#define APOC3D_OCTREEBOX_H

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

#include "Vector.h"

namespace Apoc3D
{
	namespace Math
	{
		class APAPI OctreeBox
		{
		public:
			OctreeBox() { }

			explicit OctreeBox(float length)
				: Length(length)
			{ }

			OctreeBox(const BoundingBox& aabb);
			OctreeBox(const BoundingSphere& sph);

			void GetBoundingSphere(BoundingSphere& sp) const;

			Vector3 Center = Vector3::Zero;
			float Length = 0;
		};
	}
}
#endif