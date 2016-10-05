#pragma once
#ifndef APOC3D_BINARYWRITER_H
#define APOC3D_BINARYWRITER_H

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
#include "apoc3d/Math/Vector.h"

using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace IO
	{
		class APAPI BinaryWriter
		{
		public:
			BinaryWriter(Stream* baseStream, bool releaseStream);
			~BinaryWriter();

			void Write(const char* bytes, int64 count) const;

			void WriteByte(char byte) const;
			void WriteBoolean(bool value) const;

			void WriteDouble(double value) const;
			void WriteSingle(float value) const;
			void WriteString(const String& value) const;
			void WriteMBString(const std::string& value) const;

			void WriteInt16(int16 value) const;
			void WriteInt32(int32 value) const;
			void WriteInt64(int64 value) const;
			void WriteUInt16(uint16 value) const;
			void WriteUInt32(uint32 value) const;
			void WriteUInt64(uint64 value) const;

			void WriteColor4(const Color4& value) const;
			void WritePlane(const Plane& plane) const;
			void WriteMatrix(const Matrix& matrix) const;
			void WriteQuaternion(const Quaternion& quat) const;
			void WritePoint(const Point& point) const;
			void WriteSize(const Size& size) const;
			void WriteRectangle(const Apoc3D::Math::Rectangle& rect) const;
			void WriteRectangleF(const Apoc3D::Math::RectangleF& rect) const;

			void WriteRay(const Ray& ray) const;
			void WriteBoundingBox(const BoundingBox& box) const;
			void WriteBoundingSphere(const BoundingSphere& sphere) const;

			void WriteTaggedDataBlock(const TaggedDataWriter* data) const;
			void WriteTaggedDataBlock(FunctorReference<void(TaggedDataWriter*)> f) const;

			void WriteVector2(const Vector2& vec) const;
			void WriteVector3(const Vector3& vec) const;
			void WriteVector4(const Vector4& vec) const;

			void WriteSingle(const float* data, int32 count) const;
			void WriteDouble(const double* data, int32 count) const;
			void WriteInt32(const int32* data, int32 count) const;
			void WriteUInt32(const uint32* data, int32 count) const;
			void WriteBoolean(const bool* data, int32 count) const;

			void WriteBooleanBits(const bool* arr, int32 count) const;


			template <int32 N> void WriteSingle(const float(&data)[N]) { return WriteSingle(data, N); }
			template <int32 N> void WriteDouble(const double(&data)[N]) { return WriteDouble(data, N); }
			template <int32 N> void WriteInt32(const int32(&data)[N]) { return WriteInt32(data, N); }
			template <int32 N> void WriteUInt32(const uint32(&data)[N]) { return WriteUInt32(data, N); }
			template <int32 N> void WriteBoolean(const bool(&data)[N]) { return WriteBoolean(data, N); }
			template <int32 N> void WriteBooleanBits(const bool(&data)[N]) { return WriteBooleanBits(data, N); }


			/** Ask the BinaryWriter not to delete the base stream upon release */
			void SuspendStreamRelease() { m_shouldDeleteStream = false; }

			Stream* getBaseStream() const { return m_baseStream; }

		private:
			
			bool m_endianIndependent;
			bool m_shouldDeleteStream;

			Stream* m_baseStream;
		};
	}
}
#endif