#pragma once
#ifndef APOC3D_STRING_H
#define APOC3D_STRING_H

#include "Common.h"

namespace Apoc3D
{
	template <typename CharType>
	class StringBase
	{
		static const int LocalBufferSize = 24;
		static const int LocalBufferLength = LocalBufferSize / sizeof(CharType);

		static_assert((LocalBufferSize % sizeof(CharType)) == 0, "");
		static_assert(std::is_trivially_copyable<CharType>::value, "");

	public:
		StringBase(const StringBase& other)
			: m_length(other.m_length)
		{
			Resize(other.m_capacity);

			other.CopyTo(c_str());
		}

		StringBase(StringBase&& other)
			: m_content(other.m_content), m_length(other.m_length), m_capacity(other.m_capacity)
		{
			other.m_content.allocatedBuffer = nullptr;
			other.m_length = 0;
			other.m_capacity = 0;
		}

		~StringBase()
		{
			if (isAllocated())
			{
				delete[] m_content.allocatedBuffer;
				m_content.allocatedBuffer = nullptr;
			}
		}

		StringBase& operator=(const StringBase& other)
		{
			if (this != &other)
			{
				if (m_capacity != other.m_capacity)
				{
					this->~StringBase();

					m_capacity = LocalBufferLength;
					m_length = 0;

					Resize(other.m_capacity);
				}
				
				m_length = other.m_length;

				other.CopyTo(c_str());
			}
			return *this;
		}

		StringBase& operator=(StringBase&& other)
		{
			if (this != &other)
			{
				this->~StringBase();

				m_content = other.m_content;
				m_length = other.m_length;
				m_capacity = other.m_capacity;

				other.m_content.allocatedBuffer = nullptr;
				other.m_length = 0;
				other.m_capacity = 0;
			}
			return *this;
		}




		void Resize(int32 newCapacity)
		{
			if (newCapacity > LocalBufferLength)
			{
				CharType* buf = new CharType[newCapacity];
				CopyTo(buf);

				if (isAllocated())
				{
					delete[] m_content.allocatedBuffer;
				}

				m_content.allocatedBuffer = buf;

				m_capacity = newCapacity;
			}
		}

		CharType* begin() { return c_str(); }
		CharType* end() { return c_str() + m_length; }

		const CharType* c_str() const { return isAllocated() ? m_content.allocatedBuffer : m_content.localBuffer; }
		CharType* c_str() { return isAllocated() ? m_content.allocatedBuffer : m_content.localBuffer; }

	private:
		bool isAllocated() const { return m_capacity > LocalBufferLength; }

		void EnsureCapacity(int32 more)
		{
			int32 finalLength = m_length + more + 1;

			int32 capacity = m_capacity;
			if (finalLength >= m_capacity)
			{
				int32 nextSize = capacity * 2;
				if (more > LocalBufferLength &&
					nextSize < finalLength)
					nextSize = finalLength;

				Resize(nextSize);
			}
		}

		void CopyTo(CharType* dest)
		{
			memcpy(dest, c_str(), (m_length+1)*sizeof(CharType));
		}
		

		union
		{
			CharType localBuffer[LocalBufferLength];

			CharType* allocatedBuffer;
		} m_content;

		int32 m_length = 0;
		int32 m_capacity = LocalBufferLength;

	};

	template <typename CharType>
	class StringView
	{

	};
}

#endif