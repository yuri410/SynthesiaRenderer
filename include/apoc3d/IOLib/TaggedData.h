#pragma once
#ifndef APOC3D_BINARYDATA_H
#define APOC3D_BINARYDATA_H

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

#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Math/Vector.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace IO
	{
		/**
		 *  Tagged data is a collection of key and values. 
		 *  The keys are name of the corresponding value in string form. 
		 *  The value is a series of binary bytes which can represent anything.
		 *  
		 *  As TaggedData is in a key-value form. the value can be a sub-TaggedData. And the value of sub-TaggedData
		 *  can have sub-sub-TaggedData.
		 *  In this way, respectively a tree of data can be stored with this class easily.
		 *  
		 *  This class implements a reader for Tagged Data.
		 */
		class APAPI TaggedDataReader
		{
		public:
			typedef std::string KeyType;

			TaggedDataReader(Stream* strm);
			~TaggedDataReader();

			void SuspendStreamRelease() { m_suspendStreamRelease = true; }

			/**  Check if the data chunk has a key name of the specified */
			bool Contains(const KeyType& name) const { return m_positions.Contains(name); }

			BinaryReader* TryGetData(const KeyType& name) const;
			BinaryReader* GetData(const KeyType& name) const;

			bool TryProcessData(const KeyType& name, FunctorReference<void(BinaryReader*)> f) const;
			void ProcessData(const KeyType& name, FunctorReference<void(BinaryReader*)> f) const;

			bool TryProcessDataSection(const KeyType& name, FunctorReference<void(TaggedDataReader*)> f) const;
			void ProcessDataSection(const KeyType& name, FunctorReference<void(TaggedDataReader*)> f) const;

			Stream* GetDataStream(const KeyType& name) const;

			void GetAuto(const KeyType& name, int64& value);
			void GetAuto(const KeyType& name, int32& value);
			void GetAuto(const KeyType& name, int16& value);
			void GetAuto(const KeyType& name, uint64& value);
			void GetAuto(const KeyType& name, uint32& value);
			void GetAuto(const KeyType& name, uint16& value);
			void GetAuto(const KeyType& name, float& value);
			void GetAuto(const KeyType& name, double& value);
			void GetAuto(const KeyType& name, bool& value);
			void GetAuto(const KeyType& name, Vector2& vec);
			void GetAuto(const KeyType& name, Vector3& vec);
			void GetAuto(const KeyType& name, Vector4& vec);
			void GetAuto(const KeyType& name, Matrix& mat);
			void GetAuto(const KeyType& name, Quaternion& quad);
			void GetAuto(const KeyType& name, Color4& clr);
			void GetAuto(const KeyType& name, Plane& plane);
			void GetAuto(const KeyType& name, Apoc3D::Math::Rectangle& rect);
			void GetAuto(const KeyType& name, Apoc3D::Math::RectangleF& rect);
			void GetAuto(const KeyType& name, Point& pt);
			void GetAuto(const KeyType& name, PointF& pt);
			void GetAuto(const KeyType& name, Apoc3D::Math::Size& sz);
			void GetAuto(const KeyType& name, BoundingBox& bb);
			void GetAuto(const KeyType& name, BoundingSphere& bs);
			void GetAuto(const KeyType& name, Ray& r);
			void GetAuto(const KeyType& name, Viewport& vp);
			void GetAuto(const KeyType& name, String& str);

			void GetAuto(const KeyType& name, int64* value, int32 count);
			void GetAuto(const KeyType& name, int32* value, int32 count);
			void GetAuto(const KeyType& name, int16* value, int32 count);
			void GetAuto(const KeyType& name, uint64* value, int32 count);
			void GetAuto(const KeyType& name, uint32* value, int32 count);
			void GetAuto(const KeyType& name, uint16* value, int32 count);
			void GetAuto(const KeyType& name, float* value, int32 count);
			void GetAuto(const KeyType& name, double* value, int32 count);
			void GetAuto(const KeyType& name, bool* value, int32 count);
			void GetAuto(const KeyType& name, Vector2* vec, int32 count);
			void GetAuto(const KeyType& name, Vector3* vec, int32 count);
			void GetAuto(const KeyType& name, Vector4* vec, int32 count);
			void GetAuto(const KeyType& name, Matrix* mat, int32 count);
			void GetAuto(const KeyType& name, Quaternion* quad, int32 count);
			void GetAuto(const KeyType& name, Color4* clr, int32 count);
			void GetAuto(const KeyType& name, Plane* plane, int32 count);
			void GetAuto(const KeyType& name, Apoc3D::Math::Rectangle* rect, int32 count);
			void GetAuto(const KeyType& name, RectangleF* rect, int32 count);
			void GetAuto(const KeyType& name, Point* pt, int32 count);
			void GetAuto(const KeyType& name, PointF* pt, int32 count);
			void GetAuto(const KeyType& name, Apoc3D::Math::Size* sz, int32 count);
			void GetAuto(const KeyType& name, BoundingBox* bb, int32 count);
			void GetAuto(const KeyType& name, BoundingSphere* bs, int32 count);
			void GetAuto(const KeyType& name, Ray* r, int32 count);
			void GetAuto(const KeyType& name, Viewport* vp, int32 count);
			void GetAuto(const KeyType& name, String* str, int32 count);

			bool TryGetAuto(const KeyType& name, int64& value);
			bool TryGetAuto(const KeyType& name, int32& value);
			bool TryGetAuto(const KeyType& name, int16& value);
			bool TryGetAuto(const KeyType& name, uint64& value);
			bool TryGetAuto(const KeyType& name, uint32& value);
			bool TryGetAuto(const KeyType& name, uint16& value);
			bool TryGetAuto(const KeyType& name, float& value);
			bool TryGetAuto(const KeyType& name, double& value);
			bool TryGetAuto(const KeyType& name, bool& value);
			bool TryGetAuto(const KeyType& name, Vector2& vec);
			bool TryGetAuto(const KeyType& name, Vector3& vec);
			bool TryGetAuto(const KeyType& name, Vector4& vec);
			bool TryGetAuto(const KeyType& name, Matrix& mat);
			bool TryGetAuto(const KeyType& name, Quaternion& quad);
			bool TryGetAuto(const KeyType& name, Color4& clr);
			bool TryGetAuto(const KeyType& name, Plane& plane);
			bool TryGetAuto(const KeyType& name, Apoc3D::Math::Rectangle& rect);
			bool TryGetAuto(const KeyType& name, RectangleF& rect);
			bool TryGetAuto(const KeyType& name, Point& pt);
			bool TryGetAuto(const KeyType& name, PointF& pt);
			bool TryGetAuto(const KeyType& name, Apoc3D::Math::Size& sz);
			bool TryGetAuto(const KeyType& name, BoundingBox& bb);
			bool TryGetAuto(const KeyType& name, BoundingSphere& bs);
			bool TryGetAuto(const KeyType& name, Ray& r);
			bool TryGetAuto(const KeyType& name, Viewport& vp);
			bool TryGetAuto(const KeyType& name, String& str);

			bool TryGetAuto(const KeyType& name, int64* value, int32 count);
			bool TryGetAuto(const KeyType& name, int32* value, int32 count);
			bool TryGetAuto(const KeyType& name, int16* value, int32 count);
			bool TryGetAuto(const KeyType& name, uint64* value, int32 count);
			bool TryGetAuto(const KeyType& name, uint32* value, int32 count);
			bool TryGetAuto(const KeyType& name, uint16* value, int32 count);
			bool TryGetAuto(const KeyType& name, float* value, int32 count);
			bool TryGetAuto(const KeyType& name, double* value, int32 count);
			bool TryGetAuto(const KeyType& name, bool* value, int32 count);
			bool TryGetAuto(const KeyType& name, Vector2* vec, int32 count);
			bool TryGetAuto(const KeyType& name, Vector3* vec, int32 count);
			bool TryGetAuto(const KeyType& name, Vector4* vec, int32 count);
			bool TryGetAuto(const KeyType& name, Matrix* mat, int32 count);
			bool TryGetAuto(const KeyType& name, Quaternion* quad, int32 count);
			bool TryGetAuto(const KeyType& name, Color4* clr, int32 count);
			bool TryGetAuto(const KeyType& name, Plane* plane, int32 count);
			bool TryGetAuto(const KeyType& name, Apoc3D::Math::Rectangle* rect, int32 count);
			bool TryGetAuto(const KeyType& name, RectangleF* rect, int32 count);
			bool TryGetAuto(const KeyType& name, Point* pt, int32 count);
			bool TryGetAuto(const KeyType& name, PointF* pt, int32 count);
			bool TryGetAuto(const KeyType& name, Apoc3D::Math::Size* sz, int32 count);
			bool TryGetAuto(const KeyType& name, BoundingBox* bb, int32 count);
			bool TryGetAuto(const KeyType& name, BoundingSphere* bs, int32 count);
			bool TryGetAuto(const KeyType& name, Ray* r, int32 count);
			bool TryGetAuto(const KeyType& name, Viewport* vp, int32 count);
			bool TryGetAuto(const KeyType& name, String* str, int32 count);


			int64 GetInt64(const KeyType& name);
			int32 GetInt32(const KeyType& name);
			int16 GetInt16(const KeyType& name);
			uint64 GetUInt64(const KeyType& name);
			uint32 GetUInt32(const KeyType& name);
			uint16 GetUInt16(const KeyType& name);
			bool GetBool(const KeyType& name);
			float GetSingle(const KeyType& name);
			double GetDouble(const KeyType& name);

			void GetInt64(const KeyType& name, int64* value, int32 count);
			void GetInt32(const KeyType& name, int32* value, int32 count);
			void GetInt16(const KeyType& name, int16* value, int32 count);
			void GetUInt64(const KeyType& name, uint64* value, int32 count);
			void GetUInt32(const KeyType& name, uint32* value, int32 count);
			void GetUInt16(const KeyType& name, uint16* value, int32 count);
			void GetBool(const KeyType& name, bool* value, int32 count);
			void GetSingle(const KeyType& name, float* value, int32 count);
			void GetDouble(const KeyType& name, double* value, int32 count);

			template <int32 N> void GetInt64(const KeyType& name, int64(&value)[N]) { GetInt64(name, value, N); }
			template <int32 N> void GetInt32(const KeyType& name, int32(&value)[N]) { GetInt32(name, value, N); }
			template <int32 N> void GetInt16(const KeyType& name, int16(&value)[N]) { GetInt16(name, value, N); }
			template <int32 N> void GetUInt64(const KeyType& name, uint64(&value)[N]) { GetUInt64(name, value, N); }
			template <int32 N> void GetUInt32(const KeyType& name, uint32(&value)[N]) { GetUInt32(name, value, N); }
			template <int32 N> void GetUInt16(const KeyType& name, uint16(&value)[N]) { GetUInt16(name, value, N); }
			template <int32 N> void GetBool(const KeyType& name, bool(&value)[N]) { GetBool(name, value, N); }
			template <int32 N> void GetSingle(const KeyType& name, float(&value)[N]) { GetSingle(name, value, N); }
			template <int32 N> void GetDouble(const KeyType& name, double(&value)[N]) { GetDouble(name, value, N); }


			void GetVector2(const KeyType& name, Vector2& vec);
			void GetVector3(const KeyType& name, Vector3& vec);
			void GetVector4(const KeyType& name, Vector4& vec);
			void GetMatrix(const KeyType& name, Matrix& mat);
			void GetQuaternion(const KeyType& name, Quaternion& quat);
			void GetColor4(const KeyType& name, Color4& clr);
			void GetPlane(const KeyType& name, Plane& plane);
			void GetString(const KeyType& name, String& str);
			void GetRectangle(const KeyType& name, Apoc3D::Math::Rectangle& rect);
			void GetRectangleF(const KeyType& name, RectangleF& rect);
			void GetPoint(const KeyType& name, Point& pt);
			void GetPointF(const KeyType& name, PointF& pt);
			void GetMathSize(const KeyType& name, Apoc3D::Math::Size& sz);
			void GetBoundingBox(const KeyType& name, BoundingBox& bb);
			void GetBoundingSphere(const KeyType& name, BoundingSphere& bs);
			void GetRay(const KeyType& name, Ray& r);
			void GetViewport(const KeyType& name, Viewport& vp);

			void GetVector2(const KeyType& name, Vector2* vec, int32 count);
			void GetVector3(const KeyType& name, Vector3* vec, int32 count);
			void GetVector4(const KeyType& name, Vector4* vec, int32 count);
			void GetMatrix(const KeyType& name, Matrix* mat, int32 count);
			void GetQuaternion(const KeyType& name, Quaternion* quat, int32 count);
			void GetColor4(const KeyType& name, Color4* clr, int32 count);
			void GetPlane(const KeyType& name, Plane* plane, int32 count);
			void GetString(const KeyType& name, String* str, int32 count);
			void GetRectangle(const KeyType& name, Apoc3D::Math::Rectangle* rect, int32 count);
			void GetRectangleF(const KeyType& name, RectangleF* rect, int32 count);
			void GetPoint(const KeyType& name, Point* pt, int32 count);
			void GetPointF(const KeyType& name, PointF* pt, int32 count);
			void GetMathSize(const KeyType& name, Apoc3D::Math::Size* sz, int32 count);
			void GetBoundingBox(const KeyType& name, BoundingBox* bb, int32 count);
			void GetBoundingSphere(const KeyType& name, BoundingSphere* bs, int32 count);
			void GetRay(const KeyType& name, Ray* r, int32 count);
			void GetViewport(const KeyType& name, Viewport* vp, int32 count);
			


			bool TryGetInt64(const KeyType& name, int64& v);
			bool TryGetInt32(const KeyType& name, int32& v);
			bool TryGetInt16(const KeyType& name, int16& v);
			bool TryGetUInt64(const KeyType& name, uint64& v);
			bool TryGetUInt32(const KeyType& name, uint32& v);
			bool TryGetUInt16(const KeyType& name, uint16& v);
			bool TryGetBool(const KeyType& name, bool& v);
			bool TryGetSingle(const KeyType& name, float& v);
			bool TryGetDouble(const KeyType& name, double& v);

			bool TryGetInt64(const KeyType& name, int64* value, int32 count);
			bool TryGetInt32(const KeyType& name, int32* value, int32 count);
			bool TryGetInt16(const KeyType& name, int16* value, int32 count);
			bool TryGetUInt64(const KeyType& name, uint64* value, int32 count);
			bool TryGetUInt32(const KeyType& name, uint32* value, int32 count);
			bool TryGetUInt16(const KeyType& name, uint16* value, int32 count);
			bool TryGetBool(const KeyType& name, bool* value, int32 count);
			bool TryGetSingle(const KeyType& name, float* value, int32 count);
			bool TryGetDouble(const KeyType& name, double* value, int32 count);

			bool TryGetVector2(const KeyType& name, Vector2& vec);
			bool TryGetVector3(const KeyType& name, Vector3& vec);
			bool TryGetVector4(const KeyType& name, Vector4& vec);
			bool TryGetMatrix(const KeyType& name, Matrix& mat);
			bool TryGetQuaternion(const KeyType& name, Quaternion& quat);
			bool TryGetColor4(const KeyType& name, Color4& clr);
			bool TryGetPlane(const KeyType& name, Plane& plane);
			bool TryGetRectangle(const KeyType& name, Apoc3D::Math::Rectangle& rect);
			bool TryGetRectangleF(const KeyType& name, RectangleF& rect);
			bool TryGetPoint(const KeyType& name, Point& pt);
			bool TryGetPointF(const KeyType& name, PointF& pt);
			bool TryGetSize(const KeyType& name, Apoc3D::Math::Size& sz);
			bool TryGetBoundingBox(const KeyType& name, BoundingBox& bb);
			bool TryGetBoundingSphere(const KeyType& name, BoundingSphere& bs);
			bool TryGetRay(const KeyType& name, Ray& r);
			bool TryGetViewport(const KeyType& name, Viewport& vp);
			bool TryGetString(const KeyType& name, String& str);

			bool TryGetVector2(const KeyType& name, Vector2* vec, int32 count);
			bool TryGetVector3(const KeyType& name, Vector3* vec, int32 count);
			bool TryGetVector4(const KeyType& name, Vector4* vec, int32 count);
			bool TryGetMatrix(const KeyType& name, Matrix* mat, int32 count);
			bool TryGetQuaternion(const KeyType& name, Quaternion* quat, int32 count);
			bool TryGetColor4(const KeyType& name, Color4* clr, int32 count);
			bool TryGetPlane(const KeyType& name, Plane* plane, int32 count);
			bool TryGetRectangle(const KeyType& name, Apoc3D::Math::Rectangle* rect, int32 count);
			bool TryGetRectangleF(const KeyType& name, RectangleF* rect, int32 count);
			bool TryGetPoint(const KeyType& name, Point* pt, int32 count);
			bool TryGetPointF(const KeyType& name, PointF* pt, int32 count);
			bool TryGetSize(const KeyType& name, Apoc3D::Math::Size* sz, int32 count);
			bool TryGetBoundingBox(const KeyType& name, BoundingBox* bb, int32 count);
			bool TryGetBoundingSphere(const KeyType& name, BoundingSphere* bs, int32 count);
			bool TryGetRay(const KeyType& name, Ray* r, int32 count);
			bool TryGetViewport(const KeyType& name, Viewport* vp, int32 count);
			bool TryGetString(const KeyType& name, String* str, int32 count);



			int64 GetChunkOffset(const KeyType& name) const;

			void Close(bool seekToEnd = false);

			/** Exports a list of key names to the specified list. */
			void FillTagList(List<KeyType>& nameTags) const;

			bool isEndianIndependent() const { return !m_endianIndependent; }
			Stream* getBaseStream() const { return m_stream; }
		private:
			
			struct Entry
			{
				KeyType Name;

				int64 Offset;
				uint32 Size;

				Entry(const KeyType& name, int64 offset, uint32 size)
					: Name(name), Offset(offset), Size(size) { }

				Entry() { }
			};

			typedef HashMap<KeyType, Entry> SectionTable;

			inline void FillBuffer(const KeyType& name, uint32 len);
			inline void FillBuffer(const Entry& ent, uint32 len);
			inline void FillBufferCurrent(uint32 len);
			inline bool TryFillBuffer(const KeyType& name, uint32 len);


			void _GetEntryInt64(const Entry* e, int64& val);
			void _GetEntryInt32(const Entry* e, int32& val);
			void _GetEntryInt16(const Entry* e, int16& val);
			void _GetEntryUInt64(const Entry* e, uint64& val);
			void _GetEntryUInt32(const Entry* e, uint32& val);
			void _GetEntryUInt16(const Entry* e, uint16& val);
			void _GetEntryBool(const Entry* e, bool& val);
			void _GetEntrySingle(const Entry* e, float& val);
			void _GetEntryDouble(const Entry* e, double& val);

			void _GetEntryUInt64(const Entry* e, uint64* val, int32 len);
			void _GetEntryUInt32(const Entry* e, uint32* val, int32 len);
			void _GetEntryUInt16(const Entry* e, uint16* val, int32 len);
			void _GetEntryInt64(const Entry* e, int64* val, int32 len);
			void _GetEntryInt32(const Entry* e, int32* val, int32 len);
			void _GetEntryInt16(const Entry* e, int16* val, int32 len);
			void _GetEntryBool(const Entry* e, bool* val, int32 len);
			void _GetEntrySingle(const Entry* e, float* val, int32 len);
			void _GetEntryDouble(const Entry* e, double* val, int32 len);

			void _GetEntryVector2(const Entry* e, Vector2& vec);
			void _GetEntryVector3(const Entry* e, Vector3& vec);
			void _GetEntryVector4(const Entry* e, Vector4& vec);
			void _GetEntryColor4(const Entry* e, Color4& clr);
			void _GetEntryMatrix(const Entry* e, Matrix& mat);
			void _GetEntryString(const Entry* e, String& str);
			void _GetEntryPlane(const Entry* e, Plane& plane);
			void _GetEntryQuaternion(const Entry* e, Quaternion& quat);
			void _GetEntryRectangle(const Entry* e, Apoc3D::Math::Rectangle& rect);
			void _GetEntryRectangleF(const Entry* e, RectangleF& rect);
			void _GetEntryPoint(const Entry* e, Point& pt);
			void _GetEntryPointF(const Entry* e, PointF& pt);
			void _GetEntryMathSize(const Entry* e, Apoc3D::Math::Size& sz);
			void _GetEntryBoundingBox(const Entry* e, BoundingBox& bb);
			void _GetEntryBoundingSphere(const Entry* e, BoundingSphere& bs);
			void _GetEntryRay(const Entry* e, Ray& r);
			void _GetEntryViewport(const Entry* e, Viewport& vp);
			
			void _GetEntryVector2(const Entry* ent, Vector2* value, int len);
			void _GetEntryVector3(const Entry* ent, Vector3* value, int len);
			void _GetEntryVector4(const Entry* ent, Vector4* value, int len);
			void _GetEntryColor4(const Entry* ent, Color4* value, int len);
			void _GetEntryMatrix(const Entry* ent, Matrix* value, int len);
			void _GetEntryString(const Entry* ent, String* value, int len);
			void _GetEntryPlane(const Entry* e, Plane* plane, int len);
			void _GetEntryQuaternion(const Entry* e, Quaternion* quat, int len);
			void _GetEntryRectangle(const Entry* e, Apoc3D::Math::Rectangle* rect, int len);
			void _GetEntryRectangleF(const Entry* e, RectangleF* rect, int len);
			void _GetEntryPoint(const Entry* e, Point* pt, int len);
			void _GetEntryPointF(const Entry* e, PointF* pt, int len);
			void _GetEntryMathSize(const Entry* e, Apoc3D::Math::Size* sz, int len);
			void _GetEntryBoundingBox(const Entry* e, BoundingBox* bb, int len);
			void _GetEntryBoundingSphere(const Entry* e, BoundingSphere* bs, int len);
			void _GetEntryRay(const Entry* e, Ray* r, int len);
			void _GetEntryViewport(const Entry* e, Viewport* vp, int len);

			const Entry* FindEntry(const KeyType& name) const { return m_positions.TryGetValue(name); }

			NO_INLINE static void throwKeynotFoundException(const KeyType& key);

			bool m_endianIndependent;
			bool m_suspendStreamRelease = false;

			int m_sectCount;
			SectionTable m_positions;
			Stream* m_stream;

			char m_buffer[32];
			uint32 m_sizeInBytes;
			int64 m_initialPosition;
		};

		/** This class implements a writer for Tagged Data. */
		class APAPI TaggedDataWriter
		{
		public:
			typedef std::string KeyType;

			/**
			 * param isEndianIndependent true if the data medium is a fixed Endianness across platform.
			 *		 false for situations like system memory.
			 */
			TaggedDataWriter(bool isEndianIndependent);
			~TaggedDataWriter();

			BinaryWriter* AddEntry(const KeyType& name);
			Stream* AddEntryStream(const KeyType& name);

			void AddEntry(const KeyType& name, FunctorReference<void(BinaryWriter*)> func);
			void AddEntryStream(const KeyType& name, FunctorReference<void(Stream*)> func);

			void AddEntryDataSection(const KeyType& name, FunctorReference<void(TaggedDataWriter*)> func);

			void AddAuto(const KeyType& name, int64 value);
			void AddAuto(const KeyType& name, int32 value);
			void AddAuto(const KeyType& name, int16 value);
			void AddAuto(const KeyType& name, uint64 value);
			void AddAuto(const KeyType& name, uint32 value);
			void AddAuto(const KeyType& name, uint16 value);
			void AddAuto(const KeyType& name, float value);
			void AddAuto(const KeyType& name, double value);
			void AddAuto(const KeyType& name, bool value);
			void AddAuto(const KeyType& name, const Vector2& vec);
			void AddAuto(const KeyType& name, const Vector3& vec);
			void AddAuto(const KeyType& name, const Vector4& vec);
			void AddAuto(const KeyType& name, const Matrix& mat);
			void AddAuto(const KeyType& name, const Color4& clr);
			void AddAuto(const KeyType& name, const Plane& plane);
			void AddAuto(const KeyType& name, const Quaternion& quad);
			void AddAuto(const KeyType& name, const String& str);
			void AddAuto(const KeyType& name, const Apoc3D::Math::Rectangle& rect);
			void AddAuto(const KeyType& name, const RectangleF& rect);
			void AddAuto(const KeyType& name, const Point& pt);
			void AddAuto(const KeyType& name, const PointF& pt);
			void AddAuto(const KeyType& name, const Apoc3D::Math::Size& sz);
			void AddAuto(const KeyType& name, const BoundingBox& bb);
			void AddAuto(const KeyType& name, const BoundingSphere& bs);
			void AddAuto(const KeyType& name, const Ray& r);
			void AddAuto(const KeyType& name, const Viewport& vp);

			void AddAuto(const KeyType& name, const int64* value, int32 count);
			void AddAuto(const KeyType& name, const int32* value, int32 count);
			void AddAuto(const KeyType& name, const int16* value, int32 count);
			void AddAuto(const KeyType& name, const uint64* value, int32 count);
			void AddAuto(const KeyType& name, const uint32* value, int32 count);
			void AddAuto(const KeyType& name, const uint16* value, int32 count);
			void AddAuto(const KeyType& name, const float* value, int32 count);
			void AddAuto(const KeyType& name, const double* value, int32 count);
			void AddAuto(const KeyType& name, const bool* value, int32 count);
			void AddAuto(const KeyType& name, const Vector2* vec, int32 count);
			void AddAuto(const KeyType& name, const Vector3* vec, int32 count);
			void AddAuto(const KeyType& name, const Vector4* vec, int32 count);
			void AddAuto(const KeyType& name, const Matrix* mat, int32 count);
			void AddAuto(const KeyType& name, const Color4* clr, int32 count);
			void AddAuto(const KeyType& name, const Plane* plane, int32 count);
			void AddAuto(const KeyType& name, const Quaternion* quad, int32 count);
			void AddAuto(const KeyType& name, const String* str, int32 count);
			void AddAuto(const KeyType& name, const Apoc3D::Math::Rectangle* rect, int32 count);
			void AddAuto(const KeyType& name, const RectangleF* rect, int32 count);
			void AddAuto(const KeyType& name, const Point* pt, int32 count);
			void AddAuto(const KeyType& name, const PointF* pt, int32 count);
			void AddAuto(const KeyType& name, const Apoc3D::Math::Size* sz, int32 count);
			void AddAuto(const KeyType& name, const BoundingBox* bb, int32 count);
			void AddAuto(const KeyType& name, const BoundingSphere* bs, int32 count);
			void AddAuto(const KeyType& name, const Ray* r, int32 count);
			void AddAuto(const KeyType& name, const Viewport* vp, int32 count);
			

			void AddInt64(const KeyType& name, int64 value);
			void AddInt32(const KeyType& name, int32 value);
			void AddInt16(const KeyType& name, int16 value);
			void AddUInt64(const KeyType& name, uint64 value);
			void AddUInt32(const KeyType& name, uint32 value);
			void AddUInt16(const KeyType& name, uint16 value);
			void AddSingle(const KeyType& name, float value);
			void AddDouble(const KeyType& name, double value);
			void AddBool(const KeyType& name, bool value);

			void AddInt64(const KeyType& name, const int64* value, int32 count);
			void AddInt32(const KeyType& name, const int32* value, int32 count);
			void AddInt16(const KeyType& name, const int16* value, int32 count);
			void AddUInt64(const KeyType& name, const uint64* value, int32 count);
			void AddUInt32(const KeyType& name, const uint32* value, int32 count);
			void AddUInt16(const KeyType& name, const uint16* value, int32 count);
			void AddSingle(const KeyType& name, const float* value, int32 count);
			void AddDouble(const KeyType& name, const double* value, int32 count);
			void AddBool(const KeyType& name, const bool* value, int32 count);
			
			template <int32 N> void AddInt64(const KeyType& name, const int64(&value)[N]) { AddInt64(name, value, N); }
			template <int32 N> void AddInt32(const KeyType& name, const int32(&value)[N]) { AddInt32(name, value, N); }
			template <int32 N> void AddInt16(const KeyType& name, const int16(&value)[N]) { AddInt16(name, value, N); }
			template <int32 N> void AddUInt64(const KeyType& name, const uint64(&value)[N]) { AddUInt64(name, value, N); }
			template <int32 N> void AddUInt32(const KeyType& name, const uint32(&value)[N]) { AddUInt32(name, value, N); }
			template <int32 N> void AddUInt16(const KeyType& name, const uint16(&value)[N]) { AddUInt16(name, value, N); }
			template <int32 N> void AddSingle(const KeyType& name, const float(&value)[N]) { AddSingle(name, value, N); }
			template <int32 N> void AddDouble(const KeyType& name, const double(&value)[N]) { AddDouble(name, value, N); }
			template <int32 N> void AddBool(const KeyType& name, const bool(&value)[N]) { AddBool(name, value, N); }



			void AddVector2(const KeyType& name, const Vector2& vec);
			void AddVector3(const KeyType& name, const Vector3& vec);
			void AddVector4(const KeyType& name, const Vector4& vec);
			void AddMatrix(const KeyType& name, const Matrix& mat);
			void AddColor4(const KeyType& name, const Color4& clr);
			void AddString(const KeyType& name, const String& str);
			void AddPlane(const KeyType& name, const Plane& plane);
			void AddQuaternion(const KeyType& name, const Quaternion& quad);
			void AddRectangle(const KeyType& name, const Apoc3D::Math::Rectangle& rect);
			void AddRectangleF(const KeyType& name, const RectangleF& rect);
			void AddPoint(const KeyType& name, const Point& pt);
			void AddPointF(const KeyType& name, const PointF& pt);
			void AddMathSize(const KeyType& name, const Apoc3D::Math::Size& sz);
			void AddBoundingBox(const KeyType& name, const BoundingBox& bb);
			void AddBoundingSphere(const KeyType& name, const BoundingSphere& bs);
			void AddRay(const KeyType& name, const Ray& r);
			void AddViewport(const KeyType& name, const Viewport& vp);


			void AddVector2(const KeyType& name, const Vector2* vec, int32 count);
			void AddVector3(const KeyType& name, const Vector3* vec, int32 count);
			void AddVector4(const KeyType& name, const Vector4* vec, int32 count);
			void AddMatrix(const KeyType& name, const Matrix* mat, int32 count);
			void AddColor4(const KeyType& name, const Color4* clr, int32 count);
			void AddString(const KeyType& name, const String* str, int32 count);
			void AddPlane(const KeyType& name, const Plane* plane, int32 count);
			void AddQuaternion(const KeyType& name, const Quaternion* quad, int32 count);
			void AddRectangle(const KeyType& name, const Apoc3D::Math::Rectangle* rect, int32 count);
			void AddRectangleF(const KeyType& name, const RectangleF* rect, int32 count);
			void AddPoint(const KeyType& name, const Point* pt, int32 count);
			void AddPointF(const KeyType& name, const  PointF* pt, int32 count);
			void AddMathSize(const KeyType& name, const Apoc3D::Math::Size* sz, int32 count);
			void AddBoundingBox(const KeyType& name, const BoundingBox* bb, int32 count);
			void AddBoundingSphere(const KeyType& name, const BoundingSphere* bs, int32 count);
			void AddRay(const KeyType& name, const Ray* r, int32 count);
			void AddViewport(const KeyType& name, const Viewport* vp, int32 count);

			BinaryWriter* GetData(const KeyType& name);

			void SetInt64(const KeyType& name, int64 value);
			void SetInt32(const KeyType& name, int32 value);
			void SetInt16(const KeyType& name, int16 value);
			void SetUInt64(const KeyType& name, uint64 value);
			void SetUInt32(const KeyType& name, uint32 value);
			void SetUInt16(const KeyType& name, uint16 value);
			void SetSingle(const KeyType& name, float value);
			void SetDouble(const KeyType& name, double value);
			void SetBool(const KeyType& name, bool value);
			
			void SetInt64(const KeyType& name, const int64* value, int32 count);
			void SetInt32(const KeyType& name, const int32* value, int32 count);
			void SetInt16(const KeyType& name, const int16* value, int32 count);
			void SetUInt64(const KeyType& name, const uint64* value, int32 count);
			void SetUInt32(const KeyType& name, const uint32* value, int32 count);
			void SetUInt16(const KeyType& name, const uint16* value, int32 count);
			void SetSingle(const KeyType& name, const float* value, int32 count);
			void SetDouble(const KeyType& name, const double* value, int32 count);
			void SetBool(const KeyType& name, const bool* value, int32 count);

			void SetVector2(const KeyType& name, const Vector2& vec);
			void SetVector3(const KeyType& name, const Vector3& vec);
			void SetVector4(const KeyType& name, const Vector4& vec);
			void SetMatrix(const KeyType& name, const Matrix& mat);
			void SetColor4(const KeyType& name, const Color4& clr);
			void SetString(const KeyType& name, const String& str);
			void SetPlane(const KeyType& name, const Plane& plane);
			void SetQuaternion(const KeyType& name, const Quaternion& quad);
			void SetRectangle(const KeyType& name, const Apoc3D::Math::Rectangle& rect);
			void SetRectangleF(const KeyType& name, const RectangleF& rect);
			void SetPoint(const KeyType& name, const Point& pt);
			void SetPointF(const KeyType& name, const PointF& pt);
			void SetMathSize(const KeyType& name, const Apoc3D::Math::Size& sz);
			void SetBoundingBox(const KeyType& name, const BoundingBox& bb);
			void SetBoundingSphere(const KeyType& name, const BoundingSphere& bs);
			void SetRay(const KeyType& name, const Ray& r);
			void SetViewport(const KeyType& name, const Viewport& vp);

			void SetVector2(const KeyType& name, const Vector2* vec, int32 count);
			void SetVector3(const KeyType& name, const Vector3* vec, int32 count);
			void SetVector4(const KeyType& name, const Vector4* vec, int32 count);
			void SetMatrix(const KeyType& name, const Matrix* mat, int32 count);
			void SetColor4(const KeyType& name, const Color4* clr, int32 count);
			void SetString(const KeyType& name, const String* str, int32 count);
			void SetPlane(const KeyType& name, const Plane* plane, int32 count);
			void SetQuaternion(const KeyType& name, const Quaternion* quad, int32 count);
			void SetRectangle(const KeyType& name, const Apoc3D::Math::Rectangle* rect, int32 count);
			void SetRectangleF(const KeyType& name, const RectangleF* rect, int32 count);
			void SetPoint(const KeyType& name, const Point* pt, int32 count);
			void SetPointF(const KeyType& name, const PointF* pt, int32 count);
			void SetMathSize(const KeyType& name, const Apoc3D::Math::Size* sz, int32 count);
			void SetBoundingBox(const KeyType& name, const BoundingBox* bb, int32 count);
			void SetBoundingSphere(const KeyType& name, const BoundingSphere* bs, int32 count);
			void SetRay(const KeyType& name, const Ray* r, int32 count);
			void SetViewport(const KeyType& name, const Viewport* vp, int32 count);


			Apoc3D::Config::ConfigurationSection* MakeDigest(const KeyType& name) const;
			void Save(Stream& stream) const;

			bool Contains(const KeyType& name) const;

			bool isEndianIndependent() const { return !m_endianIndependent; }

		private:
			struct Entry
			{
				KeyType Name;
				MemoryOutStream* Buffer;
				Entry(const KeyType& name);

				Entry() : Buffer(nullptr) { }
				void ResetWritePosition() const;
			};

			typedef HashMap<KeyType, Entry> SectionTable;

			const Entry* FindEntry(const KeyType& name) const
			{
				return m_positions.TryGetValue(name);
			}

			void _SetEntryDataInt64(const Entry& ent, int64 value);
			void _SetEntryDataInt32(const Entry& ent, int32 value);
			void _SetEntryDataInt16(const Entry& ent, int16 value);
			void _SetEntryDataUInt64(const Entry& ent, uint64 value);
			void _SetEntryDataUInt32(const Entry& ent, uint32 value);
			void _SetEntryDataUInt16(const Entry& ent, uint16 value);
			void _SetEntryDataSingle(const Entry& ent, float value);
			void _SetEntryDataDouble(const Entry& ent, double value);
			void _SetEntryDataBool(const Entry& ent, bool value);
			
			void _SetEntryDataInt64(const Entry& ent, const int64* value, int32 count);
			void _SetEntryDataInt32(const Entry& ent, const int32* value, int32 count);
			void _SetEntryDataInt16(const Entry& ent, const int16* value, int32 count);
			void _SetEntryDataUInt64(const Entry& ent, const uint64* value, int32 count);
			void _SetEntryDataUInt32(const Entry& ent, const uint32* value, int32 count);
			void _SetEntryDataUInt16(const Entry& ent, const uint16* value, int32 count);
			void _SetEntryDataSingle(const Entry& ent, const float* value, int32 count);
			void _SetEntryDataDouble(const Entry& ent, const double* value, int32 count);
			void _SetEntryDataBool(const Entry& ent, const bool* value, int32 count);



			void _SetEntryDataVector2(const Entry& ent, const Vector2& vec);
			void _SetEntryDataVector3(const Entry& ent, const Vector3& vec);
			void _SetEntryDataVector4(const Entry& ent, const Vector4& vec);
			void _SetEntryDataMatrix(const Entry& ent, const Matrix& mat);
			void _SetEntryDataColor4(const Entry& ent, const Color4& clr);
			void _SetEntryDataString(const Entry& ent, const String& str);
			void _SetEntryDataPlane(const Entry& ent, const Plane& plane);
			void _SetEntryDataQuaternion(const Entry& ent, const Quaternion& quat);
			void _SetEntryDataRectangle(const Entry& ent, const Apoc3D::Math::Rectangle& rect);
			void _SetEntryDataRectangleF(const Entry& ent, const RectangleF& rect);
			void _SetEntryDataPoint(const Entry& ent, const Point& pt);
			void _SetEntryDataPointF(const Entry& ent, const PointF& pt);
			void _SetEntryDataMathSize(const Entry& ent, const Apoc3D::Math::Size& sz);
			void _SetEntryDataBoundingBox(const Entry& ent, const BoundingBox& bb);
			void _SetEntryDataBoundingSphere(const Entry& ent, const BoundingSphere& bs);
			void _SetEntryDataRay(const Entry& ent, const Ray& r);
			void _SetEntryDataViewport(const Entry& ent, const Viewport& vp);

			void _SetEntryDataVector2(const Entry& ent, const Vector2* vec, int32 count);
			void _SetEntryDataVector3(const Entry& ent, const Vector3* vec, int32 count);
			void _SetEntryDataVector4(const Entry& ent, const Vector4* vec, int32 count);
			void _SetEntryDataMatrix(const Entry& ent, const Matrix* mat, int32 count);
			void _SetEntryDataColor4(const Entry& ent, const Color4* clr, int32 count);
			void _SetEntryDataString(const Entry& ent, const String* str, int32 count);
			void _SetEntryDataPlane(const Entry& ent, const Plane* plane, int32 count);
			void _SetEntryDataQuaternion(const Entry& ent, const Quaternion* quat, int32 count);
			void _SetEntryDataRectangle(const Entry& ent, const Apoc3D::Math::Rectangle* rect, int32 count);
			void _SetEntryDataRectangleF(const Entry& ent, const RectangleF* rect, int32 count);
			void _SetEntryDataPoint(const Entry& ent, const Point* pt, int32 count);
			void _SetEntryDataPointF(const Entry& ent, const PointF* pt, int32 count);
			void _SetEntryDataMathSize(const Entry& ent, const Apoc3D::Math::Size* sz, int32 count);
			void _SetEntryDataBoundingBox(const Entry& ent, const BoundingBox* bb, int32 count);
			void _SetEntryDataBoundingSphere(const Entry& ent, const BoundingSphere* bs, int32 count);
			void _SetEntryDataRay(const Entry& ent, const Ray* r, int32 count);
			void _SetEntryDataViewport(const Entry& ent, const Viewport* vp, int32 count);


			bool m_endianIndependent;
			SectionTable m_positions;
			char m_buffer[32];

		};
	}
}

#endif