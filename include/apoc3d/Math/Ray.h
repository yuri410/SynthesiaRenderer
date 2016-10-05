#pragma once
#ifndef APOC3D_RAY_H
#define APOC3D_RAY_H

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

#include "Plane.h"

namespace Apoc3D
{
	namespace Math
	{
		class APAPI Ray
		{
		public:
			/** Specifies the location of the ray's origin. */
			Vector3 Position;

			/** A unit vector specifying the direction in which the ray is pointing. */
			Vector3 Direction;

			Ray() { }
			Ray(const Vector3& position, const Vector3& direction)
				: Position(position), Direction(direction)
			{ }

			bool operator==(const Ray &other) const
			{
				return other.Position == Position && other.Direction == Direction; 
			}
			bool operator!=(const Ray &other) const { return !(*this == other); }

			bool IntersectsTriangle(const Vector3& a, const Vector3& b, const Vector3& c, Vector3& result);

			/** Determines whether a ray intersects the specified object. */
			static bool Intersects(const Ray& ray, const BoundingBox& box, float& distance);
		};

		class RaySegment
		{
		public:
			Vector3 Start;
			Vector3 End;

			RaySegment() { }
			RaySegment(const Vector3& start, const Vector3& end)
				: Start(start), End(end) { }

		};
	}
}
#endif