#pragma once
#ifndef APOC3D_STRINGUTILS_H
#define APOC3D_STRINGUTILS_H

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
#include "apoc3d/Utility/Hash.h"

namespace Apoc3D
{
	namespace Utility
	{
		namespace StringUtils
		{
			enum FormatFlags : uint64
			{
				SF_Default = 0,
				SF_Left = 1 << 0,

				SF_UpperCase = 1 << 1,

				SF_ShowHexBase = 1 << 2,
				SF_ShowPositiveSign = 1 << 3,

				SF_FPDecimal = 1 << 4,
				SF_FPScientific = 1 << 5,

				SF_FPCustomPrecision = 1 << 6,

				// the default fill char is space. set this to use custom fill char
				SF_CustomFillChar = 1 << 7,


				// masks after first 8 bits

				SF_FPCustomPrecisionMask =	0x00000000000ff000UL,
				SF_WidthMask =				0x00000000fff00000UL,
				SF_FillCharMask =			0x00ffffff00000000UL
			};


			APAPI extern const String Empty;

			APAPI std::string toPlatformNarrowString(const String& str);
			APAPI std::string toPlatformNarrowString(const wchar_t* str);
			APAPI String toPlatformWideString(const std::string& str);
			APAPI String toPlatformWideString(const char* str);
			APAPI std::string toASCIINarrowString(const String& str);
			APAPI String toASCIIWideString(const std::string& str);
			
			APAPI String UTF8toUTF16(const std::string& utf8);
			APAPI std::string UTF16toUTF8(const String& utf16);

			APAPI String32 UTF8toUTF32(const std::string& utf8);
			APAPI std::string UTF32toUTF8(const String32& utf32);

			APAPI String32 UTF16toUTF32(const String& utf16);
			APAPI String UTF32toUTF16(const String32& utf32);

			//////////////////////////////////////////////////////////////////////////
			
			APAPI bool ParseBool(const String& val);
			APAPI uint16 ParseUInt16(const String& val);
			APAPI uint32 ParseUInt32(const String& val);
			APAPI uint64 ParseUInt64(const String& val);

			APAPI uint32 ParseUInt32Hex(const String& val);
			APAPI uint64 ParseUInt64Hex(const String& val);

			APAPI uint32 ParseUInt32Bin(const String& val);
			APAPI uint64 ParseUInt64Bin(const String& val);

			APAPI int16 ParseInt16(const String& val);
			APAPI int32 ParseInt32(const String& val);
			APAPI int64 ParseInt64(const String& val);
			APAPI float ParseSingle(const String& val);
			APAPI double ParseDouble(const String& val);

			//////////////////////////////////////////////////////////////////////////

			APAPI bool ParseBool(const std::string& val);
			APAPI uint16 ParseUInt16(const std::string& val);
			APAPI uint32 ParseUInt32(const std::string& val);
			APAPI uint64 ParseUInt64(const std::string& val);

			APAPI uint32 ParseUInt32Hex(const std::string& val);
			APAPI uint64 ParseUInt64Hex(const std::string& val);

			APAPI uint32 ParseUInt32Bin(const std::string& val);
			APAPI uint64 ParseUInt64Bin(const std::string& val);

			APAPI int16 ParseInt16(const std::string& val);
			APAPI int32 ParseInt32(const std::string& val);
			APAPI int64 ParseInt64(const std::string& val);
			APAPI float ParseSingle(const std::string& val);
			APAPI double ParseDouble(const std::string& val);

			//////////////////////////////////////////////////////////////////////////

			APAPI String IntToString(int16 val, uint64 flags = SF_Default);
			APAPI String IntToString(int32 val, uint64 flags = SF_Default);
			APAPI String IntToString(int64 val, uint64 flags = SF_Default);
			APAPI String SingleToString(float val, uint64 flags = SF_Default);
			APAPI String DoubleToString(double val, uint64 flags = SF_Default);

			APAPI String UIntToString(uint16 val, uint64 flags = SF_Default);
			APAPI String UIntToString(uint32 val, uint64 flags = SF_Default);
			APAPI String UIntToString(uint64 val, uint64 flags = SF_Default);

