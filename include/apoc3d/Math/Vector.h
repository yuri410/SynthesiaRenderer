#pragma once
#ifndef APOC3D_VECTOR_H
#define APOC3D_VECTOR_H

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

#include "sse_functions.h"

#define EPSILON 0.00001f
#define _CATMULLROM(value1, value2, value3, value4, amount, squared, cubed) \
		(0.5f * (2.0f * value2 + (value3 - value1) * amount + \
		(2.0f * value1 - 5.0f * value2 + 4.0f * value3 - value4) * squared + \
		(3.0f * value2 - value1 - 3.0f * value3 + value4) * cubed))


namespace Apoc3D
{
	namespace Math
	{
		class Vector2;
		class Vector3;
		class Vector4;

		/** Defines a four component vector. */
		class APAPI Vector4
		{
		public:
			float X;	/** The X component of the vector. */
			float Y;	/** The Y component of the vector. */
			float Z;	/** The Z component of the vector. */
			float W;	/** The W component of the vector. */

			Vector4() { }

			Vector4(const float (&ptr)[4])
				: X(ptr[0]), Y(ptr[1]), Z(ptr[2]), W(ptr[3]) { }

			Vector4(float x, float y, float z, float w)
				: X(x), Y(y), Z(z), W(w) { }

			Vector4(const Vector3& v, float w);

			bool operator==(const Vector4 &other) const		{ return other.X == X && other.Y == Y && other.Z == Z && other.W == W; }
			bool operator!=(const Vector4 &other) const		{ return !(*this == other); }

			operator float*()								{ return &X; }
			operator const float*() const					{ return &X; }

			void operator +=(const Vector4& vec)			{ X += vec.X; Y += vec.Y; Z += vec.Z; W += vec.W; }
			void operator -=(const Vector4& vec)			{ X -= vec.X; Y -= vec.Y; Z -= vec.Z; W -= vec.W; }

			void operator *=(const Vector4& vec)			{ X *= vec.X; Y *= vec.Y; Z *= vec.Z; W *= vec.W; }
			void operator *=(float scalar)					{ X *= scalar; Y *= scalar; Z *= scalar; W *= scalar; }
			void operator /=(float scalar)					{ float inv = 1.0f/scalar; X *= inv; Y *= inv; Z *= inv; W *= inv; }

			Vector4 operator -() const						{ return Vector4(-X, -Y, -Z, -W); }
			Vector4 operator -(const Vector4& vec) const	{ return Vector4(X - vec.X, Y - vec.Y, Z - vec.Z, W - vec.W); }
			Vector4 operator +(const Vector4& vec) const	{ return Vector4(X + vec.X, Y + vec.Y, Z + vec.Z, W + vec.W); }
			Vector4 operator *(const Vector4& vec) const	{ return Vector4(X * vec.X, Y * vec.Y, Z * vec.Z, W * vec.W); }
			Vector4 operator *(float scalar) const			{ return Vector4(X * scalar, Y * scalar, Z * scalar, W * scalar); }
			Vector4 operator /(float scalar) const			{ scalar = 1/scalar; return Vector4(X * scalar, Y * scalar, Z * scalar, W * scalar); }
			
			float operator[] (int idx) const				{ assert(idx<4); return (&X)[idx]; }
			float& operator[] (int idx)						{ assert(idx < 4); return (&X)[idx]; }


			void Load(const float* ptr)						{ X = ptr[0]; Y = ptr[1]; Z = ptr[2]; W = ptr[3]; }
			void Store(float* dest)							{ dest[0] = X; dest[1] = Y; dest[2] = Z; dest[3] = W; }

			/** Calculates the length of the vector. */
			float Length() const							{ return sqrtf(X*X + Y*Y + Z*Z + W*W); }

			/** Calculates the squared length of the vector. */
			float LengthSquared() const						{ return X*X + Y*Y + Z*Z + W*W; }

			/** Converts the vector into a unit vector. */
			void NormalizeInPlace()
			{
				float length = Length();
				if (length < EPSILON)					
				{
					X = Y = Z = W = 0;
					return;
				}
				float inv = 1.0f / length;
				X *= inv; Y *= inv;
				Z *= inv; W *= inv;
			}
			Vector4 Normalized() const { Vector4 copy = *this; copy.NormalizeInPlace(); return copy; }

			/** Reverses the direction of the vector. */
			void NegateInPlace() { X = -X; Y = -Y; Z = -Z; W = -W; }
			Vector4 Negate() const { Vector4 copy = *this; copy.NegateInPlace(); return copy; }

			/** Calculates the distance between two vectors. */
			float Distance(const Vector4& vec) const
			{
				float x = X - vec.X;
				float y = Y - vec.Y;
				float z = Z - vec.Z;
				float w = W - vec.W;

				return sqrtf(x*x + y*y + z*z + w*w);
			}

			float DistanceSquared(const Vector4& vec) const
			{
				float x = X - vec.X;
				float y = Y - vec.Y;
				float z = Z - vec.Z;
				float w = W - vec.W;

				return x*x + y*y + z*z + w*w;
			}

			/** Calculates the dot product of two vectors. */
			float Dot(const Vector4& right) const { return X * right.X + Y * right.Y + Z * right.Z + W * right.W; }

			uint32 GetHashCode() const
			{
				return reinterpret_cast<const uint32&>(X) ^ reinterpret_cast<const uint32&>(Y) ^
					reinterpret_cast<const uint32&>(Z) ^ reinterpret_cast<const uint32&>(W); 
			}

			void Parse(const String& str, bool loosy = false);
			String ToTextString(int precision = -1) const;
			String ToParsableString(int precision = -1) const;
			

			static Vector4 Set(float s)				{ return Vector4(s, s, s, s); }
			static Vector4 Set(const float* ptr)	{ Vector4 v; v.Load(ptr); return v; }


			/** Calculates the length of the vector. */
			static float Length(const Vector4& v)				{ return sqrtf(v.X*v.X + v.Y*v.Y + v.Z*v.Z + v.W*v.W); }

			/** Calculates the squared length of the vector. */
			static float LengthSquared(const Vector4& v)		{ return v.X*v.X + v.Y*v.Y + v.Z*v.Z + v.W*v.W; }

			/** Converts the vector into a unit vector. */
			static Vector4 Normalize(const Vector4& vector)
			{
				float length = Length(vector);
				if (length < EPSILON)					
				{
					return Zero;
				}
				float inv = 1.0f / length;
				return Vector4(vector.X * inv, vector.Y * inv, vector.Z * inv, vector.W * inv);
			}

			/** Adds two vectors. */
			static Vector4 Add(const Vector4& a, const Vector4& b)					{ return Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W); }

			/** Subtracts two vectors. */
			static Vector4 Subtract(const Vector4& a, const Vector4& b)				{ return Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);  }

			/** Scales a vector by the given value. */
			static Vector4 Multiply(const Vector4& v, float scale)					{ return Vector4(v.X * scale, v.Y * scale, v.Z * scale, v.W * scale); }

			/** Modulates a vector by another. */
			static Vector4 Modulate(const Vector4& a, const Vector4& b)				{ return Vector4(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W); }

			static Vector4 Divide(const Vector4& v, float scale)					{ float inv = 1/scale; return Vector4(v.X * inv, v.Y * inv, v.Z * inv, v.W * inv); }

			/** Reverses the direction of a given vector. */
			static Vector4 Negate(const Vector4& v)									{ return Vector4(-v.X, -v.Y, -v.Z, -v.W); }

			/** 
			 * Returns a Vector4 containing the 4D Cartesian coordinates of 
			 * a point specified in Barycentric coordinates relative to a 4D triangle.
			 */
			static Vector4 Barycentric(const Vector4& value1, const Vector4& value2, 
				const Vector4& value3, float amount1, float amount2)
			{
				Vector4 vector;
				vector.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
				vector.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
				vector.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
				vector.W = value1.W + amount1 * (value2.W - value1.W) + amount2 * (value3.W - value1.W);
				return vector;
			}

