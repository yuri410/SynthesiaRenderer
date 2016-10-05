#pragma once
#ifndef OS_DATUM_H
#define OS_DATUM_H

#include "Common.h"
#include "Exception.h"

namespace os
{
	class Datum
	{
	public:

#define MAKE_SETSTORAGE(type, enumType, member) \
	/**                                 \
	 *	Set external storage. This can only be set once.	\
	 */									\
	void SetStorage(type* data, uint32 count) { CheckSetStorage(); SetType(enumType); m_isExternal = true; m_array.member = data; m_size = m_capacity = count; }

#define MAKE_SET(type, enumType, member) \
	/**                                 \
	 *	Set the data inside the datum.  \
	 *  This will increase the size when needed. 
	 *  Type will be set if SetType is not used before, otherwise the type must match.	\
	 */									\
	void Set(type const& val, uint32 index = 0) { SetterEnsureCapacity<type>(index); SetterEnsureType(enumType); m_array.member[index] = val; }

#define MAKE_GET(type, enumType,  member) \
	/**                                 \
	 *	Get the data inside the datum.	\
	 * Index is zero-based to size-1. Type must match.		\
	 */									\
	template<> type const& Get(uint32 index) const { CheckGetterIndexBounds(index); CheckType(enumType); return m_array.member[index]; }

#define MAKE_ASSIGNMENT(type) \
	/**                                 \
	 *	Assign this datum to a scalar data.		\
	 */									\
	Datum& operator=(type const& rhs) { Set(rhs); return *this; }

#define MAKE_COMPARISON(type, enumType, member) \
	bool operator==(type const& rhs) const { return m_size == 1 && m_type == enumType && m_array.member[0] == rhs; } \
	bool operator!=(type const& rhs) const { return !(*this == rhs); }

		/**
		 *  Represents types of data Datum can store
		 */
		enum Type
		{
			DAT_Unknown,
			DAT_Int32,
			DAT_Float,
			DAT_Vector4,
			DAT_Matrix,
			DAT_Table,
			DAT_String,
			DAT_Pointer,
			DAT_Count
		};

		/**
		 *  Initializes the datum to be empty.
		 */
		Datum();
		Datum(const Datum& other);
		~Datum();

		Datum& operator=(const Datum& rhs);
		
		MAKE_ASSIGNMENT(int32);
		MAKE_ASSIGNMENT(float);
		MAKE_ASSIGNMENT(Vector4);
		MAKE_ASSIGNMENT(Matrix);
		MAKE_ASSIGNMENT(Scope*);
		MAKE_ASSIGNMENT(String);
		MAKE_ASSIGNMENT(RTTI*);

		MAKE_COMPARISON(int32, DAT_Int32, i);
		MAKE_COMPARISON(float, DAT_Float, f);
		MAKE_COMPARISON(Matrix, DAT_Matrix, m);
		MAKE_COMPARISON(Scope*, DAT_Table, t);
		MAKE_COMPARISON(String, DAT_String, s);
		MAKE_COMPARISON(RTTI*, DAT_Pointer, p);

		bool operator==(const Vector4& rhs) const;
		bool operator!=(const Vector4& rhs) const { return !(*this == rhs); }

		bool operator==(const Datum& rhs) const;
		bool operator!=(const Datum& rhs) const { return !(*this == rhs); }

		/**
		 *  Removes all the values in the datum.
		 */
		void Clear();

		/**
		 *  Returns the type of the data.
		 */
		Type GetType() const { return m_type; }
		/**
		 *  Set the type of the data.
		 *  This can only be set once.
		 */
		void SetType(Type t);

		MAKE_SETSTORAGE(int32, DAT_Int32, i);
		MAKE_SETSTORAGE(float, DAT_Float, f);
		MAKE_SETSTORAGE(Vector4, DAT_Vector4, v);
		MAKE_SETSTORAGE(Matrix, DAT_Matrix, m);
		MAKE_SETSTORAGE(Scope*, DAT_Table, t);
		MAKE_SETSTORAGE(String, DAT_String, s);
		MAKE_SETSTORAGE(RTTI*, DAT_Pointer, p);


		MAKE_SET(int32, DAT_Int32, i);
		MAKE_SET(float, DAT_Float, f);
		MAKE_SET(Vector4, DAT_Vector4, v);
		MAKE_SET(Matrix, DAT_Matrix, m);
		MAKE_SET(Scope*, DAT_Table, t);
		MAKE_SET(String, DAT_String, s);
		MAKE_SET(RTTI*, DAT_Pointer, p);

		void Set(const Datum& val, uint32 index = 0);


		template<typename T> T const& Get(uint32 index) const
		{
			throw Exception(L"Not supported");
		}

		MAKE_GET(int32, DAT_Int32, i);
		MAKE_GET(float, DAT_Float, f);
		MAKE_GET(Vector4, DAT_Vector4, v);
		MAKE_GET(Matrix, DAT_Matrix, m);
		MAKE_GET(Scope*, DAT_Table, t);
		MAKE_GET(String, DAT_String, s);
		MAKE_GET(RTTI*, DAT_Pointer, p);

		/**
		 *  Reserve the size while keeping the values inside.
		 */
		void SetSize(uint32 size);

		/**
		 *  Returns the number of values.
		 */
		uint32 Size() const { return m_size; }

		/**
		 *  Parse from a String at given index
		 */
		void SetFromString(const String& text, uint32 index = 0);

		/**
		 *  Converts the value at given index.
		 */
		String ToString(uint32 index = 0) const;


		bool IsExternal() const { return m_isExternal; }
		Scope& operator[](uint32 index ) { return *Get<Scope*>(index ); }
	private:
		/**
		 *  Ensure capacity for internal storage & check bounds for external storage
		 */
		template <typename T>
		void SetterEnsureCapacity(uint32 idx)
		{
			if (!m_isExternal)
			{
				if (m_capacity<idx+1)
				{
					Resize<T>(idx+1);

					m_size = idx+1;
				}
			}
			else
			{
				if (idx >= m_capacity)
				{
					throw Exception(L"Index out of range");
				}
			}
		}

		/**
		 *  Check if the type is the same as the one given. 
		 *  Assign the type of this datum if it was unknown.
		 */
		void SetterEnsureType(Type t);

		/**
		 *  Check if the type is the same as the one given. 
		 */
		void CheckType(Type t) const;
		/**
		 *  Check bounds against size
		 */
		void CheckGetterIndexBounds(uint32 idx) const;
		
		/**
		 *  Check against any allocation made before
		 */
		void CheckSetStorage();


		template <typename T>
		void Resize(uint32 size);
		

		template <typename T>
		static void CopyArray(const T* src, T* dst, int32 count);

		template <typename T>
		static bool CompareArray(const T* a, const T* b, int32 count);

		union
		{
			int32* i;
			float* f;
			Vector4* v;
			Matrix* m;
			Scope** t;
			String* s;
			RTTI** p;

			void* universal;
		} m_array;

		bool m_isExternal;
		Type m_type;

		uint32 m_size;
		uint32 m_capacity;
	};
}

#endif