			APAPI String UIntToStringHex(uint32 val, uint64 flags = SF_Default);
			APAPI String UIntToStringHex(uint64 val, uint64 flags = SF_Default);
			APAPI String UIntToStringBin(uint32 val, uint64 flags = SF_Default);
			APAPI String UIntToStringBin(uint64 val, uint64 flags = SF_Default);

			APAPI String BoolToString(bool val);

			//////////////////////////////////////////////////////////////////////////

			APAPI std::string IntToNarrowString(int16 val, uint64 flags = SF_Default);
			APAPI std::string IntToNarrowString(int32 val, uint64 flags = SF_Default);
			APAPI std::string IntToNarrowString(int64 val, uint64 flags = SF_Default);
			APAPI std::string SingleToNarrowString(float val, uint64 flags = SF_Default);
			APAPI std::string DoubleToNarrowString(double val, uint64 flags = SF_Default);

			APAPI std::string UIntToNarrowString(uint16 val, uint64 flags = SF_Default);
			APAPI std::string UIntToNarrowString(uint32 val, uint64 flags = SF_Default);
			APAPI std::string UIntToNarrowString(uint64 val, uint64 flags = SF_Default);

			APAPI std::string UIntToNarrowStringHex(uint32 val,  uint64 flags = SF_Default);
			APAPI std::string UIntToNarrowStringHex(uint64 val, uint64 flags = SF_Default);
			APAPI std::string UIntToNarrowStringBin(uint32 val, uint64 flags = SF_Default);
			APAPI std::string UIntToNarrowStringBin(uint64 val, uint64 flags = SF_Default);

			APAPI std::string BoolToNarrowString(bool val);

			//////////////////////////////////////////////////////////////////////////

			APAPI bool EqualsNoCase(const String& a, const String& b);
			APAPI bool EqualsNoCase(const std::string& a, const std::string& b);

			APAPI bool Match(const String& str, const String& pattern);
			APAPI bool Match(const std::string& str, const std::string& pattern);

			//////////////////////////////////////////////////////////////////////////

			APAPI void Trim(String& str, const String& delims = L" \t\r");
			APAPI void TrimLeft(String& str, const String& delims = L" \t\r");
			APAPI void TrimRight(String& str, const String& delims = L" \t\r");

			//////////////////////////////////////////////////////////////////////////

			APAPI void Split(const String& str, Apoc3D::Collections::List<String>& results, const String& delims);
			APAPI void Split(const std::string& str, Apoc3D::Collections::List<std::string>& results, const std::string& delims);
			APAPI void Split(const String& str, Apoc3D::Collections::List<String>& results, char16_t delims);
			APAPI void Split(const std::string& str, Apoc3D::Collections::List<std::string>& results, char delims);

			//////////////////////////////////////////////////////////////////////////

			APAPI Apoc3D::Collections::List<String> Split(const String& str, const String& delims);
			APAPI Apoc3D::Collections::List<std::string> Split(const std::string& str, const std::string& delims);
			APAPI Apoc3D::Collections::List<String> Split(const String& str, char16_t delims);
			APAPI Apoc3D::Collections::List<std::string> Split(const std::string& str, char delims);

			template <typename StrType, typename ListType, typename ElementT, typename DelimType = StrType>
			void SplitParse(const StrType& str, ListType& result, const DelimType& delims, FunctorReference<ElementT(const StrType&)> TConverter);

			template <typename StrType, typename ListType, typename ElementT, typename DelimType = StrType>
			ListType SplitParse(const StrType& str, const DelimType& delims, FunctorReference<ElementT(const StrType&)> TConverter);


			template <typename StrType, typename ListType, typename ElementT, ElementT(*TConverter)(const StrType&), typename DelimType = StrType>
			void SplitParse(const StrType& str, ListType& result, const DelimType& delims);

			template <typename StrType, typename ListType, typename ElementT, ElementT(*TConverter)(const StrType&), typename DelimType = StrType>
			ListType SplitParse(const StrType& str, const DelimType& delims);


			template <typename ElementT>
			void SplitParseEnum(const String& str, Apoc3D::Collections::List<ElementT>& result, const String& delims, const TypeDualConverter<ElementT>& parser);

			template <typename ElementT>
			Apoc3D::Collections::List<ElementT> SplitParseEnum(const String& str, const String& delims, const TypeDualConverter<ElementT>& parser);

			//////////////////////////////////////////////////////////////////////////

