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

		StringBase() 
		{
			m_content.localBuffer[0] = 0;
		}

		StringBase(const CharType* str)
			: this(str, StrLen(str)) { }

		StringBase(const CharType* str, int32 len)
		{
			Resize(len + 1, true);
			AssignInplace(str, len, 0);
		}

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

					Resize(other.m_capacity, true);
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

		StringBase& operator=(const CharType* o)
		{
			int32 len = StrLen(str);

			Resize(len + 1, true);
			AssignInplace(o, len, 0);

			return *this;
		}


		StringBase& operator +=(const StringBase& o) { Append(o); return *this; }
		StringBase& operator +=(const CharType* o) { Append(o); return *this; }
		StringBase& operator +=(CharType o) { Append(o); return *this; }


		void Append(const CharType* str, int32 len)
		{
			EnsureIncrCapacity(len);

			AssignInplace(o, len, m_length);
		}

		void Append(CharType ch)
		{
			Append(&ch, 1);
		}

		void Append(const CharType* str)
		{
			Append(str, StrLen(str));
		}

		void Append(const StringBase& str)
		{
			Append(str.c_str(), str.m_length);
		}

		void Insert(const CharType* str, int32 len, int32 pos)
		{
			EnsureIncrCapacity(len);

			assert(len + m_length <= m_capacity);

			CharType* buf = c_str();

			buf[len + m_length] = 0;
			for (int32 i = len + m_length - 1; i >= len + pos; i--)
			{
				buf[i] = buf[i - len];
			}
			
			memcpy(buf + pos, src, len * sizeof(CharType));
			
			m_length += len;
		}

		void Insert(CharType ch)
		{
			Insert(&ch, 1, pos);
		}

		void Insert(const CharType* str, int32 pos)
		{
			Insert(str, StrLen(str), pos);
		}

		void Insert(const StringBase& str, int32 pos)
		{
			Insert(str.c_str(), str.m_length, pos);
		}

		StringBase SubString(int32 pos = 0, int32 len = -1) const
		{
			if (m_length == 0 || pos == m_length)
				return StringBase();

			pos = Clamp(pos, 0, m_length - 1);

			if (len == -1)
			{
				len = m_length - pos;
			}
			else
			{
				len = Min(m_length - pos, len);
			}
			return StringBase(c_str() + pos, len);
		}

		int32 Find(CharType ch, int32 start) const
		{
			const CharType* str = c_str();
			for (int32 i = start; i < m_length; i++)
			{
				if (str[i] == ch)
					return i;
			}
			return -1;
		}

		void Resize(int32 newCapacity, bool discardContent = false)
		{
			if (newCapacity > LocalBufferLength)
			{
				CharType* buf = new CharType[newCapacity];

				if (!discardContent)
				{
					CopyTo(buf);
				}
				else
				{
					buf[0] = 0;
				}

				if (isAllocated())
				{
					delete[] m_content.allocatedBuffer;
				}

				m_content.allocatedBuffer = buf;

				m_capacity = newCapacity;
			}
		}


		int32 getLength() const { return m_length; }

		CharType* begin() { return c_str(); }
		CharType* end() { return c_str() + m_length; }

		const CharType* c_str() const { return isAllocated() ? m_content.allocatedBuffer : m_content.localBuffer; }
		CharType* c_str() { return isAllocated() ? m_content.allocatedBuffer : m_content.localBuffer; }

		const CharType& operator[](int32 i) const { return c_str()[i]; }
		CharType& operator[](int32 i) { return c_str()[i]; }

		static int32 StrCmp(const CharType* s1, const CharType* s2)
		{
			while (*s1 && (*s1 == *s2))
				s1++, s2++;
			return *(const unsigned CharType*)s1 - *(const unsigned CharType*)s2;
		}

	private:
		bool isAllocated() const { return m_capacity > LocalBufferLength; }

		void EnsureIncrCapacity(int32 more)
		{
			int32 finalLength = m_length + more + 1;

			int32 capacity = m_capacity;
			if (finalLength > capacity)
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
			memcpy(dest, c_str(), (m_length + 1) * sizeof(CharType));
		}
		void AssignInplace(const CharType* src, int32 srcLen, int32 startOffset)
		{
			assert(srcLen + 1 + startOffset <= m_capacity);

			CharType* dst = c_str() + startOffset;
			memcpy(dst, src, srcLen * sizeof(CharType));
			dst[srcLen] = 0;

			m_length = startOffset + srcLen;
		}
		
		static int32 StrLen(const char* str)
		{
			return (int32)strlen(str);
		}
		static int32 StrLen(const wchar_t* str)
		{
			return (int32)wcslen(str);
		}


		union
		{
			CharType localBuffer[LocalBufferLength];

			CharType* allocatedBuffer;
		} m_content;

		int32 m_length = 0;
		int32 m_capacity = LocalBufferLength;

	};

	
	template <typename T>
	StringBase<T> operator+ (const StringBase<T>& lhs, const StringBase<T>& rhs)
	{
		StringBase<T> r = lhs;
		r.Append(rhs);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (StringBase<T>&& lhs, StringBase<T>&& rhs)
	{
		StringBase<T> r = std::move(lhs);
		r.Append(rhs);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (const StringBase<T>& lhs, const T* rhs)
	{
		StringBase<T> r = lhs;
		r.Append(rhs);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (StringBase<T>&& lhs, const T* rhs)
	{
		StringBase<T> r = std::move(lhs);
		r.Append(rhs);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (const T* lhs, const StringBase<T>& rhs)
	{
		StringBase<T> r = rhs;
		r.Insert(lhs, 0);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (const T* lhs, StringBase<T>&& rhs)
	{
		StringBase<T> r = std::move(rhs);
		r.Insert(lhs, 0);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (const StringBase<T>& lhs, T rhs)
	{
		StringBase<T> r = lhs;
		r.Append(rhs);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (StringBase<T>&& lhs, T rhs)
	{
		StringBase<T> r = std::move(lhs);
		r.Append(rhs);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (T lhs, const StringBase<T>& rhs)
	{
		StringBase<T> r = rhs;
		r.Insert(rhs, 0);
		return r;
	}

	template <typename T>
	StringBase<T> operator+ (T lhs, StringBase<T>&& rhs)
	{
		StringBase<T> r = std::move(rhs);
		r.Insert(rhs, 0);
		return r;
	}


	template <typename T>
	bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs)
	{
		return lhs.getLength() == rhs.getLength() &&
			memcmp(lhs.c_str(), rhs.c_str(), lhs.getLength() * sizeof(T)) == 0;
	}

	template <typename T>
	bool operator==(const T* lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs, rhs.c_str()) == 0; }
	template <typename T>
	bool operator==(const StringBase<T>& lhs, const T* rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs) == 0; }

	template <typename T>
	bool operator!=(const StringBase<T>& lhs, const StringBase<T>& rhs) { return !operator==(lhs, rhs); }
	template <typename T>
	bool operator!=(const T* lhs, const StringBase<T>& rhs) { return !operator==(lhs, rhs); }
	template <typename T>
	bool operator!=(const StringBase<T>& lhs, const T* rhs) { return !operator==(lhs, rhs); }

	template <typename T>
	bool operator<(const StringBase<T>& lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs.c_str()) < 0; }
	template <typename T>
	bool operator<(const T* lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs, rhs.c_str()) < 0; }
	template <typename T>
	bool operator<(const StringBase<T>& lhs, const T* rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs) < 0; }
	
	template <typename T>
	bool operator<=(const StringBase<T>& lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs.c_str()) <= 0; }
	template <typename T>
	bool operator<=(const T* lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs, rhs.c_str()) <= 0; }
	template <typename T>
	bool operator<=(const StringBase<T>& lhs, const T* rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs) <= 0; }
	
	template <typename T>
	bool operator>(const StringBase<T>& lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs.c_str()) > 0; }
	template <typename T>
	bool operator>(const T* lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs, rhs.c_str()) > 0; }
	template <typename T>
	bool operator>(const StringBase<T>& lhs, const T* rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs) > 0; }
	
	template <typename T>
	bool operator>=(const StringBase<T>& lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs.c_str()) >= 0; }
	template <typename T>
	bool operator>=(const T* lhs, const StringBase<T>& rhs) { return StringBase<T>::StrCmp(lhs, rhs.c_str()) >= 0; }
	template <typename T>
	bool operator>=(const StringBase<T>& lhs, const T* rhs) { return StringBase<T>::StrCmp(lhs.c_str(), rhs) >= 0; }
}

#endif