#pragma once
#ifndef APOC3D_POINT_H
#define APOC3D_POINT_H

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

namespace Apoc3D
{
	namespace Math
	{
		class APAPI Size
		{
		public:
			int Width;
			int Height;

			Size() : Width(0), Height(0) { }
			Size(int w, int h) : Width(w), Height(h) { }

			bool operator==(const Size &other) const { return (Width == other.Width) && (Height == other.Height); }
			bool operator!=(const Size &other) const { return !(*this == other); }

			static const Size Zero;
		};

		class APAPI Point
		{
		public:
			int32 X;
			int32 Y;

			Point() : X(0), Y(0) { }
			Point(int32 x, int32 y) : X(x), Y(y) { }

			bool operator==(const Point& other) const { return (X == other.X) && (Y == other.Y); }
			bool operator!=(const Point& other) const { return !(*this == other); }

			Point operator+(const Point& other) const { return Point(X + other.X, Y + other.Y); }
			Point operator-(const Point& other) const { return Point(X - other.X, Y - other.Y); }
			Point operator-() const { return Point(-X, -Y); }

			Point operator/(int32 v) const { return Point(X / v, Y / v); }
			Point operator*(int32 v) const { return Point(X * v, Y * v); }
			Point operator*(const Point& other) const { return Point(X * other.X, Y * other.Y); }

			Point& operator +=(const Point& rhs)
			{
				X += rhs.X;
				Y += rhs.Y;
				return *this;
			}
			Point& operator -=(const Point& rhs)
			{
				X -= rhs.X;
				Y -= rhs.Y;
				return *this;
			}

			Point& operator *=(int32 v)
			{
				X *= v; Y *= v;
				return *this;
			}
			Point& operator /=(int32 v)
			{
				X /= v; Y /= v;
				return *this;
			}

			static float Distance(const Point& a, const Point& b)
			{
				int32 dx = b.X - a.X;
				int32 dy = b.Y - a.Y;
				return sqrtf(static_cast<float>(dx*dx + dy*dy));
			}

			static const Point Zero;
		};

		class APAPI PointF
		{
		public:
			float X;
			float Y;

			PointF() : X(0), Y(0) { }
			PointF(float x, float y) : X(x), Y(y) { }
			PointF(const Point& p) : X(static_cast<float>(p.X)), Y(static_cast<float>(p.Y)) { }
			PointF(const Vector2& v);

			bool operator==(const PointF &other) const { return (X == other.X) && (Y == other.Y); }
			bool operator!=(const PointF &other) const { return !(*this == other); }

			PointF operator+(const PointF &other) const { return PointF(X + other.X, Y + other.Y); }
			PointF operator-(const PointF &other) const { return PointF(X - other.X, Y - other.Y); }
			PointF operator-() const { return PointF(-X, -Y); }

			PointF operator/(float v) const { return PointF(X / v, Y / v); }
			PointF operator*(float v) const { return PointF(X * v, Y * v); }
			PointF operator*(const PointF& other) const { return PointF(X * other.X, Y * other.Y); }

			PointF& operator +=(const PointF& rhs)
			{
				X += rhs.X;
				Y += rhs.Y;
				return *this;
			}
			PointF& operator -=(const PointF& rhs)
			{
				X -= rhs.X;
				Y -= rhs.Y;
				return *this;
			}

			PointF& operator *=(float v)
			{
				X *= v; Y *= v;
				return *this;
			}
			PointF& operator /=(float v)
			{
				X /= v; Y /= v;
				return *this;
			}

			static float Distance(const PointF& a, const PointF& b)
			{
				float dx = b.X - a.X;
				float dy = b.Y - a.Y;
				return sqrtf(dx*dx + dy*dy);
			}

			static const PointF Zero;
		};

	}
}
#endif