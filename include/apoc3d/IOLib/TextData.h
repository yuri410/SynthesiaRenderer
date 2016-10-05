#pragma once
#ifndef APOC3D_TEXTDATA_H
#define APOC3D_TEXTDATA_H

#include "apoc3d/Common.h"

namespace Apoc3D
{
	namespace IO
	{
		namespace Encoding
		{
			enum TextEncoding
			{
				TEC_Unknown,
				TEC_UTF8,
				TEC_UTF16LE,
				TEC_UTF16BE,
				TEC_UTF32LE,
				TEC_UTF32BE,
				TEC_ASCII
			};

			APAPI TextEncoding FindEncodingByBOM(const char* bomData, int32 length, int32* bomLength = nullptr);
			APAPI TextEncoding FindEncodingByBOM(const char* bomData, int32 length, TextEncoding defaultEnc, int32* bomLength = nullptr);


			APAPI const char* GetEncodingBOM(TextEncoding enc, int32& length);
			
			APAPI String ConvertFromRawData(const char* rawData, int32 length, TextEncoding encoding, bool checkBom);
			APAPI void ConvertToRawData(const String& text, TextEncoding encoding, bool includeBom, char*& dest, int32& destLength);
			APAPI void ConvertToRawData(const String& text, TextEncoding encoding, bool includeBom, Stream& dest);


			APAPI String ReadAllText(const VFS::ResourceLocation& rl, TextEncoding encoding);
			APAPI void WriteAllText(Stream& strm, const String& text, TextEncoding encoding);
		}


	}
}

#endif