			/** Performs a Catmull-Rom interpolation using the specified positions. */
			static Vector4 CatmullRom(const Vector4& value1, const Vector4& value2, 
				const Vector4& value3, const Vector4& value4, float amount)
			{
				Vector4 vector;
				float squared = amount * amount;
				float cubed = amount * squared;

				vector.X = _CATMULLROM(value1.X, value2.X, value3.X, value4.X, amount, squared, cubed);
				vector.Y = _CATMULLROM(value1.Y, value2.Y, value3.Y, value4.Y, amount, squared, cubed);
				vector.Z = _CATMULLROM(value1.Z, value2.Z, value3.Z, value4.Z, amount, squared, cubed);
				vector.W = _CATMULLROM(value1.W, value2.W, value3.W, value4.W, amount, squared, cubed);

				return vector;
			}

			/** Performs a Hermite spline interpolation. */
			static Vector4 Hermite(const Vector4& value1, const Vector4& tangent1, 
				const Vector4& value2, const Vector4& tangent2, float amount)
			{
				Vector4 vector;
				float squared = amount * amount;
				float cubed = amount * squared;
				float part1 = 2.0f * cubed - 3.0f * squared + 1.0f;
				float part2 = -2.0f * cubed + 3.0f * squared;
				float part3 = cubed - 2.0f * squared + amount;
				float part4 = cubed - squared;

				vector.X = value1.X * part1 + value2.X * part2 + tangent1.X * part3 + tangent2.X * part4;
				vector.Y = value1.Y * part1 + value2.Y * part2 + tangent1.Y * part3 + tangent2.Y * part4;
				vector.Z = value1.Z * part1 + value2.Z * part2 + tangent1.Z * part3 + tangent2.Z * part4;
				vector.W = value1.W * part1 + value2.W * part2 + tangent1.W * part3 + tangent2.W * part4;

				return vector;
			}

			/** Restricts a value to be within a specified range. */
			static Vector4 Clamp(const Vector4& value, const Vector4& min, const Vector4& max)
			{
				float x = value.X;
				x = (x > max.X) ? max.X : x;
				x = (x < min.X) ? min.X : x;

				float y = value.Y;
				y = (y > max.Y) ? max.Y : y;
				y = (y < min.Y) ? min.Y : y;

				float z = value.Z;
				z = (z > max.Z) ? max.Z : z;
				z = (z < min.Z) ? min.Z : z;

				float w = value.W;
				w = (w > max.W) ? max.W : w;
				w = (w < min.W) ? min.W : w;

				return Vector4(x, y, z, w);
			}
			
			/** Performs a linear interpolation between two vectors. */
			static Vector4 Lerp(const Vector4& start, const Vector4& end, float amount)
			{
				Vector4 vector;

				vector.X = start.X + (end.X - start.X) * amount;
				vector.Y = start.Y + (end.Y - start.Y) * amount;
				vector.Z = start.Z + (end.Z - start.Z) * amount;
				vector.W = start.W + (end.W - start.W) * amount;

				return vector;
			}

			/** Performs a cubic interpolation between two vectors. */
			static Vector4 SmoothStep(const Vector4& start, const Vector4& end, float amount)
			{
				Vector4 vector;

				amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
				amount = (amount * amount) * (3.0f - (2.0f * amount));

				vector.X = start.X + (end.X - start.X) * amount;
				vector.Y = start.Y + (end.Y - start.Y) * amount;
				vector.Z = start.Z + (end.Z - start.Z) * amount;
				vector.W = start.W + (end.W - start.W) * amount;

				return vector;
			}

			/** Calculates the distance between two vectors. */
			static float Distance(const Vector4& value1, const Vector4& value2)
			{
				float x = value1.X - value2.X;
				float y = value1.Y - value2.Y;
				float z = value1.Z - value2.Z;
				float w = value1.W - value2.W;

				return sqrtf(x * x + y * y + z * z + w * w);
			}

			/** Calculates the squared distance between two vectors. */
			static float DistanceSquared(const Vector4& value1, const Vector4& value2)
			{
				float x = value1.X - value2.X;
				float y = value1.Y - value2.Y;
				float z = value1.Z - value2.Z;
				float w = value1.W - value2.W;

				return x * x + y * y + z * z + w * w;
			}

			/** Calculates the dot product of two vectors. */
			static float Dot(const Vector4& a, const Vector4& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W; }

			/** Returns a vector containing the smallest components of the specified vectors. */
			static Vector4 Minimize(const Vector4& left, const Vector4& right)
			{
				Vector4 vector;
				vector.X = (left.X < right.X) ? left.X : right.X;
				vector.Y = (left.Y < right.Y) ? left.Y : right.Y;
				vector.Z = (left.Z < right.Z) ? left.Z : right.Z;
				vector.W = (left.W < right.W) ? left.W : right.W;
				return vector;
			}

			/** Returns a vector containing the largest components of the specified vectors. */
			static Vector4 Maximize(const Vector4& left, const Vector4& right)
			{
				Vector4 vector;
				vector.X = (left.X > right.X) ? left.X : right.X;
				vector.Y = (left.Y > right.Y) ? left.Y : right.Y;
				vector.Z = (left.Z > right.Z) ? left.Z : right.Z;
				vector.W = (left.W > right.W) ? left.W : right.W;
				return vector;
			}

			
			/** Transforms a 3D vector by the given Matrix */
			static Vector4 Transform(const Vector4& vector, const Matrix& transform);
			
			/** a Vector4 with all of its components set to zero. */
			static const Vector4 Zero;
			
