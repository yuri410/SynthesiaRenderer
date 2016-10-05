#pragma once
#ifndef APOC3D_QUATERNION_H
#define APOC3D_QUATERNION_H

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
		/**
		 *  Defines a four dimensional mathematical quaternion.
		 */
		class APAPI Quaternion
		{
		public:
			float X;		/** The X component of the quaternion. */
			float Y;		/** The Y component of the quaternion. */
			float Z;		/** The Z component of the quaternion. */
			float W;		/** The W component of the quaternion. */

			static const Quaternion Identity;

			Quaternion() {}

			/**
             *  
             *  @param x The X component of the quaternion.
			 *	@param y The Y component of the quaternion.
			 *	@param z The Z component of the quaternion.
			 *	@param w The W component of the quaternion.
			*/
			Quaternion(float x, float y, float z, float w)
				: X(x), Y(y), Z(z), W(w)
			{
			}

			bool IsIdentity() const
			{
				if (X != 0.0f || Y != 0.0f || Z != 0.0f)
				return false;

				return (W == 1.0f);
			}

			/** Gets the axis components of the quaternion. */
			Vector3 getAxis() const
			{
				float len = LengthSquared();
				if (len > EPSILON)
				{
					return Vector3(X, Y, Z);
				}
				return Vector3::Zero;
			}

			/** Gets the angle of the quaternion. */
			float getAngle() const
			{
				float len = LengthSquared();
				if (len > EPSILON)
				{
					return 2 * acosf(W);
				}
				return 0;
			}

			void GetAxisAngle(Vector3& axis, float& angle) const
			{
				float len = LengthSquared();

				angle = 0;
				if (len > EPSILON)
				{
					axis.X = X;
					axis.Y = Y;
					axis.Z = Z;

					if (fabs(W) < 1)
					{
						angle = 2 * acosf(W);
					}
				}
				else
				{
					axis.X = axis.Y = axis.Z = 0;
				}
			}

			/** Calculates the length of the quaternion. */
			float Length() const { return sqrtf(X * X + Y * Y + Z * Z + W * W); }

			/** Calculates the squared length of the quaternion. */
			float LengthSquared() const { return X * X + Y * Y + Z * Z + W * W; }

			/** Converts the quaternion into a unit quaternion. */
			void Normalize()
			{
				float length = Length();

				if (length>EPSILON)
				{
					length = 1.0f / length;

					X *= length;
					Y *= length;
					Z *= length;
					W *= length;
				}
				else
				{
					X = Y = Z = W = 0;
				}
			}

			/** Conjugates the quaternion. */
			void Conjugate()
			{
				X = -X;
				Y = -Y;
				Z = -Z;
			}

			/** Conjugates and renormalizes the quaternion. */
			void Invert()
			{
				float lengthSq = LengthSquared();
				if (lengthSq>EPSILON)
				{
					X = -X * lengthSq;
					Y = -Y * lengthSq;
					Z = -Z * lengthSq;
					W = W * lengthSq;
				}
				else
				{
					X = Y = Z = W = 0;
				}
			}

			bool operator==(const Quaternion &other) const
			{
				return other.X == X && other.Y == Y && other.Z == Z && other.W == W; 
			}
			bool operator!=(const Quaternion &other) const { return !(*this == other); }


			/** Adds two quaternions. */
			static void Add(Quaternion& r, const Quaternion& left, const Quaternion& right)
			{
				r.X = left.X + right.X;
				r.Y = left.Y + right.Y;
				r.Z = left.Z + right.Z;
				r.W = left.W + right.W;
			}

			/** Subtracts two quaternions. */
			static void Subtract(Quaternion& result, const Quaternion& left, const Quaternion& right)
			{
				result.X = left.X - right.X;
				result.Y = left.Y - right.Y;
				result.Z = left.Z - right.Z;
				result.W = left.W - right.W;
			}

			/** Modulates a quaternion by another. */
			static void Multiply(Quaternion& result, const Quaternion& left, const Quaternion& right)
			{
				const float& rx = right.X;
				const float& ry = right.Y;
				const float& rz = right.Z;
				const float& rw = right.W;
				const float& lx = left.X;
				const float& ly = left.Y;
				const float& lz = left.Z;
				const float& lw = left.W;
				float yz = ry * lz - rz * ly;
				float xz = rz * lx - rx * lz;
				float xy = rx * ly - ry * lx;
				float lengthSq = rx * lx + ry * ly + rz * lz;

				result.X = rx * lw + lx * rw + yz;
				result.Y = ry * lw + ly * rw + xz;
				result.Z = rz * lw + lz * rw + xy;
				result.W = rw * lw - lengthSq;
			}

			/** Scales a quaternion by the given value. */
			static void Multiply(Quaternion& result, const Quaternion& quaternion, float scale)
			{
				result.X = quaternion.X * scale;
				result.Y = quaternion.Y * scale;
				result.Z = quaternion.Z * scale;
				result.W = quaternion.W * scale;
			}

			/** Reverses the direction of a given quaternion. */
			static void Negate(Quaternion& result, const Quaternion& quaternion)
			{
				result.X = -quaternion.X;
				result.Y = -quaternion.Y;
				result.Z = -quaternion.Z;
				result.W = -quaternion.W;
			}

			/** Divides a quaternion by another. */
			static void Divide(Quaternion& result, const Quaternion& left, const Quaternion& right)
			{
				result.X = left.X / right.X;
				result.Y = left.Y / right.Y;
				result.Z = left.Z / right.Z;
				result.W = left.W / right.W;
			}

			/** Concatenates two quaternions. */
			static void Concatenate(Quaternion& r, const Quaternion& left, const Quaternion& right)
			{
				const float rx = right.X;
				const float ry = right.Y;
				const float rz = right.Z;
				const float rw = right.W;
				const float lx = left.X;
				const float ly = left.Y;
				const float lz = left.Z;
				const float lw = left.W;
				float yz = ry * lz - rz * ly;
				float xz = rz * lx - rx * lz;
				float xy = rx * ly - ry * lx;
				float lengthSq = rx * lx + ry * ly + rz * lz;

				r.X = rx * lw + lx * rw + yz;
				r.Y = ry * lw + ly * rw + xz;
				r.Z = rz * lw + lz * rw + xy;
				r.W = rw * lw - lengthSq;
			}

			/** Conjugates a quaternion. */
			static void Conjugate(Quaternion& result, const Quaternion& quaternion)
			{				
				result.X = -quaternion.X;
				result.Y = -quaternion.Y;
				result.Z = -quaternion.Z;
				result.W = quaternion.W;
			}


			/** Calculates the dot product of two quaternions. */
			static float Dot(const Quaternion& left, const Quaternion& right)
			{
				return left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W;
			}

			/** Conjugates and renormalizes the quaternion. */
			static void Invert(Quaternion& result, const Quaternion& quaternion)
			{
				float lengthSq = 1.0f / (quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);

				result.X = -quaternion.X * lengthSq;
				result.Y = -quaternion.Y * lengthSq;
				result.Z = -quaternion.Z * lengthSq;
				result.W = quaternion.W * lengthSq;
			}

			/** Performs a linear interpolation between two quaternion. */
			static void Lerp(Quaternion& result, const Quaternion& start, const Quaternion& end, float amount)
			{				
				float inverse = 1.0f - amount;
				float dot = start.X * end.X + start.Y * end.Y + start.Z * end.Z + start.W * end.W;

				if (dot >= 0.0f)
				{
					result.X = inverse * start.X + amount * end.X;
					result.Y = inverse * start.Y + amount * end.Y;
					result.Z = inverse * start.Z + amount * end.Z;
					result.W = inverse * start.W + amount * end.W;
				}
				else
				{
					result.X = inverse * start.X - amount * end.X;
					result.Y = inverse * start.Y - amount * end.Y;
					result.Z = inverse * start.Z - amount * end.Z;
					result.W = inverse * start.W - amount * end.W;
				}

				float invLength = 1.0f / result.Length();

				result.X *= invLength;
				result.Y *= invLength;
				result.Z *= invLength;
				result.W *= invLength;

			}

			/** Converts the quaternion into a unit quaternion. */
			static void Normalize(Quaternion& result, const Quaternion& quaternion)
			{
				float length = 1.0f / quaternion.Length();
				result.X = quaternion.X * length;
				result.Y = quaternion.Y * length;
				result.Z = quaternion.Z * length;
				result.W = quaternion.W * length;
			}

			/** Creates a quaternion given a rotation and an axis. */
			static void CreateRotationAxis(Quaternion& result, const Vector3& axis, float angle)
			{
				Vector3 axis2 = Vector3::Normalize(axis);

				float half = angle * 0.5f;
				float sin = sinf(half);
				float cos = cosf(half);

				result.X = axis2.X * sin;
				result.Y = axis2.Y * sin;
				result.Z = axis2.Z * sin;
				result.W = cos;

			}

			/** Creates a quaternion given a rotation matrix. */
			static void CreateRotationMatrix(Quaternion& result, const Matrix& matrix);

			/** Creates a quaternion given a yaw, pitch, and roll value. */
			static void CreateRotationYawPitchRoll(Quaternion& result, float yaw, float pitch, float roll)
			{
				float halfRoll = roll * 0.5f;
				float sinRoll = sinf(halfRoll);
				float cosRoll = cosf(halfRoll);
				float halfPitch = pitch * 0.5f;
				float sinPitch = sinf(halfPitch);
				float cosPitch = cosf(halfPitch);
				float halfYaw = yaw * 0.5f;
				float sinYaw = sinf(halfYaw);
				float cosYaw = cosf(halfYaw);

				result.X = cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll;
				result.Y = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
				result.Z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;
				result.W = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
			}

			/** Interpolates between two quaternions, using spherical linear interpolation. */
			static void Slerp(Quaternion& result, const Quaternion& start,  const Quaternion& end, float amount)
			{
				float opposite;
				float inverse;
				float dot = start.X * end.X + start.Y * end.Y + start.Z * end.Z + start.W * end.W;
				bool flag = false;

				if (dot < 0.0f)
				{
					flag = true;
					dot = -dot;
				}

				if (dot > 0.999999f)
				{
					inverse = 1.0f - amount;
					opposite = flag ? -amount : amount;
				}
				else
				{
					float acos = acosf(dot);
					float invSin = (1.0f / sinf(acos));

					inverse = sinf((1.0f - amount) * acos) * invSin;
					opposite = flag ? -sinf(amount * acos) * invSin : sinf(amount * acos) * invSin;
				}

				result.X = inverse * start.X + opposite * end.X;
				result.Y = inverse * start.Y + opposite * end.Y;
				result.Z = inverse * start.Z + opposite * end.Z;
				result.W = inverse * start.W + opposite * end.W;

			}
		};
	}
}

#endif