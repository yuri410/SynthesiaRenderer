#pragma once
#ifndef APOC3D_PLANE_H
#define APOC3D_PLANE_H

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


#include "apoc3d/Common.h"

#include "Matrix.h"
#include "MathCommon.h"

namespace Apoc3D
{
	namespace Math
	{
		class APAPI Plane
		{
		public:
#if APOC3D_MATH_IMPL == APOC3D_SSE
			union
			{
				struct  
				{
					float X;
					float Y;
					float Z;
					float D;
				};
				/**
				 *  The distance of the plane along its normal from the origin.
				 */
				Vector4 Normal;
			};
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			/** The normal vector of the plane. */
			float X;
			float Y;
			float Z;
			/** The distance of the plane along its normal from the origin. */
			float D;

#endif

			Plane(const Plane &another)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				Normal = another.Normal;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				X = another.X; Y = another.Y;
				Z = another.Z; D = another.D;
			#endif
			}

			Plane(float a, float b, float c, float d)
			{
				X = a; Y = b; Z = c;
				D = d;
			}
			Plane()
			{
				X = Y = Z = D = 0;
			}

#if APOC3D_MATH_IMPL == APOC3D_SSE
			Plane(Vector3 v)
			{			
				Normal = v;
				D = 0;
			}
			Plane(Vector3 normal, float d)
			{
				Normal = normal;
				D = d;
			}
			Plane(Vector3 point, Vector3 normal)
			{
				Normal = normal;
				D = -SIMDVec3Dot(normal, point);
			}
			Plane(Vector3 point1, Vector3 point2, Vector3 point3)
			{
				Vector3 p12 = SIMDVecSub(point2, point1);
				Vector3 p13 = SIMDVecSub(point3, point1);
				Vector3 cross = SIMDVec3Cross(p12, p13);
				cross = SIMDVec3Normalize(cross);
				D = -SIMDVec3Dot(cross, point1);
			}
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			Plane(const Vector3& v)
			{	
				X = v.X; Y = v.Y;
				Z = v.Z; D = 0;
			}
			Plane(const Vector3& normal, float d)
			{
				X = normal.X; Y = normal.Y;
				Z = normal.Z; D = d;
			}
			Plane(const Vector3& point, const Vector3& normal)
			{
				X = normal.X; Y = normal.Y;
				Z = normal.Z;
				D = -Vector3::Dot(normal, point);
			}
			Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
			{
				Vector3 p12 = point2 - point1;
				Vector3 p13 = point3 - point1;
				Vector3 cross = Vector3::Cross(p12, p13);
				cross.NormalizeInPlace();

				X = cross.X; Y = cross.Y;
				Z = cross.Z;
				D = -Vector3::Dot(cross, point1);
			}
#endif
			

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/**
			 *  Calculates the dot product of a specified vector and the normal of the plane plus the distance value of the plane.
			 */
			float Dot3(Vector3 a) const
			{
				return SIMDVec3Dot(a, Normal) + D;
			}
			/**
			 *  Calculates the dot product of the specified vector and plane.
			 */
			float Dot4(Vector4 a) const
			{
				return SIMDVec4Dot(a, Normal);
			}
			/**
			 *  Calculates the dot product of the specified vector and the normal of the plane.
			 */
			float DotNormal(Vector3 n) const
			{
				return SIMDVec3Dot(n, Normal);
			}
			/**
			 *  Changes the coefficients of the normal vector of the plane to make it of unit length.
			 */
			void Normalize()
			{
				float mag = SIMDVec3Length(Normal);

				if (mag>EPSILON)
				{
					mag = 1.0f / mag;

					X *= mag;
					Y *= mag;
					Z *= mag;
					D *= mag;
				}
			}
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			/** Calculates the dot product of a specified vector and the normal of the plane plus the distance value of the plane. */
			float Dot3(const Vector3& a) const
			{
				return X * a.X + Y * a.Y + Z * a.Z + D;
			}
			/** Calculates the dot product of the specified vector and plane. */
			float Dot4(const Vector4& a) const
			{
				return X * a.X + Y * a.Y + Z * a.Z + D * a.W;
			}
			/** Calculates the dot product of the specified vector and the normal of the plane. */
			float DotNormal(const Vector3& n) const
			{
				return X * n.X + Y * n.Y + Z * n.Z;
			}
			/** Changes the coefficients of the normal vector of the plane to make it of unit length. */
			void NormalizeInPlace()
			{
				float mag = sqrtf(X * X + Y * Y + Z * Z);
 
				if (mag>EPSILON)
				{
					mag = 1.0f / mag;

					X *= mag;
					Y *= mag;
					Z *= mag;
					D *= mag;
				}
			}
#endif
			

			void ProjectLine(const Vector3& start, const Vector3& end, Vector3& ps, Vector3 pe) const;