			static const Vector4 UnitX;		/** X unit Vector4 (1, 0, 0, 0). */
			static const Vector4 UnitY;		/** Y unit Vector4 (0, 1, 0, 0). */
			static const Vector4 UnitZ;		/** Z unit Vector4 (0, 0, 1, 0). */
			static const Vector4 UnitW;		/** W unit Vector4 (0, 0, 0, 1). */
			static const Vector4 One;

		};

		/** Defines a two component vector. */
		class APAPI Vector2
		{
		public:
			float X;	/** the X component of the vector */
			float Y;	/** the Y component of the vector */

			Vector2() { }

			Vector2(float x, float y)
				: X(x), Y(y) { }

			Vector2(const PointF& p);

			bool operator==(const Vector2 &other) const { return other.X == X && other.Y == Y;  }
			bool operator!=(const Vector2 &other) const { return !(*this == other); }

			operator float*()							{ return &X; }
			operator const float*() const				{ return &X; }
			
			void operator +=(const Vector2& vec)		{ X += vec.X; Y += vec.Y; }
			void operator -=(const Vector2& vec)		{ X -= vec.X; Y -= vec.Y; }

			void operator *=(const Vector2& vec)		{ X *= vec.X; Y *= vec.Y; }
			void operator *=(float scalar)				{ X *= scalar; Y *= scalar; }
			void operator /=(float scalar)				{ float inv = 1.0f/scalar; X *= inv; Y *= inv; }

			Vector2 operator -() const						{ return Vector2(-X, -Y); }
			Vector2 operator -(const Vector2& vec) const	{ return Vector2(X - vec.X, Y - vec.Y); }
			Vector2 operator +(const Vector2& vec) const	{ return Vector2(X + vec.X, Y + vec.Y); }
			Vector2 operator *(const Vector2& vec) const	{ return Vector2(X * vec.X, Y * vec.Y); }
			Vector2 operator *(float scalar) const			{ return Vector2(X * scalar, Y * scalar); }
			Vector2 operator /(float scalar) const			{ scalar = 1/scalar; return Vector2(X * scalar, Y * scalar); }
			
			float operator[] (int idx) const	{ assert(idx<2); return (&X)[idx]; }
			float& operator[] (int idx)			{ assert(idx < 4); return (&X)[idx]; }

			void Load(const float* ptr)			{ X = ptr[0]; Y = ptr[1]; }
			void Store(float* dest) const		{ dest[0] = X; dest[1] = Y; }


			/** Calculates the length of the vector. */
			float Length() const				{ return sqrtf(X*X + Y*Y); }

			/** Calculates the squared length of the vector. */
			float LengthSquared() const			{ return X*X + Y*Y; }

			/** Converts the vector into a unit vector. */
			void NormalizeInPlace()
			{
				float length = Length();
				if (length < EPSILON)					
				{
					X = Y = 0;
					return;
				}
				float invLen = 1.0f / length;
				X *= invLen;
				Y *= invLen;
			}
			Vector2 Normalized() const { Vector2 copy = *this; copy.NormalizeInPlace(); return copy; }

			/** Reverses the direction of the vector. */
			void NegateInPlace() { X = -X; Y = -Y; }
			Vector2 Negate() const { Vector2 copy = *this; copy.NegateInPlace(); return copy; }

			float Cross(const Vector2& b) const { return X * b.Y - b.X * Y; }


			/** Calculates the distance between two vectors. */
			float Distance(const Vector2& vec) const
			{
				float x = X - vec.X;
				float y = Y - vec.Y;

				return sqrtf(x * x + y * y);
			}

			/** Calculates the dot product of two vectors. */
			float Dot(const Vector2& right) const { return X * right.X + Y * right.Y; }

			uint32 GetHashCode() const { return reinterpret_cast<const uint32&>(X) ^ reinterpret_cast<const uint32&>(Y); }


			void Parse(const String& str, bool loosy = false);
			String ToTextString(int precision = -1) const;
			String ToParsableString(int precision = -1) const;

			
			static Vector2 Set(float s)				{ return Vector2(s, s); }
			static Vector2 Set(const float* ptr)	{ Vector2 v; v.Load(ptr); return v; }

			/** Calculates the length of the vector. */
			static float Length(const Vector2& v)						{ return sqrtf(v.X*v.X + v.Y*v.Y); }

			/** Calculates the squared length of the vector. */
			static float LengthSquared(const Vector2& v)				{ return v.X*v.X + v.Y*v.Y; }

			/** Converts the vector into a unit vector. */
			static Vector2 Normalize(const Vector2& vector)				{ return vector.Normalized(); }

			/** Adds two vectors. */
			static Vector2 Add(const Vector2& left, const Vector2& right)			{ return Vector2(left.X + right.X, left.Y + right.Y); }
			/** Subtracts two vectors. */
			static Vector2 Subtract(const Vector2& left, const Vector2& right)		{ return Vector2(left.X - right.X, left.Y - right.Y); }
			/** Scales a vector by the given value. */
			static Vector2 Multiply(const Vector2& value, float scale)				{ return Vector2(value.X * scale, value.Y * scale); }
			/** Modulates a vector by another. */
			static Vector2 Modulate(const Vector2& left, const Vector2& right)		{ return Vector2(left.X * right.X, left.Y * right.Y); }
			
			static Vector2 Divide(const Vector2& value, float scale)				{ float inv = 1.0f/scale; return Vector2(value.X * inv, value.Y * inv); }
			static float Cross(const Vector2& a, const Vector2& b)					{ return a.X * b.Y - b.X * a.Y; }

			/** Reverses the direction of a given vector. */
			static Vector2 Negate(const Vector2& value)								{ return Vector2(-value.X, -value.Y); }

			/**
			 * Returns a Vector2 containing the 2D Cartesian coordinates of a point 
			 * specified in Barycentric coordinates relative to a 2D triangle.
			 */
			static Vector2 Barycentric(const Vector2& value1, const Vector2& value2, const Vector2& value3, float amount1, float amount2)
			{
				Vector2 vector;
				vector.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
				vector.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
				return vector;
			}

			/** Performs a Catmull-Rom interpolation using the specified positions. */
			static Vector2 CatmullRom(const Vector2& value1, const Vector2& value2, const Vector2& value3, const Vector2& value4, float amount)
			{
				Vector2 vector;
				float squared = amount * amount;
				float cubed = amount * squared;

				vector.X = _CATMULLROM(value1.X, value2.X, value3.X, value4.X, amount, squared, cubed);
				vector.Y = _CATMULLROM(value1.Y, value2.Y, value3.Y, value4.Y, amount, squared, cubed);
				
				return vector;
			}

			/** Performs a Hermite spline interpolation. */
			static Vector2 Hermite(const Vector2& value1, const Vector2& tangent1, const Vector2& value2, const Vector2& tangent2, float amount)
			{
				Vector2 vector;
				float squared = amount * amount;
				float cubed = amount * squared;
				float part1 = 2.0f * cubed - 3.0f * squared + 1.0f;
				float part2 = -2.0f * cubed + 3.0f * squared;
				float part3 = cubed - 2.0f * squared + amount;
				float part4 = cubed - squared;

				vector.X = value1.X * part1 + value2.X * part2 + tangent1.X * part3 + tangent2.X * part4;
				vector.Y = value1.Y * part1 + value2.Y * part2 + tangent1.Y * part3 + tangent2.Y * part4;

				return vector;
			}

			/** Performs a linear interpolation between two vectors. */
			static Vector2 Lerp(const Vector2& start, const Vector2& end, float amount)
			{
				Vector2 vector;

				vector.X = start.X + (end.X - start.X) * amount;
				vector.Y = start.Y + (end.Y - start.Y) * amount;

				return vector;
			}

			/** Performs a cubic interpolation between two vectors. */
			static Vector2 SmoothStep(const Vector2& start, const Vector2& end, float amount)
			{
				Vector2 vector;

				amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
				amount = (amount * amount) * (3.0f - (2.0f * amount));

				vector.X = start.X + (end.X - start.X) * amount;
				vector.Y = start.Y + (end.Y - start.Y) * amount;

				return vector;
			}

			/** Restricts a value to be within a specified range. */
			static Vector2 Clamp(const Vector2& value, const Vector2& min, const Vector2& max)
			{
				float x = value.X;
				x = (x > max.X) ? max.X : x;
				x = (x < min.X) ? min.X : x;

				float y = value.Y;
				y = (y > max.Y) ? max.Y : y;
				y = (y < min.Y) ? min.Y : y;

				return Vector2(x, y);
			}

			/** Calculates the distance between two vectors. */
			static float Distance(const Vector2& value1, const Vector2& value2)
			{
				float x = value1.X - value2.X;
				float y = value1.Y - value2.Y;

				return sqrtf(x * x + y * y);
			}

			/** Calculates the squared distance between two vectors. */
			static float DistanceSquared(const Vector2& value1, const Vector2& value2)
			{
				float x = value1.X - value2.X;
				float y = value1.Y - value2.Y;

				return x * x + y * y;
			}

			/** Calculates the dot product of two vectors. */
			static float Dot(const Vector2& left, const Vector2& right) { return left.X * right.X + left.Y * right.Y; }

			/** Returns the reflection of a vector off a surface that has the specified normal. */
			static Vector2 Reflect(const Vector2& vector, const Vector2& normal)
			{
				Vector2 vector2;
				float dot = vector.X * normal.X + vector.Y * normal.Y;
				vector2.X = vector.X - 2.0f * dot * normal.X;
				vector2.Y = vector.Y - 2.0f * dot * normal.Y;
				return vector2;
			}

			/** Returns a vector containing the smallest components of the specified vectors. */
			static Vector2 Minimize(const Vector2& left, const Vector2& right)
			{
				Vector2 vector;
				vector.X = (left.X < right.X) ? left.X : right.X;
				vector.Y = (left.Y < right.Y) ? left.Y : right.Y;
				return vector;
			}

			/** Returns a vector containing the largest components of the specified vectors. */
			static Vector2 Maximize(const Vector2& left, const Vector2& right)
			{
				Vector2 vector;
				vector.X = (left.X > right.X) ? left.X : right.X;
				vector.Y = (left.Y > right.Y) ? left.Y : right.Y;
				return vector;
			}

			static const Vector2 Zero;		/** a Vector2 with all of its components set to zero. */
			static const Vector2 UnitX;		/** X unit Vector2 (1, 0). */
			static const Vector2 UnitY;		/** Y unit Vector2 (0, 1). */

			static const Vector2 One;
		};

		/** Defines a three component vector. */
		class APAPI Vector3
		{
		public:
			float X;
			float Y;
			float Z;

			Vector3() { }
			Vector3(const float (&ptr)[3])
				: X(ptr[0]), Y(ptr[1]), Z(ptr[2]) { }
			
			Vector3(float x, float y, float z)
				: X(x), Y(y), Z(z) { }

			bool operator==(const Vector3 &other) const { return other.X == X && other.Y == Y && other.Z == Z;  }
			bool operator!=(const Vector3 &other) const { return !(*this == other); }

			operator float*()							{ return &X; }
			operator const float*() const				{ return &X; }

			void operator +=(const Vector3& vec)		{ X += vec.X; Y += vec.Y; Z += vec.Z; }
			void operator -=(const Vector3& vec)		{ X -= vec.X; Y -= vec.Y; Z -= vec.Z; }

			void operator *=(const Vector3& vec)		{ X *= vec.X; Y *= vec.Y; Z *= vec.Z; }
			void operator *=(float scalar)				{ X *= scalar; Y *= scalar; Z *= scalar; }
			void operator /=(float scalar)				{ float inv = 1.0f/scalar; X *= inv; Y *= inv; Z *= inv; }

			Vector3 operator -() const						{ return Vector3(-X, -Y, -Z); }
			Vector3 operator -(const Vector3& vec) const	{ return Vector3(X - vec.X, Y - vec.Y, Z - vec.Z); }
			Vector3 operator +(const Vector3& vec) const	{ return Vector3(X + vec.X, Y + vec.Y, Z + vec.Z); }
			Vector3 operator *(const Vector3& vec) const	{ return Vector3(X * vec.X, Y * vec.Y, Z * vec.Z); }
			Vector3 operator *(float scalar) const			{ return Vector3(X * scalar, Y * scalar, Z * scalar); }
			Vector3 operator /(float scalar) const			{ scalar = 1/scalar; return Vector3(X * scalar, Y * scalar, Z * scalar); }
			
			float operator[] (int idx) const		{ assert(idx<3); return (&X)[idx]; }
			float& operator[] (int idx)				{ assert(idx < 3); return (&X)[idx]; }

			void Load(const float* ptr)				{ X = ptr[0]; Y = ptr[1]; Z = ptr[2]; }
			void Store(float* dest) const			{ dest[0] = X; dest[1] = Y; dest[2] = Z; }

			/** Calculates the length of the vector. */
			float Length() const					{ return sqrtf(X*X + Y*Y + Z*Z); }

			/** Calculates the squared length of the vector. */
			float LengthSquared() const				{ return X*X + Y*Y + Z*Z; }

			/** Converts the vector into a unit vector. */
			void NormalizeInPlace()
			{
				float length = Length();
				if (length < EPSILON)					
				{
					X = Y = Z = 0;
					return;
				}
				float invLen = 1.0f / length;
				X *= invLen; Y *= invLen; Z *= invLen;
			}
			Vector3 Normalized() const { Vector3 copy = *this; copy.NormalizeInPlace(); return copy; }

			/** Reverses the direction of the vector. */
			void NegateInPlace() { X = -X; Y = -Y; Z = -Z; }
			Vector3 Negate() const { Vector3 copy = *this; copy.NegateInPlace(); return copy; }

			
			Vector3 Cross(const Vector3& b) const 
			{
				Vector3 result;
				result.X = Y * b.Z - Z * b.Y;
				result.Y = Z * b.X - X * b.Z;
				result.Z = X * b.Y - Y * b.X;
				return result;
			}

			/** Calculates the distance between two vectors. */
			float Distance(const Vector3& vec) const
			{
				float x = X - vec.X;
				float y = Y - vec.Y;
				float z = Z - vec.Z;

				return sqrtf(x * x + y * y + z * z);
			}
			float DistanceSquared(const Vector3& vec) const
			{
				float x = X - vec.X;
				float y = Y - vec.Y;
				float z = Z - vec.Z;

				return x * x + y * y + z * z;
			}

			/**
			 *  Calculates the dot product of two vectors.
			 */
			float Dot(const Vector3& right) const	{ return X * right.X + Y * right.Y + Z * right.Z; }

			uint32 GetHashCode() const { return reinterpret_cast<const uint32&>(X) ^ reinterpret_cast<const uint32&>(Y) ^ reinterpret_cast<const uint32&>(Z); }

			void Parse(const String& str, bool loosy = false);
			String ToTextString(int precision = -1) const;
			String ToParsableString(int precision = -1) const;

			
			static Vector3 Set(float s)				{ return Vector3(s, s, s); }
			static Vector3 Set(const float* ptr)	{ Vector3 v; v.Load(ptr); return v; }


			/** Calculates the length of the vector. */
			static float Length(const Vector3& v)					{ return sqrtf(v.X*v.X + v.Y*v.Y + v.Z*v.Z); }

			/** Calculates the squared length of the vector. */
			static float LengthSquared(const Vector3& v)			{ return v.X*v.X + v.Y*v.Y + v.Z*v.Z; }

			static Vector3 Normalize(const Vector3& vector)			{ return vector.Normalized(); }

			/** Adds two vectors. */
			static Vector3 Add(const Vector3& left, const Vector3& right)		{ return Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z); }

			/** Subtracts two vectors. */
			static Vector3 Subtract(const Vector3& left, const Vector3& right)		{ return Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z); }

			/** Scales a vector by the given value. */
			static Vector3 Multiply(const Vector3& value, float scale)				{ return Vector3(value.X * scale, value.Y * scale, value.Z * scale); }

			/** Modulates a vector by another. */
			static Vector3 Modulate(const Vector3& left, const Vector3& right)		{ return Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z); }

			/** Scales a vector by the given value. */
			static Vector3 Divide(const Vector3& value, float scale)				{ float inv = 1/scale; return Vector3(value.X * inv, value.Y * inv, value.Z * inv); }

			/** Reverses the direction of a given vector. */
			static Vector3 Negate(const Vector3& value)								{ return Vector3(-value.X, -value.Y, -value.Z); }

			/**
			 * Returns a Vector3 containing the 3D Cartesian coordinates of 
			 * a point specified in Barycentric coordinates relative to a 3D triangle.
			 */
			static Vector3 Barycentric(const Vector3& value1, const Vector3& value2, const Vector3& value3, float amount1, float amount2)
			{
				Vector3 vector;
				vector.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
				vector.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
				vector.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
				return vector;
			}

			/** Performs a Catmull-Rom interpolation using the specified positions. */
			static Vector3 CatmullRom(const Vector3& value1, Vector3 value2, const Vector3& value3, const Vector3& value4, float amount)
			{
				Vector3 vector;
				float squared = amount * amount;
				float cubed = amount * squared;

				vector.X = _CATMULLROM(value1.X, value2.X, value3.X, value4.X, amount, squared, cubed);
				vector.Y = _CATMULLROM(value1.Y, value2.Y, value3.Y, value4.Y, amount, squared, cubed);
				vector.Z = _CATMULLROM(value1.Z, value2.Z, value3.Z, value4.Z, amount, squared, cubed);

				return vector;
			}
			/** Performs a Hermite spline interpolation. */
			static Vector3 Hermite(const Vector3& value1, const Vector3& tangent1, const Vector3& value2, const Vector3& tangent2, float amount)
			{
				Vector3 vector;
				float squared = amount * amount;
				float cubed = amount * squared;
				float part1 = 2.0f * cubed - 3.0f * squared + 1.0f;
				float part2 = -2.0f * cubed + 3.0f * squared;
				float part3 = cubed - 2.0f * squared + amount;
				float part4 = cubed - squared;

				vector.X = value1.X * part1 + value2.X * part2 + tangent1.X * part3 + tangent2.X * part4;
				vector.Y = value1.Y * part1 + value2.Y * part2 + tangent1.Y * part3 + tangent2.Y * part4;
				vector.Z = value1.Z * part1 + value2.Z * part2 + tangent1.Z * part3 + tangent2.Z * part4;

				return vector;
			}

			/** Performs a linear interpolation between two vectors. */
			static Vector3 Lerp(const Vector3& start, const Vector3& end, float amount)
			{
				Vector3 vector;

				vector.X = start.X + (end.X - start.X) * amount;
				vector.Y = start.Y + (end.Y - start.Y) * amount;
				vector.Z = start.Z + (end.Z - start.Z) * amount;

				return vector;
			}

			/** Performs a cubic interpolation between two vectors. */
			static Vector3 SmoothStep(const Vector3& start, const Vector3& end, float amount)
			{
				Vector3 vector;

				amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
				amount = amount * amount * (3.0f - 2.0f * amount);

				vector.X = start.X + (end.X - start.X) * amount;
				vector.Y = start.Y + (end.Y - start.Y) * amount;
				vector.Z = start.Z + (end.Z - start.Z) * amount;

				return vector;
			}

			/** Restricts a value to be within a specified range. */
			static Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max)
			{
				float x = value.X;
				x = (x > max.X) ? max.X : x;
				x = (x < min.X) ? min.X : x;

				float y = value.Y;
				y = (y > max.Y) ? max.Y : y;
				y = (y < min.Y) ? min.Y : y;

				float z = value.Z;
				z = (z > max.Z) ? max.Z : z;
				z = (z < min.Z) ? min.Z : z;

				return Vector3(x, y, z);
			}

			/** Calculates the distance between two vectors. */
			static float Distance(const Vector3& value1, const Vector3& value2)
			{
				float x = value1.X - value2.X;
				float y = value1.Y - value2.Y;
				float z = value1.Z - value2.Z;

				return sqrtf(x * x + y * y + z * z);
			}
			/** Calculates the squared distance between two vectors. */
			static float DistanceSquared(const Vector3& value1, const Vector3& value2)
			{
				float x = value1.X - value2.X;
				float y = value1.Y - value2.Y;
				float z = value1.Z - value2.Z;

				return x * x + y * y + z * z;
			}

			/** Calculates the dot product of two vectors. */
			static float Dot(const Vector3& left, const Vector3& right) { return left.X * right.X + left.Y * right.Y + left.Z * right.Z; }

			/** Calculates the cross product of two vectors. */
			static Vector3 Cross(const Vector3& left, const Vector3& right)
			{
				Vector3 result;
				result.X = left.Y * right.Z - left.Z * right.Y;
				result.Y = left.Z * right.X - left.X * right.Z;
				result.Z = left.X * right.Y - left.Y * right.X;
				return result;
			}

			/** Returns the reflection of a vector off a surface that has the specified normal. */
			static Vector3 Reflect(const Vector3& vector, const Vector3& normal)
			{
				Vector3 result;
				float dot = vector.X * normal.X + vector.Y * normal.Y + vector.Z * normal.Z;

				result.X = vector.X - 2.0f * dot * normal.X;
				result.Y = vector.Y - 2.0f * dot * normal.Y;
				result.Z = vector.Z - 2.0f * dot * normal.Z;

				return result;
			}


			/** Returns a vector containing the smallest components of the specified vectors. */
			static Vector3 Minimize(const Vector3& left, const Vector3& right)
			{
				Vector3 vector;
				vector.X = (left.X < right.X) ? left.X : right.X;
				vector.Y = (left.Y < right.Y) ? left.Y : right.Y;
				vector.Z = (left.Z < right.Z) ? left.Z : right.Z;
				return vector;
			}
			/** Returns a vector containing the largest components of the specified vectors. */
			static Vector3 Maximize(const Vector3& left, const Vector3& right)
			{
				Vector3 vector;
				vector.X = (left.X > right.X) ? left.X : right.X;
				vector.Y = (left.Y > right.Y) ? left.Y : right.Y;
				vector.Z = (left.Z > right.Z) ? left.Z : right.Z;
				return vector;
			}

			/** Transforms a 3D vector by the given Matrix */
			static Vector4 Transform(const Vector3& vector, const Matrix& transform);
			/** Performs a coordinate transformation using the given Matrix. can not project */
			static Vector3 TransformSimple(const Vector3& vector, const Matrix& transform);
			/** Performs a coordinate transformation using the given Matrix */
			static Vector3 TransformCoordinate(const Vector3& vector, const Matrix& transform);
			/** Performs a normal transformation using the given Matrix */
			static Vector3 TransformNormal(const Vector3& vector, const Matrix& transform);

			static bool IsLess(const Vector3& left, const Vector3& right)			{ return left.X <  right.X && left.Y <  right.Y && left.Z <  right.Z; }
			static bool IsLessEqual(const Vector3& left, const Vector3& right)		{ return left.X <= right.X && left.Y <= right.Y && left.Z <= right.Z; }
			static bool IsGreater(const Vector3& left, const Vector3& right)		{ return left.X >  right.X && left.Y >  right.Y && left.Z >  right.Z; }
			static bool IsGreaterEqual(const Vector3& left, const Vector3& right)	{ return left.X >= right.X && left.Y >= right.Y && left.Z >= right.Z; }

			static const Vector3 Zero;		/** a Vector3 with all of its components set to zero. */
			static const Vector3 UnitX;		/** X unit Vector3 (1, 0, 0). */
			static const Vector3 UnitY;		/** Y unit Vector3 (0, 1, 0). */
			static const Vector3 UnitZ;		/** Z unit Vector3 (0, 0, 1). */
			static const Vector3 One;
		};
		
		inline Vector4::Vector4(const Vector3& v, float w)
			: X(v.X), Y(v.Y), Z(v.Z), W(w)
		{ }

		class APAPI Vector2Utils
		{
		public:
#if APOC3D_MATH_IMPL == APOC3D_SSE

			static const float* GetElementAddress(const Vector2& v) { return reinterpret_cast<const float*>(&v); }
			static float GetX(const Vector2& v) { return *(reinterpret_cast<const float*>(&v)+0); }
			static float GetY(const Vector2& v) { return *(reinterpret_cast<const float*>(&v) + 1); }
			static float& GetX(Vector2& v) { return *(reinterpret_cast<float*>(&v)+0); }
			static float& GetY(Vector2& v) { return *(reinterpret_cast<float*>(&v) + 1); }
			static float& GetElement(Vector2& v, int i) { return *(reinterpret_cast<float*>(&v) + i); }
			static void Store(Vector2 v, float* dest)
			{
				SSEVecLoader buffer;
				_mm_store_ps(reinterpret_cast<float*>(&buffer), v);
				dest[0] = buffer.X; dest[1] = buffer.Y; dest[2] = dest[3] = 0;
			}
			static Vector2 LDVectorPtr(const float* v)
			{
				const SSEVecLoader buffer = { v[0], v[1], 0, 0};
				return _mm_load_ps(reinterpret_cast<const float*>(&buffer));
			}
			static Vector2 LDVector(float v)
			{
				return _mm_set1_ps(v);
			}
			static Vector2 LDVector(float x, float y)
			{
				const SSEVecLoader buffer = { x, y, 0, 0};
				__m128 r = _mm_load_ps(reinterpret_cast<const float*>(&buffer));
				return r;
			}

			
			/* 
			 * Calculates the length of the vector.
			 */
			static float Length(Vector2 v) { return SIMDVec2Length(v); }
			/*
			 * Calculates the squared length of the vector.
			 */
			static float LengthSquared(Vector2 v) { return SIMDVec2LengthSquared(v); }

			/*
			 * Converts the vector into a unit vector.
			 */
			static Vector2 Normalize(Vector2 vector) { return SIMDVec2Normalize(vector); }

#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/* 
			 * Returns a Vector2 containing the 2D Cartesian coordinates of a point 
			 * specified in Barycentric coordinates relative to a 2D triangle.
			 */
			static Vector2 Barycentric(Vector2 value1, Vector2 value2, 
				Vector2 value3, float amount1, float amount2)
			{
				__m128 t1 = SIMDVecSub(value2,value1);
				__m128 t2 = SIMDVecSub(value3,value1);
				t1 = SIMDVecMul(t1, amount1);
				t2 = SIMDVecMul(t2, amount2);

				__m128 result = SIMDVecAdd(value1, t1);
				result = SIMDVecAdd(result, t2);
				return result;
			}
			/* 
			 * Performs a Catmull-Rom interpolation using the specified positions.
			 */
			static Vector2 CatmullRom(Vector2 value1, Vector2 value2, 
				Vector2 value3, const Vector2& value4, float amount)
			{
				//Vector2 vector;
				float squared = amount * amount;
				float cubed = amount * squared;

				__m128 t2, t3;
				
				__m128 result;
				__m128 t1 = SIMDVecMul(value2, 2);
				__m128 lsum = SIMDVecSub(value3, value1);

				lsum = SIMDVecMul(lsum, amount);

				result = SIMDVecAdd(t1, lsum);

				t1 = SIMDVecMul(value1, 2);
				t2 = SIMDVecMul(value2, -5);
				t3 = SIMDVecMul(value3, 4);
				
				lsum = SIMDVecAdd(t1, t2);
				lsum = SIMDVecAdd(lsum, t3);
				lsum = SIMDVecSub(lsum, value4);
				lsum = SIMDVecMul(t2, squared);

				result = SIMDVecAdd(result, lsum);

				t1 = SIMDVecMul(value2, 3);
				t2 = SIMDVecMul(value3, -3);
							
				lsum = SIMDVecAdd(t1, t2);
				lsum = SIMDVecSub(lsum, value1);
				lsum = SIMDVecAdd(lsum, value4);
				lsum = SIMDVecMul(t2, cubed);

				result = SIMDVecAdd(result, lsum);
				result = SIMDVecMul(result, 0.5f);

				//vector.X = 0.5f * (2.0f * value2.X + (value3.X - value1.X) * amount +
				//	(2.0f * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * squared + 
				//	(3.0f * value2.X - 3.0f * value3.X + value4.X - value1.X) * cubed);

				//vector.Y = 0.5f * ((((2.0f * value2.Y) + ((-value1.Y + value3.Y) * amount)) +
				//	(((((2.0f * value1.Y) - (5.0f * value2.Y)) + (4.0f * value3.Y)) - value4.Y) * squared)) +
				//	((((-value1.Y + (3.0f * value2.Y)) - (3.0f * value3.Y)) + value4.Y) * cubed));

				return result;
			}
			/* 
			 * Performs a Hermite spline interpolation.
			 */
			static Vector2 Hermite(Vector2 value1, Vector2 tangent1,
				Vector2 value2, const Vector2& tangent2, float amount)
			{
				float squared = amount * amount;
				float cubed = amount * squared;
				float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
				float part2 = (-2.0f * cubed) + (3.0f * squared);
				float part3 = (cubed - (2.0f * squared)) + amount;
				float part4 = cubed - squared;


				__m128 t1 = SIMDVecMul(value1, part1);
				__m128 t2 = SIMDVecMul(value2, part2);
				__m128 t3 = SIMDVecMul(tangent1, part3);
				__m128 t4 = SIMDVecMul(tangent2, part4);

				__m128 result = SIMDVecAdd(t1, t2);
				result = SIMDVecAdd(result, t3);
				result = SIMDVecAdd(result, t4);

				return result;
			}
			/*
			 * Performs a linear interpolation between two vectors.
			 */
			static Vector2 Lerp(Vector2 start, Vector2 end, float amount)
			{				
				__m128 t1 = SIMDVecSub(end, start);
				t1 = SIMDVecMul(t1, amount);
				t1 = SIMDVecAdd(start, t1);
				return t1;
			}
			/*
			 * Performs a cubic interpolation between two vectors.
			 */
			static Vector2 SmoothStep(Vector2 start, Vector2 end, float amount)
			{			
				amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
				amount = (amount * amount) * (3.0f - (2.0f * amount));

				__m128 t1 = SIMDVecSub(end, start);
				t1 = SIMDVecMul(t1, amount);
				t1 = SIMDVecAdd(start, t1);

				return t1;
			}
			/* 
			 * Restricts a value to be within a specified range.
			 */
			static Vector2 Clamp(Vector2 value, Vector2 min, Vector2 max)
			{
				__m128 t1 = SIMDVecMax(min, value);
				t1 = SIMDVecMin(max, value);

				return t1;
			}
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/*
			 * Calculates the distance between two vectors.
			 */
			static float Distance(Vector2 value1, Vector2 value2) { return SIMDVec2Distance(value1, value2);	}
			/*
			 * Calculates the dot product of two vectors.
			 */
			static float Dot(Vector2 value1, Vector2 value2) { return SIMDVec2Dot(value1, value2); }
			/*
			 * Calculates the dot product of two vectors.
			 */
			static Vector2 Dot2(Vector2 left, Vector2 right) { return SIMDVec2Dot2(left, right); }
			
			static Vector2 Reflect(Vector2 vector, Vector2 normal) { return SIMDVec2Reflect(vector, normal); }
			/*
			 * Returns a vector containing the smallest components of the specified vectors.
			 */
			static Vector2 Minimize(Vector2 left, Vector2 right) { return SIMDVecMin(left, right); }
			/*
			 * Returns a vector containing the largest components of the specified vectors.
			 */
			static Vector2 Maximize(Vector2 left, Vector2 right) { return SIMDVecMax(left, right); }
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif

		};
		class APAPI Vector3Utils
		{
		public:	

#if APOC3D_MATH_IMPL == APOC3D_SSE
			static const float* GetElementAddress(const Vector3& v) { return reinterpret_cast<const float*>(&v); }
			static float& GetElement(Vector3& v, int i) { return *(reinterpret_cast<float*>(&v)+i); }
			static float& GetX(Vector3& v) { return *(reinterpret_cast<float*>(&v)+0); }
			static float& GetY(Vector3& v) { return *(reinterpret_cast<float*>(&v)+1); }
			static float& GetZ(Vector3& v) { return *(reinterpret_cast<float*>(&v)+2); }
			static float GetX(const Vector3& v) { return *(reinterpret_cast<const float*>(&v)+0); }
			static float GetY(const Vector3&  v) { return *(reinterpret_cast<const float*>(&v) + 1); }
			static float GetZ(const Vector3&  v) { return *(reinterpret_cast<const float*>(&v)+2); }
			static void Store(Vector3 v, float* dest)
			{
				SSEVecLoader buffer;
				_mm_store_ps(reinterpret_cast<float*>(&buffer), v);
				dest[0] = buffer.X; dest[1] = buffer.Y; dest[2] = buffer.Z; dest[3] = 0;
			}
			static Vector3 LDVectorPtr(const float* v)
			{
				const SSEVecLoader buffer = { v[0], v[1], v[2], 0 };

				return SIMDVecLoad(buffer);
			}
			static Vector3 LDVector(float v) { return SIMDVecLoad(v); }
			static Vector3 LDVector(float x, float y, float z)
			{
				const SSEVecLoader buffer = { x, y, z, 0 };

				return SIMDVecLoad(buffer);
			}

			/*
			 * Calculates the length of the vector.
			 */
			static float Length(Vector3 v) { return SIMDVec3Length(v); }
			/*
			 * Calculates the squared length of the vector.
			 */
			static float LengthSquared(Vector3 v) { return SIMDVec3LengthSquared(v); }

			/*
			 * Converts the vector into a unit vector.
			 */
			static Vector3 Normalize(Vector3 vector) { return SIMDVec3Normalize(vector); }
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif
			
#if APOC3D_MATH_IMPL == APOC3D_SSE
			/* 
			 * Adds two vectors.
			 */
			static Vector3 Add(Vector3 left, Vector3 right) { return SIMDVecAdd(left, right); }
			/*
			 * Subtracts two vectors.
			 */
			static Vector3 Subtract(Vector3 left, Vector3 right) { return SIMDVecSub(left, right); }
			/*
			 * Scales a vector by the given value.
			 */
			static Vector3 Multiply(Vector3 value, float scale) { return SIMDVecMul(value, scale); }
			/*
			 * Modulates a vector by another.
			 */
			static Vector3 Modulate(Vector3 left, Vector3 right) { return SIMDVecMul(left, right); }
			/*
			 * Scales a vector by the given value.
			 */
			static Vector3 Divide(Vector3 value, float scale) { return SIMDVecDiv(value, scale); }
			/*
			 * Reverses the direction of a given vector.
			 */
			static Vector3 Negate(Vector3 value) { return SIMDVecNegate(value); }
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT

#endif
			
#if APOC3D_MATH_IMPL == APOC3D_SSE
			/*
			 * Returns a Vector3 containing the 3D Cartesian coordinates of 
			 * a point specified in Barycentric coordinates relative to a 3D triangle.
			 */
			static Vector3 Barycentric(Vector3 value1, Vector3 value2, 
				Vector3 value3, float amount1, float amount2)
			{
				__m128 t1 = SIMDVecSub(value2,value1);
				__m128 t2 = SIMDVecSub(value3,value1);
				t1 = SIMDVecMul(t1, amount1);
				t2 = SIMDVecMul(t2, amount2);

				__m128 result = SIMDVecAdd(value1, t1);
				result = SIMDVecAdd(result, t2);
				return result;
			}

			/* 
			 * Performs a Catmull-Rom interpolation using the specified positions.
			 */
			static Vector3 CatmullRom(Vector3 value1, Vector3 value2, 
				Vector3 value3, const Vector3& value4, float amount)
			{
				float squared = amount * amount;
				float cubed = amount * squared;

				__m128 t2, t3;
				
				__m128 result;
				__m128 t1 = SIMDVecMul(value2, 2);
				__m128 lsum = SIMDVecSub(value3, value1);

				lsum = SIMDVecMul(lsum, amount);

				result = SIMDVecAdd(t1, lsum);

				t1 = SIMDVecMul(value1, 2);
				t2 = SIMDVecMul(value2, -5);
				t3 = SIMDVecMul(value3, 4);
				
				lsum = SIMDVecAdd(t1, t2);
				lsum = SIMDVecAdd(lsum, t3);
				lsum = SIMDVecSub(lsum, value4);
				lsum = SIMDVecMul(t2, squared);

				result = SIMDVecAdd(result, lsum);

				t1 = SIMDVecMul(value2, 3);
				t2 = SIMDVecMul(value3, -3);
							
				lsum = SIMDVecAdd(t1, t2);
				lsum = SIMDVecSub(lsum, value1);
				lsum = SIMDVecAdd(lsum, value4);
				lsum = SIMDVecMul(t2, cubed);

				result = SIMDVecAdd(result, lsum);
				result = SIMDVecMul(result, 0.5f);
				return result;
			}
			/* 
			 * Performs a Hermite spline interpolation.
			 */
			static Vector3 Hermite(Vector3 value1, Vector3 tangent1, 
				Vector3 value2, const Vector3& tangent2, float amount)
			{
				float squared = amount * amount;
				float cubed = amount * squared;
				float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
				float part2 = (-2.0f * cubed) + (3.0f * squared);
				float part3 = (cubed - (2.0f * squared)) + amount;
				float part4 = cubed - squared;


				__m128 t1 = SIMDVecMul(value1, part1);
				__m128 t2 = SIMDVecMul(value2, part2);
				__m128 t3 = SIMDVecMul(tangent1, part3);
				__m128 t4 = SIMDVecMul(tangent2, part4);

				__m128 result = SIMDVecAdd(t1, t2);
				result = SIMDVecAdd(result, t3);
				result = SIMDVecAdd(result, t4);

				return result;
			}
			/* 
			 * Performs a linear interpolation between two vectors.
			 */
			static Vector3 Lerp(Vector3 start, Vector3 end, float amount)
			{
				__m128 t1 = SIMDVecSub(end, start);
				t1 = SIMDVecMul(t1, amount);
				t1 = SIMDVecAdd(start, t1);
				return t1;
			}
			/*
			 * Performs a cubic interpolation between two vectors.
			 */
			static Vector3 SmoothStep(Vector3 start, Vector3 end, float amount)
			{
				amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
				amount = (amount * amount) * (3.0f - (2.0f * amount));

				__m128 t1 = SIMDVecSub(end, start);
				t1 = SIMDVecMul(t1, amount);
				t1 = SIMDVecAdd(start, t1);

				return t1;
			}
			/* 
			 * Restricts a value to be within a specified range.
			 */
			static Vector3 Clamp(Vector3 value, Vector3 min, Vector3 max)
			{
				__m128 t1 = SIMDVecMax(min, value);
				t1 = SIMDVecMin(max, value);

				return t1;
			}
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
		
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/*
			 * Calculates the distance between two vectors.
			 */
			static float Distance(Vector3 value1, Vector3 value2) { return SIMDVec3Distance(value1, value2); }
			/*
			 * Calculates the squared distance between two vectors.
			 */
			static float DistanceSquared(Vector3 value1, Vector3 value2) { return SIMDVec3DistanceSquared(value1, value2); }

			/*
			 * Calculates the dot product of two vectors.
			 */
			static float Dot(Vector3 left, Vector3 right) { return SIMDVec3Dot(left, right); }

			/*
			 * Calculates the cross product of two vectors.
			 */
			static Vector3 Cross(Vector3 left, Vector3 right) { return SIMDVec3Cross(left, right); }

			/*
			 * Returns the reflection of a vector off a surface that has the specified normal. 
			 */
			static Vector3 Reflect(Vector3 vector, Vector3 normal) { return SIMDVec3Reflect(vector, normal); }


			/*
			 * Returns a vector containing the smallest components of the specified vectors.
			 */
			static Vector3 Minimize(Vector3 left, Vector3 right) { return SIMDVecMin(left, right); }
			/*
			 * Returns a vector containing the largest components of the specified vectors.
			 */
			static Vector3 Maximize(Vector3 left, Vector3 right) { return SIMDVecMax(left, right); }
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/* 
			 * Transforms a 3D vector by the given Matrix
			 */
			static Vector4 Transform(Vector3 vector, const Matrix& transform);
			/*
			 * Performs a coordinate transformation using the given Matrix. can not project
			 */
			static Vector3 TransformSimple(Vector3 vector, const Matrix& transform);
			/*
			 * Performs a coordinate transformation using the given Matrix
			 */
			static Vector3 TransformCoordinate(Vector3 vector, const Matrix& transform);
			/*
			 * Performs a normal transformation using the given Matrix
			 */
			static Vector3 TransformNormal(Vector3 vector, const Matrix& transform);
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			static bool IsLess(Vector3 left, Vector3 right) { return SIMDVec3Less(left, right); }
			static bool IsLessEqual(Vector3 left, Vector3 right) { return SIMDVec3LessEqual(left, right); }
			static bool IsGreater(Vector3 left, Vector3 right) { return SIMDVec3Greater(left, right); }
			static bool IsGreaterEqual(Vector3 left, Vector3 right) { return SIMDVec3GreaterEqual(left, right); }

#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			

#endif

		};
		class APAPI Vector4Utils
		{
		public:
#if APOC3D_MATH_IMPL == APOC3D_SSE
			static const float* GetElementAddress(const Vector4& v) { return reinterpret_cast<const float*>(&v); }
			static float& GetElement(Vector4& v, int i) { return *(reinterpret_cast<float*>(&v)+i); }
			static float& GetX(Vector4& v) { return *(reinterpret_cast<float*>(&v)+0); }
			static float& GetY(Vector4& v) { return *(reinterpret_cast<float*>(&v)+1); }
			static float& GetZ(Vector4& v) { return *(reinterpret_cast<float*>(&v)+2); }
			static float& GetW(Vector4& v) { return *(reinterpret_cast<float*>(&v)+3); }
			static float GetX(const Vector4& v) { return *(reinterpret_cast<const float*>(&v) + 0); }
			static float GetY(const Vector4& v) { return *(reinterpret_cast<const float*>(&v) + 1); }
			static float GetZ(const Vector4& v) { return *(reinterpret_cast<const float*>(&v) + 2); }
			static float GetW(const Vector4& v) { return *(reinterpret_cast<const float*>(&v) + 3); }
			static void Store(Vector4 v, float* dest)
			{
				SSEVecLoader buffer;
				_mm_store_ps(reinterpret_cast<float*>(&buffer), v);
				dest[0] = buffer.X; dest[1] = buffer.Y; dest[2] = buffer.Z; dest[3] = buffer.W;
			}
			static Vector4 LDVectorPtr(const float* v)
			{
				const SSEVecLoader buffer = {v[0],v[1],v[2],v[3]};
				return SIMDVecLoad(buffer);
			}
			static Vector4 LDVector(float v) { return SIMDVecLoad(v); }
			static Vector4 LDVector(float x, float y, float z, float w)
			{
				const SSEVecLoader buffer = {x,y,z,w};
				return SIMDVecLoad(buffer);
			}
			/*
			 * Calculates the length of the vector.
			 */
			static float Length(Vector4 v) { return SIMDVec4Length(v); }
			/*
			 * Calculates the squared length of the vector.
			 */
			static float LengthSquared(Vector4 v) { return SIMDVec4LengthSquared(v); }

			/*
			 * Converts the vector into a unit vector.
			 */
			static Vector4 Normalize(Vector4 vector) { return SIMDVec3Normalize(vector); }

#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/*
			 * Adds two vectors.
			 */
			static Vector4 Add(Vector4 left, Vector4 right) { return SIMDVecAdd(left, right); }
			/* 
			 * Subtracts two vectors.
			 */
			static Vector4 Subtract(Vector4 left, Vector4 right) { return SIMDVecSub(left, right); }
			/*
			 * Scales a vector by the given value.
			 */
			static Vector4 Multiply(Vector4 value, float scale) { return SIMDVecMul(value, scale); }
			/*
			 * Modulates a vector by another.
			 */
			static Vector4 Modulate(Vector4 left, Vector4 right) { return SIMDVecMul(left, right); }
			/*
			 * Scales a vector by the given value.
			 */
			static Vector4 Divide(Vector4 value, float scale) { return SIMDVecDiv(value, scale); }
			/*
			 * Reverses the direction of a given vector.
			 */
			static Vector4 Negate(Vector4 value) { return SIMDVecNegate(value); }
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif
			
#if APOC3D_MATH_IMPL == APOC3D_SSE
			/*
			 * Returns a Vector4 containing the 4D Cartesian coordinates of 
			 * a point specified in Barycentric coordinates relative to a 4D triangle.
			 */
			static Vector4 Barycentric(Vector4 value1, Vector4 value2, 
				Vector4 value3, float amount1, float amount2)
			{
				__m128 t1 = SIMDVecSub(value2,value1);
				__m128 t2 = SIMDVecSub(value3,value1);
				t1 = SIMDVecMul(t1, amount1);
				t2 = SIMDVecMul(t2, amount2);

				__m128 result = SIMDVecAdd(value1, t1);
				result = SIMDVecAdd(result, t2);
				return result;
			}
			/* 
			 * Performs a Catmull-Rom interpolation using the specified positions.
			 */
			static Vector4 CatmullRom(Vector4 value1, Vector4 value2, 
				Vector4 value3, const Vector4& value4, float amount)
			{
				float squared = amount * amount;
				float cubed = amount * squared;

				__m128 t2, t3;
				
				__m128 result;
				__m128 t1 = SIMDVecMul(value2, 2);
				__m128 lsum = SIMDVecSub(value3, value1);

				lsum = SIMDVecMul(lsum, amount);

				result = SIMDVecAdd(t1, lsum);

				t1 = SIMDVecMul(value1, 2);
				t2 = SIMDVecMul(value2, -5);
				t3 = SIMDVecMul(value3, 4);
				
				lsum = SIMDVecAdd(t1, t2);
				lsum = SIMDVecAdd(lsum, t3);
				lsum = SIMDVecSub(lsum, value4);
				lsum = SIMDVecMul(t2, squared);

				result = SIMDVecAdd(result, lsum);

				t1 = SIMDVecMul(value2, 3);
				t2 = SIMDVecMul(value3, -3);
							
				lsum = SIMDVecAdd(t1, t2);
				lsum = SIMDVecSub(lsum, value1);
				lsum = SIMDVecAdd(lsum, value4);
				lsum = SIMDVecMul(t2, cubed);

				result = SIMDVecAdd(result, lsum);
				result = SIMDVecMul(result, 0.5f);
				return result;
			}

			/* 
			 * Performs a Hermite spline interpolation.
			 */
			static Vector4 Hermite(Vector4 value1, Vector4 tangent1, 
				Vector4 value2, const Vector4& tangent2, float amount)
			{
				float squared = amount * amount;
				float cubed = amount * squared;
				float part1 = ((2.0f * cubed) - (3.0f * squared)) + 1.0f;
				float part2 = (-2.0f * cubed) + (3.0f * squared);
				float part3 = (cubed - (2.0f * squared)) + amount;
				float part4 = cubed - squared;


				__m128 t1 = SIMDVecMul(value1, part1);
				__m128 t2 = SIMDVecMul(value2, part2);
				__m128 t3 = SIMDVecMul(tangent1, part3);
				__m128 t4 = SIMDVecMul(tangent2, part4);

				__m128 result = SIMDVecAdd(t1, t2);
				result = SIMDVecAdd(result, t3);
				result = SIMDVecAdd(result, t4);

				return result;
			}

			/* 
			 * Restricts a value to be within a specified range.
			 */
			static Vector4 Clamp(Vector4 value, Vector4 min, Vector4 max)
			{
				__m128 t1 = SIMDVecMax(min, value);
				t1 = SIMDVecMin(max, value);

				return t1;
			}
			
			/*
			 * Performs a linear interpolation between two vectors.
			 */
			static Vector4 Lerp(Vector4 start, Vector4 end, float amount)
			{
				__m128 t1 = SIMDVecSub(end, start);
				t1 = SIMDVecMul(t1, amount);
				t1 = SIMDVecAdd(start, t1);
				return t1;
			}
			/*
			 * Performs a cubic interpolation between two vectors.
			 */
			static Vector4 SmoothStep(Vector4 start, Vector4 end, float amount)
			{
				amount = (amount > 1.0f) ? 1.0f : ((amount < 0.0f) ? 0.0f : amount);
				amount = (amount * amount) * (3.0f - (2.0f * amount));

				__m128 t1 = SIMDVecSub(end, start);
				t1 = SIMDVecMul(t1, amount);
				t1 = SIMDVecAdd(start, t1);

				return t1;
			}
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
			
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/*
			 * Calculates the distance between two vectors.
			 */
			static float Distance(Vector4 value1, Vector4 value2) { return SIMDVec4Distance(value1, value2); }
			/*
			 * Calculates the squared distance between two vectors.
			 */
			static float DistanceSquared(Vector4 value1, Vector4 value2) { return SIMDVec4DistanceSquared(value1, value2); }
			/* 
			 * Calculates the dot product of two vectors.
			 */
			static float Dot(Vector4 left, Vector4 right) { return SIMDVec4Dot(left, right); }

			/*
			 * Returns a vector containing the smallest components of the specified vectors.
			 */
			static Vector4 Minimize(Vector4 left, Vector4 right) { return SIMDVecMin(left, right); }

			/*
			 * Returns a vector containing the largest components of the specified vectors.
			 */
			static Vector4 Maximize(Vector4 left, Vector4 right) { return SIMDVecMax(left, right); }
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
			
#endif

#if APOC3D_MATH_IMPL == APOC3D_SSE
			/*
			 * Transforms a 3D vector by the given Matrix
			 */
			static Vector4 Transform(Vector4 vector, const Matrix& transform);
			/*
			 * Performs a coordinate transformation using the given Matrix. can not project
			 */
			static Vector4 TransformSimple(Vector4 vector, const Matrix& transform);
			/*
			 * Performs a coordinate transformation using the given Matrix
			 */
			static Vector4 TransformCoordinate(Vector4 vector, const Matrix& transform);
			/*
			 * Performs a normal transformation using the given Matrix
			 */
			static Vector4 TransformNormal(Vector4 vector, const Matrix& transform);
#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
#endif

		};

	}
}

#endif