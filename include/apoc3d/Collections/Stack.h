#pragma once

#ifndef APOC3D_STACK_H
#define APOC3D_STACK_H

/*
-----------------------------------------------------------------------------
This source file is part of Apoc3D Engine

Copyright (c) 2009+ Tao Xin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  if not, write to the Free Software Foundation, 
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.txt.

-----------------------------------------------------------------------------
*/
#include "CollectionsCommon.h"

namespace Apoc3D
{
	namespace Collections
	{
		template<typename T>
		class Stack
		{
		public:	
			explicit Stack(int capacity = 8)
				: m_capacity(capacity)
			{
				m_array = Allocate(m_capacity); // new T[m_length];
			}
			~Stack()
			{
				Destruct();
			}


			Stack(const Stack& another)
				: m_count(another.m_count), m_capacity(another.m_capacity)
			{
				m_array = Allocate(m_capacity);// new T[m_length];

				Utils::CopyToNew(m_array, another.m_array, m_count);
			}

			Stack& operator=(const Stack& another)
			{
				if (this != &another)
				{
					Destruct();

					m_count = another.m_count;
					m_capacity = another.m_capacity;
					m_array = Allocate(m_capacity);// new T[m_length];
					Utils::CopyToNew(m_array, another.m_array, m_count);
				}
				return *this;
			}

			Stack(Stack&& another)
				: m_count(another.m_count), m_capacity(another.m_capacity)
			{
				m_array = another.m_array;
				another.m_array = nullptr;
				another.m_count = another.m_capacity = 0;
			}

			Stack& operator=(Stack&& another)
			{
				if (this != &another)
				{
					Destruct();

					m_count = another.m_count;
					m_capacity = another.m_capacity;
					m_array = another.m_array;

					another.m_array = nullptr;
					another.m_count = another.m_capacity = 0;
				}
				return *this;
			}

			int getCount() const { return m_count; }

			void Clear()
			{
				Utils::DoDestory(0, m_count, m_array);
				m_count = 0; 
			}

			bool Contains(const T& item)
			{
				for (int32 i = m_count - 1; i >= 0; i--)
					if (m_array[i] == item)
						return true;
				return false;
			}

			const T& Peek() const { assert(m_count > 0); return m_array[m_count - 1]; }
			T& Peek() { assert(m_count > 0); return m_array[m_count - 1]; }

			T Pop() 
			{
				assert(m_count > 0);
				
				T temp = std::move(m_array[--m_count]);
				Utils::DoDestory(m_count, 1, m_array);
				return temp;
			}
			
			void FastPop() 
			{
				assert(m_count > 0);
				Utils::DoDestory(--m_count, 1, m_array);
			}


			void Push(const T& item)
			{
				if (m_count == m_capacity)
				{
					if (m_capacity == 0)
						m_capacity = 1;
					else
						m_capacity *= 2;

					T* destinationArray = Allocate(m_capacity * 2);
					Utils::MoveToNew(destinationArray, m_array, m_count);
					
					Utils::DoDestory(0, m_count, m_array);
					Free(m_array);

					m_array = destinationArray;
				}

				Utils::DoPutNew(item, m_count++, m_array);
			}

		private:
			static T* Allocate(int32 size) { return (T*)malloc(sizeof(T)*size); }
			static void Free(T*& ptr) { free(ptr); ptr = nullptr; }

			void Destruct()
			{
				Clear();
				Free(m_array);
			}

			T* m_array;
			int m_count = 0;
			int m_capacity;
		};

		template<typename T, int32 N>
		class FixedStack
		{
		public:
			FixedStack() 
			{

			}
			~FixedStack()
			{
				Clear();
			}

			FixedStack(const FixedStack& o)
				: m_count(o.m_count)
			{
				Utils::CopyToNew((T*)m_storage, (T*)o.m_storage, m_count);
			}
			FixedStack(FixedStack&& o)
				: m_count(o.m_count)
			{
				Utils::MoveToNew((T*)m_storage, (T*)o.m_storage, m_count);
				o.Clear();
			}
			FixedStack& operator=(const FixedStack& o)
			{
				if (this != &o)
				{
					Clear();
					m_count = o.m_count;
					Utils::CopyToNew((T*)m_storage, (T*)o.m_storage, m_count);
				}
				return *this;
			}
			FixedStack& operator=(FixedStack&& o)
			{
				if (this != &o)
				{
					Clear();
					m_count = o.m_count;
					Utils::MoveToNew((T*)m_storage, (T*)o.m_storage, m_count);
					o.Clear();
				}
				return *this;
			}



			int getCount() const { return m_count; }

			void Clear() 
			{
				Utils::DoDestory(0, m_count, (T*)m_storage);

				m_count = 0; 
			}

			bool Contains(const T& item)
			{
				T* arr = (T*)m_storage;

				for (int32 i = m_count - 1; i >= 0; i--)
					if (arr[i] == item)
						return true;
				return false;
			}

			T& Peek() { assert(m_count > 0); return ((T*)m_storage)[m_count - 1]; }

			T Pop() 
			{
				assert(m_count > 0);

				T* arr = (T*)m_storage;

				T temp = std::move(arr[--m_count]);
				Utils::DoDestory(arr[m_count]);
				return temp;
			}

			void FastPop()
			{
				assert(m_count > 0);
				Utils::DoDestory(((T*)m_storage)[--m_count]);
			}

			void Push(const T& item)
			{
				assert(m_count < N);
				Utils::DoPutNew(item, m_count++, (T*)m_storage);
			}

		private:
			int m_count = 0;
			char m_storage[sizeof(T)*N];
		};
	}
}

#endif