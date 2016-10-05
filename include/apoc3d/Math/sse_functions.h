#pragma once
#ifndef APOC3D_SSE_FUNCTIONS_H
#define APOC3D_SSE_FUNCTIONS_H

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

#include "../Common.h"

#define ZERO_VECTOR

#define VEC_INDEX_X 0
#define VEC_INDEX_Y 1
#define VEC_INDEX_Z 2
#define VEC_INDEX_W 3

#define VEC_ADDR(x) (x*4)
#define VEC_ADDR_X VEC_ADDR(0)
#define VEC_ADDR_Y VEC_ADDR(1)
#define VEC_ADDR_Z VEC_ADDR(2)

#include <xmmintrin.h>

#define _MM_SHUFFLE1(x) _MM_SHUFFLE(x,x,x,x)

const int SIMDDot4MaskX = 0xF1; // all components involved to dot, output to X
const int SIMDDot3MaskX = 0x71; // all components involved to dot, output to X

/*#pragma pack(push ,16)
struct __declspec(align(16)) SSEVecLoader
{
	float X;
	float Y;
	float Z;
	float W;
};
#pragma pack(pop)


inline __m128 SIMDVecLoad(const SSEVecLoader& vec) { return _mm_load_ps(reinterpret_cast<const float*>(&vec)); };*/
static __m128 _MASKSIGN_;

inline __m128 SIMDVecLoad(float f)
{
	return _mm_set1_ps(f);
};
inline __m128 SIMDVecLoad(float x, float y, float z)
{
	const float vec[4] = {x,y,z,0};
	return _mm_load_ps(vec);
};
inline __m128 SIMDVecLoad(float x, float y, float z, float w)
{
	const float vec[4] = {x,y,z,w};
	return _mm_load_ps(vec);
};
/* Adds two vectors.
*/
inline __m128 SIMDVecAdd(__m128 va, __m128 vb) { return _mm_add_ps(va, vb); };

/* Subtracts two vectors.
*/
inline __m128 SIMDVecSub(__m128 va, __m128 vb) { return _mm_sub_ps(va, vb); };

/* Modulates a vector by another.
*/
inline __m128 SIMDVecMul(__m128 va, __m128 vb) { return _mm_mul_ps(va, vb); };

/* Scales a vector by the given value.
*/
inline __m128 SIMDVecMul(__m128 va, float vb)
{
	__m128 scale = _mm_set1_ps(vb);
	return _mm_mul_ps(va, scale);			
}


inline __m128 SIMDVecDiv(__m128 va, __m128 vb) { return _mm_div_ps(va, vb); };
		
inline __m128 SIMDVecDiv(__m128 va, float vb)
{
	__m128 dd = _mm_set1_ps(vb);
	return _mm_div_ps(va, dd);
};

inline __m128 SIMDVecStore(float* pVec, __m128 v)
{
	//SSEVecLoader buffer;
	//_mm_store_ps(reinterpret_cast<float*>(&buffer), v);
	//pVec[0] = buffer.X; pVec[1] = buffer.Y; pVec[2] = buffer.Z; pVec[3] = buffer.W;
	_mm_store_ps(pVec, v);
};

/* Reverses the direction of a given vector.
*/
inline __m128 SIMDVecNegate(__m128 va)
{
	return _mm_xor_ps(_MASKSIGN_, va);
}


