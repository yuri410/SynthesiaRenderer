#pragma once
#ifndef APOC3D_COMPRESSION_H
#define APOC3D_COMPRESSION_H


#include "apoc3d/Common.h"

namespace Apoc3D
{
	namespace Utility
	{
		APAPI int32 rleDecompress(char* dstBuffer, int32 dstBufferSize, const char* srcBuffer, int32 srcBufferSize);
		APAPI int32 rleCompress(char* dstBuffer, int32 dstBufferSize, const char* srcBuffer, int32 srcBufferSize);

		APAPI int32 rleDecompress(char* dstBuffer, int32 dstBufferSize, Apoc3D::IO::BufferedStreamReader* srcStrm);
		APAPI int32 rleCompress(const char* srcBuffer, int32 srcBufferSize, Apoc3D::IO::Stream* dstStrm);

		APAPI int32 rleEvalDecompressedSize(const char* srcBuffer, int32 srcBufferSize);
		APAPI int32 rleEvalCompressedSize(const char* srcBuffer, int32 srcBufferSize);

	}
}

#endif