			APAPI int32 SplitParseSingles(const String& str, float* flts, int32 maxCount, const String& delims);
			APAPI Apoc3D::Collections::List<float> SplitParseSingles(const String& str, const String& delims);
			APAPI void SplitParseSingles(const String& str, Apoc3D::Collections::List<float>& results, const String& delims);
			
			template <int32 N>
			void SplitParseSinglesChecked(const String& str, float (&flts)[N], const String& delims) { SplitParseSinglesChecked(str, flts, N, delims); }
			inline void SplitParseSinglesChecked(const String& str, float* flts, int32 maxCount, const String& delims)
			{
				int32 r = SplitParseSingles(str, flts, maxCount, delims); assert(r == maxCount);
			}

			//////////////////////////////////////////////////////////////////////////

			APAPI int32 SplitParseSingles(const std::string& str, float* flts, int32 maxCount, const std::string& delims);
			APAPI Apoc3D::Collections::List<float> SplitParseSingles(const std::string& str, const std::string& delims);
			APAPI void SplitParseSingles(const std::string& str, Apoc3D::Collections::List<float>& results, const std::string& delims);
			
			template <int32 N>
			void SplitParseSinglesChecked(const std::string& str, float (&flts)[N], const std::string& delims) { SplitParseSinglesChecked(str, flts, N, delims); }
			inline void SplitParseSinglesChecked(const std::string& str, float* flts, int32 maxCount, const std::string& delims)
			{
				int32 r = SplitParseSingles(str, flts, maxCount, delims); assert(r == maxCount);
			}

			//////////////////////////////////////////////////////////////////////////

			APAPI int32 SplitParseInts(const String& str, int32* ints, int32 maxCount, const String& delims);
			APAPI Apoc3D::Collections::List<int32> SplitParseInts(const String& str, const String& delims);
			APAPI void SplitParseInts(const String& str, Apoc3D::Collections::List<int32>& results, const String& delims);
			
			template <int32 N>
			void SplitParseIntsChecked(const String& str, int32 (&ints)[N], const String& delims) { SplitParseIntsChecked(str, ints, N, delims); }
			inline void SplitParseIntsChecked(const String& str, int32* ints, int32 maxCount, const String& delims)
			{
				int32 r = SplitParseInts(str, ints, maxCount, delims); assert(r == maxCount);
			}

			//////////////////////////////////////////////////////////////////////////

			APAPI int32 SplitParseInts(const std::string& str, int32* ints, int32 maxCount, const std::string& delims);
			APAPI Apoc3D::Collections::List<int32> SplitParseInts(const std::string& str, const std::string& delims);
			APAPI void SplitParseInts(const std::string& str, Apoc3D::Collections::List<int32>& results, const std::string& delims);
			
			template <int32 N>
			void SplitParseIntsChecked(const std::string& str, int32 (&ints)[N], const std::string& delims) { SplitParseIntsChecked(str, ints, N, delims); }
			inline void SplitParseIntsChecked(const std::string& str, int32* ints, int32 maxCount, const std::string& delims)
			{
				int32 r = SplitParseInts(str, ints, maxCount, delims); assert(r == maxCount);
			}
			
			template <typename ElementT, String(*ToStringConverter)(const ElementT&), bool UseQuotes = false >
			void Pack(const ElementT* v, int count, String& result, wchar_t sep = ',')
			{
				for (int i = 0; i < count; i++)
				{
					String p = ToStringConverter(v[i]);

					if (UseQuotes && p.find(',') != String::npos)
					{
						result.append(1, '"');
						result += p;
						result.append(1, '"');
					}
					else
					{
						result += p;
					}

					if (i != count - 1)
					{
						result.append(1, sep);
						result.append(1, ' ');
					}
				}
			}

			template <typename ElementT, String(*ToStringConverter)(const ElementT&), bool UseQuotes = false >
			String Pack(const ElementT* v, int count, wchar_t sep = ',')
			{
				String r; Pack<ElementT, ToStringConverter, UseQuotes>(v, count, r, sep); return r;
			}

			template <typename ElementT, String(*ToStringConverter)(const ElementT&), bool UseQuotes = false >
			void Pack(const Apoc3D::Collections::List<ElementT>& v, String& result, wchar_t sep = ',')
			{
				Pack<ElementT, ToStringConverter, UseQuotes>(v.getElements(), v.getCount(), result, sep);
			}