/*	NewtonRaphson Reciprocal Square Root
0.5 * rsqrtps * (3 - x * rsqrtps(x) * rsqrtps(x)) */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4640)
#endif
inline __m128 rsqrt_nr(__m128 a)
{
	static const __m128 fvecf0pt5 = SIMDVecLoad(0.5f);
	static const __m128 fvecf3pt0 = SIMDVecLoad(3.0f);
	__m128 Ra0 = _mm_rsqrt_ps(a);

	__m128 l = _mm_mul_ps(fvecf0pt5 , Ra0);
			
	__m128 r = _mm_mul_ps(a , Ra0);
	r = _mm_mul_ps(r , Ra0);

	r = _mm_sub_ps(fvecf3pt0, r);

	return _mm_sub_ps(l,r);// (fvecf0pt5 * Ra0) * (fvecf3pt0 - (a * Ra0) * Ra0);
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

		
/* Returns a vector containing the smallest components of the specified vectors.
*/
inline __m128 SIMDVecMin(__m128 va, __m128 vb) { return _mm_min_ps(va, vb); };

/* Returns a vector containing the largest components of the specified vectors.
*/
inline __m128 SIMDVecMax(__m128 va, __m128 vb) { return _mm_max_ps(va, vb); };


/* Element Count Dependent */


/* Calculates the cross product of two vectors.
*/
inline __m128 SIMDVec3Cross(__m128 va, __m128 vb)
{
	__m128 l1, l2, m1, m2;
	l1 = _mm_shuffle_ps(va,va, _MM_SHUFFLE(VEC_INDEX_Y,VEC_INDEX_Z,VEC_INDEX_X,VEC_INDEX_W));
	l2 = _mm_shuffle_ps(vb,vb, _MM_SHUFFLE(VEC_INDEX_Z,VEC_INDEX_X,VEC_INDEX_Y,VEC_INDEX_W));
	m2 = _mm_mul_ps(l1,l2);
	l1 = _mm_shuffle_ps(va,va, _MM_SHUFFLE(VEC_INDEX_Z,VEC_INDEX_X,VEC_INDEX_Y,VEC_INDEX_W));
	l2 = _mm_shuffle_ps(vb,vb, _MM_SHUFFLE(VEC_INDEX_Y,VEC_INDEX_Z,VEC_INDEX_X,VEC_INDEX_W));
	m1 = _mm_mul_ps(l1,l2);
	return _mm_sub_ps( m1,m2);
}

/*  Calculates the dot product of two vectors.
*/
inline float SIMDVec3Dot(__m128 va, __m128 vb)
{
	__m128 dot = _mm_dp_ps(va, vb, SIMDDot3MaskX);
	return reinterpret_cast<const float&>(dot);
};
inline float SIMDVec4Dot(__m128 va, __m128 vb)
{
	__m128 dot = _mm_dp_ps(va, vb, SIMDDot4MaskX);
	return reinterpret_cast<const float&>(dot);
};
		
/*  Calculates the dot product of two vectors.
*/
inline __m128 SIMDVec3Dot2(__m128 va, __m128 vb)
{
	return _mm_dp_ps(va, vb, SIMDDot3MaskX);
};
inline __m128 SIMDVec4Dot2(__m128 va, __m128 vb)
{
	return _mm_dp_ps(va, vb, SIMDDot4MaskX);
};
		
/* Calculates the length of a specified vector.
*/
inline float SIMDVec3Length(__m128 va)
{			
	__m128 dot = SIMDVec3Dot2(va,va);

	dot = _mm_sqrt_ps(dot);
			
	return reinterpret_cast<const float&>(dot);
};
inline float SIMDVec4Length(__m128 va)
{			
	__m128 dot = SIMDVec4Dot2(va,va);

	dot = _mm_sqrt_ps(dot);
	return reinterpret_cast<const float&>(dot);
};

/* Calculates the squared length of a specified vector.
*/
inline float SIMDVec3LengthSquared(__m128 va)
{
	return SIMDVec3Dot(va, va);
};
inline float SIMDVec4LengthSquared(__m128 va)
{
	return SIMDVec4Dot(va, va);
};

/* Calculates the distance between two vectors.
*/
inline float SIMDVec3Distance(__m128 va, __m128 vb)
{
	__m128 d = _mm_sub_ps(va, vb);
	return SIMDVec3Length(d);
}
inline float SIMDVec4Distance(__m128 va, __m128 vb)
{
	__m128 d = _mm_sub_ps(va, vb);
	return SIMDVec4Length(d);
}

/* Calculates the squared distance between two vectors.
*/
inline float SIMDVec3DistanceSquared(__m128 va, __m128 vb)
{
	__m128 d = _mm_sub_ps(va, vb);
	return SIMDVec3LengthSquared(d);
}
inline float SIMDVec4DistanceSquared(__m128 va, __m128 vb)
{
	__m128 d = _mm_sub_ps(va, vb);
	return SIMDVec4LengthSquared(d);
}

/* Converts a specified vector into a unit vector.
*/
inline __m128 SIMDVec3Normalize(__m128 va)
{
	__m128 t = SIMDVec3Dot2(va, va);
#ifdef ZERO_VECTOR

	static const __m128 vecZero = _mm_setzero_ps();
	t = _mm_and_ps(_mm_cmpneq_ss(_mm_shuffle_ps( t, t, VEC_INDEX_X), vecZero), rsqrt_nr(t));
#else
	t = rsqrt_nr(t);
#endif
	return _mm_mul_ps(va, _mm_shuffle_ps(t,t,0x00));
};
inline __m128 SIMDVec4Normalize(__m128 va)
{
	__m128 t = SIMDVec4Dot2(va, va);
#ifdef ZERO_VECTOR

	static const __m128 vecZero = _mm_setzero_ps();
	t = _mm_and_ps(_mm_cmpneq_ss(t, vecZero), rsqrt_nr(t));
#else
	t = rsqrt_nr(t);
#endif
	return _mm_mul_ps(va, _mm_shuffle_ps(t,t,0x00));
};




/* Returns the reflection of a vector off a surface that has the specified normal. 
*/
inline __m128 SIMDVec3Reflect(__m128 Incident, __m128 Normal)
{
	// Result = Incident - (2 * dot(Incident, Normal)) * Normal
	__m128 Result = SIMDVec3Dot2(Incident,Normal);
	Result = _mm_add_ps(Result,Result);
	Result = _mm_mul_ps(Result,Normal);
	Result = _mm_sub_ps(Incident,Result);
	return Result;
};
inline __m128 SIMDVec4Reflect(__m128 Incident, __m128 Normal)
{
	// Result = Incident - (2 * dot(Incident, Normal)) * Normal
	__m128 Result = SIMDVec4Dot2(Incident,Normal);
	Result = _mm_add_ps(Result,Result);
	Result = _mm_mul_ps(Result,Normal);
	Result = _mm_sub_ps(Incident,Result);
	return Result;
};

inline bool SIMDVec3LessEqual(__m128 a, __m128 b)
{
	__m128 result = _mm_cmple_ss(a,b);
	const bool* bc = reinterpret_cast<const bool*>(&result);
	return bc[VEC_ADDR_X] & bc[VEC_ADDR_Y] & bc[VEC_ADDR_Z];
};
inline bool SIMDVec3Less(__m128 a, __m128 b)
{
	__m128 result = _mm_cmplt_ss(a,b);
	const bool* bc = reinterpret_cast<const bool*>(&result);
		return bc[VEC_ADDR_X] & bc[VEC_ADDR_Y] & bc[VEC_ADDR_Z];
};
inline bool SIMDVec3Greater(__m128 a, __m128 b)
{
	__m128 result = _mm_cmpgt_ss(a,b);
	const bool* bc = reinterpret_cast<const bool*>(&result);
		return bc[VEC_ADDR_X] & bc[VEC_ADDR_Y] & bc[VEC_ADDR_Z];
};
inline bool SIMDVec3GreaterEqual(__m128 a, __m128 b)
{
	__m128 result = _mm_cmpge_ss(a,b);
	const bool* bc = reinterpret_cast<const bool*>(&result);
		return bc[VEC_ADDR_X] & bc[VEC_ADDR_Y] & bc[VEC_ADDR_Z];
};

#endif
	