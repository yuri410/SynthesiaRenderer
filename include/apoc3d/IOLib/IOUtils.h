#pragma once
#ifndef APOC3D_IOUTILS_H
#define APOC3D_IOUTILS_H

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

using namespace Apoc3D;

inline bool isPlatformLittleEndian()
{
	const short int number = 1;
	return (const char&)number == 1;
}


/** Little-endian, used in file reading */
inline uint16 cui16_le(const char* const src);
inline uint32 cui32_le(const char* const src);
inline uint64 cui64_le(const char* const src);
inline int16 ci16_le(const char* const src);
inline int32 ci32_le(const char* const src);	
inline int64 ci64_le(const char* const src);
inline float cr32_le(const char* const src);
inline double cr64_le(const char* const src);

/** Convert from memory, endian dependent */
inline uint16 cui16_dep(const char* const src);
inline uint32 cui32_dep(const char* const src);
inline uint64 cui64_dep(const char* const src);
inline int16 ci16_dep(const char* const src);
inline int32 ci32_dep(const char* const src);	
inline int64 ci64_dep(const char* const src);
inline float cr32_dep(const char* const src);
inline double cr64_dep(const char* const src);

/** Convert to Little-endian multi bytes */
inline void i16tomb_le(int16 v, char* dest);
inline void i32tomb_le(int32 v, char* dest);
inline void i64tomb_le(int64 v, char* dest);
inline void ui16tomb_le(uint16 v, char* dest);
inline void ui32tomb_le(uint32 v, char* dest);
inline void ui64tomb_le(uint64 v, char* dest);
inline void r32tomb_le(float v, char* dest);
inline void r64tomb_le(double v, char* dest);

/** Convert to multi bytes, the result is endian dependent */
inline void i16tomb_dep(int16 v, char* dest);
inline void i32tomb_dep(int32 v, char* dest);
inline void i64tomb_dep(int64 v, char* dest);
inline void ui16tomb_dep(uint16 v, char* dest);
inline void ui32tomb_dep(uint32 v, char* dest);
inline void ui64tomb_dep(uint64 v, char* dest);
inline void r32tomb_dep(float v, char* dest);
inline void r64tomb_dep(double v, char* dest);

inline uint16 cui16_dep(const char* const src) { return *reinterpret_cast<const uint16*>(src); }
inline uint32 cui32_dep(const char* const src) { return *reinterpret_cast<const uint32*>(src); }
inline uint64 cui64_dep(const char* const src) { return *reinterpret_cast<const uint64*>(src); }
inline int16 ci16_dep(const char* const src) { return *reinterpret_cast<const int16*>(src); }
inline int32 ci32_dep(const char* const src) { return *reinterpret_cast<const int32*>(src); }
inline int64 ci64_dep(const char* const src) { return *reinterpret_cast<const int64*>(src); }
inline float cr32_dep(const char* const src)  { return *reinterpret_cast<const float*>(src); }
inline double cr64_dep(const char* const src) { return *reinterpret_cast<const double*>(src); }


inline uint16 cui16_le(const char* const src)
{
#ifndef BIG_ENDIAN
	return *reinterpret_cast<const uint16*>(src);
#else
	const byte* const src2 = reinterpret_cast<const byte*>(src);
	return (src2[1] << 8) | src2[0];
#endif
}
inline uint32 cui32_le(const char* const src)
{
#ifndef BIG_ENDIAN
	return *reinterpret_cast<const uint32*>(src);
#else
	const byte* const src2 = reinterpret_cast<const byte*>(src);
	return (src2[3] << 24) | (src2[2] << 16) | (src2[1] << 8) | src2[0];
#endif
}
inline uint64 cui64_le(const char* const src)
{
#ifndef BIG_ENDIAN
	return *reinterpret_cast<const uint64*>(src);
#else
	const byte* const src2 = reinterpret_cast<const byte*>(src);
	return ((uint64)src2[7] << 56) | ((uint64)src2[6] << 48) | ((uint64)src2[5] << 40) | ((uint64)src2[4] << 32) |
		((uint64)src2[3] << 24) | ((uint64)src2[2] << 16) | ((uint64)src2[1] << 8) | (uint64)src2[0];
#endif
}