			template <typename ElementT, String(*ToStringConverter)(const ElementT&), bool UseQuotes = false >
			String Pack(const Apoc3D::Collections::List<ElementT>& v, wchar_t sep = ',')
			{
				String r; Pack<ElementT, ToStringConverter, UseQuotes>(v, r, sep); return r;
			}

			APAPI String PackStrings(const Apoc3D::Collections::List<String>& v, bool useQuotes, wchar_t sep = ',');

			//////////////////////////////////////////////////////////////////////////

			APAPI bool Replace(String& str, const String& from, const String& to);
			APAPI void ReplaceAll(String& str, const String& from, const String& to);

			//////////////////////////////////////////////////////////////////////////

			APAPI bool StartsWith(const String& str, const String& v, bool caseInsensitive = false);
			APAPI bool EndsWith(const String& str, const String& v, bool caseInsensitive = false);
			APAPI bool StartsWith(const std::string& str, const std::string& v, bool caseInsensitive = false);
			APAPI bool EndsWith(const std::string& str, const std::string& v, bool caseInsensitive = false);

			APAPI void ToLowerCase(String& str);
			APAPI void ToUpperCase(String& str);
			APAPI void ToLowerCase(std::string& str);
			APAPI void ToUpperCase(std::string& str);

			APAPI char ToLowerCase(char ch);
			APAPI char ToUpperCase(char str);
			APAPI wchar_t ToLowerCase(wchar_t ch);
			APAPI wchar_t ToUpperCase(wchar_t str);

			inline uint GetHashCode(const String& str)
			{
				return MurmurHash(str.c_str(), sizeof(wchar_t)*str.length());
			}
		};

		namespace StrFmt
		{
			inline uint64 FP(byte precision) { return (((uint32)precision) << 12) | StringUtils::SF_FPCustomPrecision; }
			inline uint64 FPDecimal(byte precision) { return FP(precision) | StringUtils::SF_FPDecimal; }

			inline uint64 A(uint16 width, char32_t fill) { return (((uint64)width) << 20) | (((uint64)fill) << 32) | StringUtils::SF_CustomFillChar; }
			inline uint64 A(uint16 width) { return ((uint64)width) << 20; }

			inline uint64 FPA(uint16 width, char32_t fill, byte precision) { return A(width, fill) | FP(precision); }
			inline uint64 FPADecimal(uint16 width, char32_t fill, byte precision) { return A(width, fill) | FPDecimal(precision); }

			template <byte precision>
			struct fp
			{
				static const uint64 val = (((uint32)precision) << 12) | StringUtils::SF_FPCustomPrecision;
			};
			
			template <byte precision>
			struct fpdec
			{
				static const uint64 val = fp<precision>::val | StringUtils::SF_FPDecimal;
			};
			
			template <uint16 width, char32_t fill = ' '>
			struct a
			{
				static const uint64 val = (((uint64)width) << 20) | (fill != ' ' ? ((((uint64)fill) << 32) | StringUtils::SF_CustomFillChar) : 0);
			};

			template <uint16 width, char32_t fill, byte precision>
			struct fpa
			{
				static const uint64 val = a<width, fill>::val | fp<precision>::val;
			};

			template <uint16 width, char32_t fill, byte precision>
			struct fpadec
			{
				static const uint64 val = a<width, fill>::val | fpdec<precision>::val;
			};
		}

		namespace StringUtilsLocalized
		{
			APAPI uint16 ParseUInt16(const String& val);
			APAPI uint32 ParseUInt32(const String& val);
			APAPI uint64 ParseUInt64(const String& val);
			APAPI int16 ParseInt16(const String& val);
			APAPI int32 ParseInt32(const String& val);
			APAPI int64 ParseInt64(const String& val);
			APAPI float ParseSingle(const String& val);
			APAPI double ParseDouble(const String& val);

			APAPI uint16 ParseUInt16(const std::string& val);
			APAPI uint32 ParseUInt32(const std::string& val);
			APAPI uint64 ParseUInt64(const std::string& val);
			APAPI int16 ParseInt16(const std::string& val);
			APAPI int32 ParseInt32(const std::string& val);
			APAPI int64 ParseInt64(const std::string& val);
			APAPI float ParseSingle(const std::string& val);
			APAPI double ParseDouble(const std::string& val);


