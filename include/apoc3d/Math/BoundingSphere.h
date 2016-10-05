#pragma once
#ifndef APOC3D_BOUNDINGSPHERE_H
#define APOC3D_BOUNDINGSPHERE_H

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
#include "MathCommon.h"

namespace Apoc3D
{
	namespace Math
	{

		class APAPI BoundingSphere
		{
		public:
			Vector3 Center;		/** Specifies the center point of the sphere. */
			float Radius;		/** The radius of the sphere. */

			BoundingSphere() { }
			BoundingSphere(const Vector3& center, float radius)
			{
				Center = center;
				Radius = radius;
			}

			bool operator==(const BoundingSphere &other) const
			{
				return other.Center == Center && other.Radius == Radius; 
			}
			bool operator!=(const BoundingSphere &other) const { return !(*this == other); }

			/** Determines whether the sphere contains the specified point. */
			bool Contains(const Vector3& vector) const
			{
				float distance = Vector3::DistanceSquared(vector, Center);

				if (distance >= (Radius * Radius))
					return false;

				return true;
			}


			bool IntersectsRayDist(const Ray& ray, float* dist) const;
			bool IntersectsRay(const Ray& ray, Vector3* p1) const;

			bool IntersectsLineSegmenent(const Vector3& start, const Vector3& end, 
				float* dist, Vector3* n, Vector3* pos) const;

			bool IntersectsTriangle(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3* triN,
				Vector3* pos, Vector3* n, float* depth) const;

			/** Determines whether a sphere intersects the specified object. */
			bool Intersects(const BoundingSphere& sphere) const
			{
				float distance = Vector3::DistanceSquared(Center, sphere.Center);
				float totalR = Radius + sphere.Radius;
				totalR *= totalR;

				return (totalR <= distance);
			}

			/**
			 *  Determines whether the sphere contains the specified box.
			 *	@param
			 *		sphere The sphere that will be checked for containment.
			 *		box The box that will be checked for containment.
			 *	@return
			 *		A member of the ContainmentType enumeration indicating whether 
			 * 		the two objects intersect, are contained, or don't meet at all.
			 */
			static ContainmentType Contains(const BoundingSphere& sphere, const BoundingBox& box);

			/** Determines whether the sphere contains the specified sphere.			
			 *	@param
			 * 		sphere The first sphere that will be checked for containment.
			 * 		sphere2 The second sphere that will be checked for containment.
			 * 	@returns
			 * 		A member of the ContainmentType enumeration indicating whether 
			 * 		the two objects intersect, are contained, or don't meet at all.
			 */
			static ContainmentType Contains(const BoundingSphere& sphere1, const BoundingSphere& sphere2)
			{
				float distance = Vector3::Distance(sphere1.Center, sphere2.Center);
				
				float radius = sphere1.Radius;
				float radius2 = sphere2.Radius;

				if (radius + radius < distance)
					return ContainmentType::Disjoint;

				if (radius - radius2 < distance)
					return ContainmentType::Intersects;

				return ContainmentType::Contains;
			}

			

			/** Constructs a BoundingSphere from a given box. */
			static void CreateFromBox(BoundingSphere& res, const BoundingBox& box);

			/** Constructs a BoundingSphere that fully contains the given points. */
			static void CreateFromPoints(BoundingSphere& res, const Vector3* points, int count);
			
			static BoundingSphere CreateFromBox(BoundingBox& box)
			{
				BoundingSphere result;
				CreateFromBox(result, box);
				return result;
			}
			static BoundingSphere CreateFromPoints(const Vector3* points, int count) 
			{
				BoundingSphere result;
				CreateFromPoints(result, points, count);
				return result;
			}

			template <typename T>
			static BoundingSphere CreateFromObjects(const T* objects, int count, FunctorReference<Vector3(const T&)> positionGetter)
			{
				BoundingSphere res;

				Vector3 center = Vector3::Zero;
				for (int i = 0; i < count; i++)
					center += positionGetter(objects[i]);

				if (count > 0)
					center /= (float)count;

				float radius = 0;
				for (int i = 0; i < count; i++)
				{
					float dist = Vector3::DistanceSquared(center, positionGetter(objects[i]));
					if (dist > radius)
						radius = dist;
				}

				res.Center = center;
				res.Radius = sqrtf(radius);

				return res;
			}

			/** Constructs a BoundingSphere that is the as large as the total combined area of the two specified spheres. */
			static void Merge(BoundingSphere& res, const BoundingSphere& sphere1, const BoundingSphere& sphere2);

			/** Determines whether a sphere intersects the specified object. */
			static bool Intersects(const BoundingSphere& sphere, const BoundingBox& box);
			
		};
	}
}

#endif