inline int16 ci16_le(const char* const src) { return (int16)cui16_le(src); }
inline int32 ci32_le(const char* const src) { return (int32)cui32_le(src); }
inline int64 ci64_le(const char* const src) { return (int64)cui64_le(src); }

inline float cr32_le(const char* const src) 
{
#ifndef BIG_ENDIAN
	return *reinterpret_cast<const float*>(src);
#else
	uint32 r = cui32_le(src);
	return reinterpret_cast<const float&>(r);		
#endif
}
inline double cr64_le(const char* const src) 
{
#ifndef BIG_ENDIAN
	return *reinterpret_cast<const double*>(src); 
#else
	uint64 r = cui64_le(src);
	return reinterpret_cast<const double&>(r);
#endif
}


inline void ui16tomb_le(uint16 v, char* dest)
{
#ifndef BIG_ENDIAN
	reinterpret_cast<uint16&>(dest[0]) = v;
#else
	byte* dest2 = reinterpret_cast<byte*>(dest);

	dest2[0] = 0xff & (v >> 8);
	dest2[1] = 0xff & v;
#endif
}
inline void ui32tomb_le(uint32 v, char* dest)
{
#ifndef BIG_ENDIAN
	reinterpret_cast<uint32&>(dest[0]) = v;
#else
	byte* dest2 = reinterpret_cast<byte*>(dest);

	dest2[0] = 0xff & (v >> 24);
	dest2[1] = 0xff & (v >> 16);
	dest2[2] = 0xff & (v >> 8);
	dest2[3] = 0xff & (v);
#endif
}
inline void ui64tomb_le(uint64 v, char* dest)
{
#ifndef BIG_ENDIAN
	reinterpret_cast<uint64&>(dest[0]) = v;
#else
	byte* dest2 = reinterpret_cast<byte*>(dest);

	dest2[0] = 0xff & (v >> 56);
	dest2[1] = 0xff & (v >> 48);
	dest2[2] = 0xff & (v >> 40);
	dest2[3] = 0xff & (v >> 32);
	dest2[4] = 0xff & (v >> 24);
	dest2[5] = 0xff & (v >> 16);
	dest2[6] = 0xff & (v >> 8);
	dest2[7] = 0xff & (v);
#endif
}

inline void i16tomb_le(int16 v, char* dest) { ui16tomb_le(reinterpret_cast<uint16&>(v), dest); }
inline void i32tomb_le(int32 v, char* dest) { ui32tomb_le(reinterpret_cast<uint32&>(v), dest); }
inline void i64tomb_le(int64 v, char* dest) { ui64tomb_le(reinterpret_cast<uint64&>(v), dest); }

inline void r32tomb_le(float v, char* dest)
{
#ifndef BIG_ENDIAN
	reinterpret_cast<float&>(dest[0]) = v;
#else
	ui32tomb_le(reinterpret_cast<const uint32&>(v), dest);
#endif
}
inline void r64tomb_le(double v, char* dest)
{
#ifndef BIG_ENDIAN
	reinterpret_cast<double&>(dest[0]) = v;
#else
	ui64tomb_le(reinterpret_cast<const uint64&>(v), dest);
#endif
}

inline void i16tomb_dep(int16 v, char* dest) { reinterpret_cast<int16&>(dest[0]) = v; }
inline void i32tomb_dep(int32 v, char* dest) { reinterpret_cast<int32&>(dest[0]) = v; }
inline void i64tomb_dep(int64 v, char* dest) { reinterpret_cast<int64&>(dest[0]) = v; }
inline void ui16tomb_dep(uint16 v, char* dest) { reinterpret_cast<uint16&>(dest[0]) = v; }
inline void ui32tomb_dep(uint32 v, char* dest) { reinterpret_cast<uint32&>(dest[0]) = v; }
inline void ui64tomb_dep(uint64 v, char* dest) { reinterpret_cast<uint64&>(dest[0]) = v; }
inline void r32tomb_dep(float v, char* dest)  { reinterpret_cast<float&>(dest[0]) = v; }
inline void r64tomb_dep(double v, char* dest) { reinterpret_cast<double&>(dest[0]) = v; }

#endif