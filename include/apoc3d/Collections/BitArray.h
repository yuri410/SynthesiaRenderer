#pragma once

/* -----------------------------------------------------------------------
 * This source file is part of Apoc3D Framework
 * 
 * Copyright (c) 2009-2014 Tao Xin
 * 
 * This content of this file is subject to the terms of the Mozilla Public 
 * License v2.0. If a copy of the MPL was not distributed with this file, 
 * you can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * This program is distributed in the hope that it will be useful, 
 * WITHOUT WARRANTY OF ANY KIND; either express or implied. See the 
 * Mozilla Public License for more details.
 * 
 * ------------------------------------------------------------------------ */

#ifndef APOC3D_BITARRAY_H
#define APOC3D_BITARRAY_H

#include "../Common.h"

namespace Apoc3D
{
	namespace Collections
	{
		template <uint32 N>
		class BitArray
		{
			static const uint32 ElementBits = 8 * sizeof(uint32);
			static const uint32 ElementBitsDivShift = 5;
			static const uint32 ElementBitsRemainderMask = 0x1f;

			static const uint32 ArrLen = (N + ElementBits - 1) / ElementBits;
		public:
			typedef uint32 InternalArrayType[ArrLen];


			BitArray()
			{
				ZeroArray(m_arr);
			}

			BitArray(const BitArray&) = default;
			BitArray& operator=(const BitArray&) = default;

			void ClearAll() { ZeroArray(m_arr); }

			template <int32 N>
			class AssignmentProxy 
			{
			public:
				AssignmentProxy(BitArray<N>& arr, int32 i)
					: m_arr(arr), m_index(i) { }

				bool operator= (bool v) { m_arr.Set(m_index, v); return v; }

				bool operator==(bool v) const { return m_arr.getBit(m_index) == v; }
				bool operator!=(bool v) const { return m_arr.getBit(m_index) != v; }

				operator bool() const { return m_arr.getBit(m_index); }
				
			private:
				BitArray<N>& m_arr;
				int32 m_index;
			};

			AssignmentProxy<N> operator[](int i) { return { *this, i }; }
			bool operator[](int i) const { return getBit(i); }
			
			void Set(int i, bool v) { if (v) setBit(i); else clearBit(i); }

			const InternalArrayType& getData() const { return m_arr; }
			InternalArrayType& getData() { return m_arr; }

		private:

			void setBit(uint32 i) { assert(i < N); m_arr[bindex(i)] |= 1 << boffset(i); }
			void clearBit(uint32 i) { assert(i < N); m_arr[bindex(i)] &= ~(1 << boffset(i)); }
			bool getBit(uint32 i) const { assert(i < N); return (m_arr[bindex(i)] & (1 << boffset(i))) != 0; }

			static int bindex(uint32 i) { return i >> ElementBitsDivShift; }
			static int boffset(uint32 i) { return i & ElementBitsRemainderMask; }

			InternalArrayType m_arr;
		};
	}
}

#endif