			/** Projects a directional vector on to the plane surface */
			Vector3 ProjectVector(const Vector3& vec) const;

			
			/** Finds the intersection between a plane and a line. */
			bool IntersectsLineSegment(const Vector3& start, const Vector3& end, Vector3& intersectPoint) const
			{
				Vector3 dir = end - start;
				float dirLen = dir.Length();

				float cos = DotNormal(dir);

				if (fabs(cos) < EPSILON)
				{
					intersectPoint = Vector3::Zero;
					return false;
				}

				float d1 = Dot3(start);
				float d2 = Dot3(end);

				if (d1 * d2 > 0)
				{
					intersectPoint = Vector3::Zero;
					return false;
				}
				
				cos /= -dirLen;

				float dist = d1 / cos;

				Vector3 off = dir * (dist/dirLen);

				intersectPoint = start + off;
				return true;
			}
			bool IntersectsLineSegment(const RaySegment& ray, Vector3& intersectPoint) const;

			bool IntersectsRay(const Vector3& start, const Vector3& dir, Vector3& intersectionPoint) const
			{
				float cos = DotNormal(dir);

				if (fabs(cos) < EPSILON)
				{
					return false;
				}
				float d1 = Dot3(start);
				float dist = -d1 / cos;

				if (dist>=0)
				{
					Vector3 off = dir * dist;

					intersectionPoint = start + off;
					return true;
				}
				return false;
			}
			bool IntersectsRay(const Ray& ray, Vector3& intersectionPoint) const;


			bool operator==(const Plane &other) const
			{
				return other.X == X && other.Y == Y && other.Z == Z && other.D == D; 
			}
			bool operator!=(const Plane &other) const { return !(*this == other); }

			/** Changes the coefficients of the normal vector of the plane to make it of unit length. */
			static Plane Normalize(const Plane &plane) { Plane np = plane; np.NormalizeInPlace(); return np; }


			/** Scales the plane by the given scaling factor. */
			static Plane Multiply(const Plane &plane, float scale)
			{
				Plane result;
				result.D = plane.D * scale;
				result.X = plane.X * scale;
				result.Y = plane.Y * scale;
				result.Z = plane.Z * scale;
				return result;
			}
			
			/** Scales the plane by the given scaling factor. */
			static void Multiply(Plane& result, const Plane &plane, float scale)
			{				
				result.D = plane.D * scale;
				result.X = plane.X * scale;
				result.Y = plane.Y * scale;
				result.Z = plane.Z * scale;				
			}

			/** Transforms a normalized plane by a quaternion rotation. */
			static Plane Transform(const Plane &plane, const Matrix &transformation)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				Plane result;
				Matrix trans = transformation;
				trans.Inverse();

				result.X = SIMDVec4Dot( transformation.Row1, plane.Normal);
				result.Y = SIMDVec4Dot( transformation.Row2, plane.Normal);
				result.Z = SIMDVec4Dot( transformation.Row3, plane.Normal);
				result.D = SIMDVec4Dot( transformation.Row4, plane.Normal);
				return result;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				Plane result;

				const float& x = plane.X;
				const float& y = plane.Y;
				const float& z = plane.Z;
				const float& d = plane.D;

				Matrix trans = transformation;
				trans.Inverse();
				result.X = x * transformation.M11 + y * transformation.M12 + z * transformation.M13 + d * transformation.M14;
				result.Y = x * transformation.M21 + y * transformation.M22 + z * transformation.M23 + d * transformation.M24;
				result.Z = x * transformation.M31 + y * transformation.M32 + z * transformation.M33 + d * transformation.M34;
				result.D = x * transformation.M41 + y * transformation.M42 + z * transformation.M43 + d * transformation.M44;

				return result;
			#endif
			}

			/** Transforms an array of normalized planes by a quaternion rotation. */
			static void Transform(Plane* result, const Plane* planes, int count, const Matrix &transformation)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				Matrix trans = transformation;
				trans.Inverse();

				for (int i=0;i<count;i++)
				{
					Plane &r = result[i];
					r.X = SIMDVec4Dot( transformation.Row1, planes[i].Normal);
					r.Y = SIMDVec4Dot( transformation.Row2, planes[i].Normal);
					r.Z = SIMDVec4Dot( transformation.Row3, planes[i].Normal);
					r.D = SIMDVec4Dot( transformation.Row4, planes[i].Normal);					
				}
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				Matrix trans = transformation;
				trans.Inverse();

				for (int i=0;i<count;i++)
				{
					const float& x = planes[i].X;
					const float& y = planes[i].Y;
					const float& z = planes[i].Z;
					const float& d = planes[i].D;

					Plane &r = result[i];
					r.X = x * trans.M11 + y * trans.M12 + z * trans.M13 + d * trans.M14;
					r.Y = x * trans.M21 + y * trans.M22 + z * trans.M23 + d * trans.M24;
					r.Z = x * trans.M31 + y * trans.M32 + z * trans.M33 + d * trans.M34;
					r.D = x * trans.M41 + y * trans.M42 + z * trans.M43 + d * trans.M44;					
				}
			#endif
			}

			/** Transforms a normalized plane by a quaternion rotation. */
			static Plane Transform(const Plane &plane, const Quaternion &rotation);

			/** Finds the intersection between a plane and a box. */
			static PlaneIntersectionType Intersects(const Plane &plane, const BoundingBox& box);
		};
	}
}


#endif