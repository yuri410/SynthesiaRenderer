#pragma once
#ifndef APOC3D_HASH
#define APOC3D_HASH

#include "apoc3d/Common.h"

namespace Apoc3D
{
	namespace Utility
	{
		class FNVHash32
		{
		public:
			void Accumulate(const void* buffer, int32 size)
			{
				const uint32 p = 16777619;
				const byte* src = (const byte*)buffer;

				while (--size >= 0)
				{
					m_result = (*src++ ^ m_result) * p;
				}
			}

			uint32 getResult() const { return m_result; }

		private:
			uint32 m_result = 2166136261;
		};

		class FNVHash64
		{
		public:
			void Accumulate(const void* buffer, int32 size)
			{
				const uint64 p = 1099511628211;
				const byte* src = (const byte*)buffer;

				while (--size >= 0)
				{
					m_result = (*src++ ^ m_result) * p;
				}
			}

			uint64 getResult() const { return m_result; }
		private:
			uint64 m_result = 14695981039346656037;
		};


		inline int32 MurmurHash(const void * key, int len)
		{
			/**
			 * MurmurHash derived from http://sites.google.com/site/murmurhash/
			 * Code is released to the public domain. 
			 * For business purposes, Murmurhash is under the MIT license.
			 * Developed by Austin Appleby
			 */

			const int32 seed = 2166136261;

			const unsigned int m = 0x5bd1e995;
			const int r = 24;

			unsigned int h = seed ^ len;

			const unsigned char * data = (const unsigned char *)key;

			while(len >= 4)
			{
				unsigned int k = *(unsigned int *)data;

				k *= m; 
				k ^= k >> r; 
				k *= m; 

				h *= m; 
				h ^= k;

				data += 4;
				len -= 4;
			}

			switch(len)
			{
			case 3: h ^= data[2] << 16;
			case 2: h ^= data[1] << 8;
			case 1: h ^= data[0];
				h *= m;
			};

			h ^= h >> 13;
			h *= m;
			h ^= h >> 15;

			return h;
		}
		
		APAPI uint32 CalculateCRC32(const void *buf, int32 size, uint32 crc = 0);

		APAPI uint32 CalculateCRC32(Apoc3D::IO::Stream& strm);
		
	}
}

#endif