			APAPI String IntToString(int16 val, uint16 width = 0, char16_t fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI String IntToString(int32 val, uint16 width = 0, char16_t fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI String IntToString(int64 val, uint16 width = 0, char16_t fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI String SingleToString(float val, uint16 precision = 6, uint16 width = 0, char16_t fill = ' ',
				std::ios::fmtflags flags = std::ios::fmtflags(std::ios::fixed));

			APAPI String UIntToString(uint16 val, uint16 width = 0, char16_t fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI String UIntToString(uint32 val, uint16 width = 0, char16_t fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI String UIntToString(uint64 val, uint16 width = 0, char16_t fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));


			APAPI std::string IntToNarrowString(int16 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI std::string IntToNarrowString(int32 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI std::string IntToNarrowString(int64 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI std::string SingleToNarrowString(float val, uint16 precision = 6, uint16 width = 0, char fill = ' ',
				std::ios::fmtflags flags = std::ios::fmtflags(std::ios::fixed));

			APAPI std::string UIntToNarrowString(uint16 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI std::string UIntToNarrowString(uint32 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
			APAPI std::string UIntToNarrowString(uint64 val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));


			APAPI void ToLowerCase(String& str);
			APAPI void ToUpperCase(String& str);

		}


		template <
			typename StrType, 
			typename ListType, 
			typename ElementT,
			typename DelimType
		>
		void StringUtils::SplitParse(const StrType& str, ListType& result, const DelimType& delims, FunctorReference<ElementT(const StrType&)> TConverter)
		{
			assert(result.getCount() == 0);

			// Use STL methods 
			size_t start, pos;
			start = 0;
			do
			{
				pos = str.find_first_of(delims, start);
				if (pos == start)
				{
					// Do nothing
					start = pos + 1;
				}
				else if (pos == StrType::npos)
				{
					// Copy the rest of the string
					if (start < str.size())
						result.Add(TConverter(str.substr(start)));
					break;
				}
				else
				{
					// Copy up to delimiter
					result.Add(TConverter(str.substr(start, pos - start)));
					start = pos + 1;
				}
				// parse up to next real data
				start = str.find_first_not_of(delims, start);

			} while (pos != StrType::npos);
		}

		template <typename StrType, typename ListType, typename ElementT, typename DelimType>
		ListType StringUtils::SplitParse(const StrType& str, const DelimType& delims, FunctorReference<ElementT(const StrType&)> TConverter)
		{
			ListType lst;
			SplitParse<StrType, ListType, ElementT, DelimType>(str, lst, delims, TConverter);
			return lst;
		}


		template <
			typename StrType, 
			typename ListType, 
			typename ElementT, 
			ElementT(*TConverter)(const StrType&), 
			typename DelimType
		>
		void StringUtils::SplitParse(const StrType& str, ListType& result, const DelimType& delims)
		{
			SplitParse<StrType, ListType, ElementT, DelimType>(str, result, delims, TConverter);
		}

		template <
			typename StrType, 
			typename ListType, 
			typename ElementT, 
			ElementT(*TConverter)(const StrType&), 
			typename DelimType
		>
		ListType StringUtils::SplitParse(const StrType& str, const DelimType& delims)
		{
			ListType lst;
			SplitParse<StrType, ListType, ElementT, DelimType>(str, lst, delims, TConverter);
			return lst;
		}



		template <typename ElementT>
		void StringUtils::SplitParseEnum(const String& str, Apoc3D::Collections::List<ElementT>& result, const String& delims, const TypeDualConverter<ElementT>& parser)
		{
			auto parserFunc = [&parser](const String& s) { return parser.Parse(s); };
			SplitParse<String, Apoc3D::Collections::List<ElementT>, ElementT, String>(str, result, delims, parserFunc);
		}

		template <typename ElementT>
		Apoc3D::Collections::List<ElementT> StringUtils::SplitParseEnum(const String& str, const String& delims, const TypeDualConverter<ElementT>& parser)
		{
			Apoc3D::Collections::List<ElementT> lst;
			SplitParseEnum<ElementT>(str, lst, delims, parser);
			return lst;
		}

	}
}


#endif