#pragma once

#ifndef APOC3D_LIST2D_H
#define APOC3D_LIST2D_H

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
#include "apoc3d/Common.h"

namespace Apoc3D
{
	namespace Collections
	{
		template<typename T>
		class List2D
		{
		public:
			List2D(int w, int h = 4)
				: m_width(w), m_height(h), m_internalPointer(0)
			{
				m_data = new T*[h]();
			}

			List2D(const List2D& another)
				: m_width(another.m_width), m_height(another.m_height), m_internalPointer(another.m_internalPointer)
			{
				m_data = new T*[m_height]();

				for (int i = 0; i < m_internalPointer; i++)
				{
					m_data[i] = new T[m_width];
					for (int j = 0; j < m_width; j++)
					{
						m_data[i][j] = another.m_data[i][j];
					}
				}
			}

			List2D(List2D&& other)
				: m_width(other.m_width), m_height(other.m_height), m_internalPointer(other.m_internalPointer), m_data(other.m_data)
			{
				other.m_data = nullptr;
				other.m_height = 0;
				other.m_internalPointer = 0;
			}

			~List2D()
			{
				if (m_data)
				{
					for (int i = 0; i < m_internalPointer; i++)
					{
						delete[] m_data[i];
					}
					delete[] m_data;

					m_data = nullptr;
				}
			}
			

			List2D& operator=(const List2D& rhs)
			{
				if (this != &rhs)
				{
					this->~List2D();

					m_internalPointer = rhs.m_internalPointer;
					m_width = rhs.m_width;
					m_height = rhs.m_height;

					m_data = new T*[m_height]();
					for (int i = 0; i < m_internalPointer; i++)
					{
						m_data[i] = new T[m_width];
						for (int j = 0; j < m_width; j++)
						{
							m_data[i][j] = rhs.m_data[i][j];
						}
					}
				}
				return *this;
			}

			List2D& operator=(List2D&& rhs)
			{
				if (this != &rhs)
				{
					this->~List2D();
					
					m_data = rhs.m_data;
					m_internalPointer = rhs.m_internalPointer;
					m_width = rhs.m_width;
					m_height = rhs.m_height;

					rhs.m_data = nullptr;
					rhs.m_height = 0;
					rhs.m_internalPointer = 0;
				}
				return *this;
			}

			bool isIndexInRange(int32 i, int32 j) const { return i >= 0 && i < m_internalPointer && j >= 0 && j < m_width; }
			bool isIndexInRange(int32 i) const { return i >= 0 && i < m_internalPointer; }

			int getCount() const { return m_internalPointer; }
			int getWidth() const { return m_width; }

			T& at(int32 i, int32 j) const
			{
				assert(i >= 0 && i < m_internalPointer);
				assert(j >= 0 && j < m_width);
				return m_data[i][j];
			}

			class RowSubscriptProxy
			{
			public:
				RowSubscriptProxy(T* row, int32 w)
					: m_row(row), m_width(w) { }

				T& operator[](int32 j) { assert(j >= 0 && j < m_width); return m_row[j]; }
			private:
				T* m_row;
				int32 m_width;
			};

			RowSubscriptProxy operator[](int32 i) { assert(i >= 0 && i < m_internalPointer); return RowSubscriptProxy(m_data[i], m_width); }

			void AddRow(const T* val)
			{
				if (m_internalPointer >= m_height)
				{
					ResizeRows(m_height == 0 ? 4 : (m_height * 2));
				}
				m_data[m_internalPointer] = new T[m_width];
				for (int i = 0; i < m_width; i++)
				{
					m_data[m_internalPointer][i] = val[i];
				}
				m_internalPointer++;
			}
			void Clear()
			{
				for (int i = 0; i < m_internalPointer; i++)
				{
					delete[] m_data[i];
				}
				m_internalPointer = 0;
				
			}
			void RemoveRow(int index)
			{
				assert(isIndexInRange(index));

				if (index == m_internalPointer - 1)
				{
					m_internalPointer--;
					delete[] m_data[m_internalPointer];
				}
				else
				{
					delete[] m_data[index];
					for (int i = index; i < m_internalPointer - 1; i++)
					{
						m_data[i] = m_data[i + 1];
					}
					m_internalPointer--;
				}
			}
			void RemoveRowRange(int index, int count)
			{
				if (count > 0 && index < m_internalPointer)
				{
					for (int i = index; i < index + count; i++)
						delete[] m_data[i];

					for (int i = index; i < m_internalPointer - count; i++)
					{
						m_data[i] = m_data[i + count];
					}
					m_internalPointer -= count;
				}
			}

		private:
			void ResizeRows(int newSize)
			{
				T** newArr = new T*[newSize]();
				memcpy(newArr, m_data, m_height*sizeof(T*));
				delete[] m_data;
				m_data = newArr;

				m_height = newSize;
			}

			T** m_data;
			int m_width;
			int m_height;
			int m_internalPointer;
		};
	}
}

#endif