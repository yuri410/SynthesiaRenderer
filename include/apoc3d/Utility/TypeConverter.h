#pragma once

#ifndef APOC3D_ENUMCONVERTERHELPER_H
#define APOC3D_ENUMCONVERTERHELPER_H

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

#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Collections/List.h"
#include "StringUtils.h"

namespace Apoc3D
{
	namespace Utility
	{
		template <typename T>
		class TypeDualConverter
		{
			struct EnumEqualityComparer
			{
				static bool Equals(const T& x, const T& y) { return x == y; }
				static int32 GetHashCode(const T& obj) { return static_cast<int32>(obj); }
			};

		public:
			typedef Apoc3D::Collections::HashMap<String, T, Apoc3D::Collections::StringEqualityComparerNoCase> CastTable;
			typedef Apoc3D::Collections::HashMap<T, String, EnumEqualityComparer> InverseCastTable;
			typedef Apoc3D::Collections::List<String> NameList;
			typedef Apoc3D::Collections::List<T> ValueList;
			
			typedef typename InverseCastTable::Iterator Iterator;
			typedef typename InverseCastTable::ValueAccessor NameAccessor;
			typedef typename CastTable::ValueAccessor ValueAccessor;

			explicit TypeDualConverter(int32 capacity)
				: m_cast(capacity), m_invCast(capacity) { }

			TypeDualConverter(std::initializer_list<std::pair<T, String>> list)
				: m_cast(list.size()), m_invCast(list.size())
			{
				for (const auto& e : list)
					AddPair(e.first, e.second);
			}

			TypeDualConverter(std::initializer_list<std::pair<String, T>> list)
				: m_cast(list.size()), m_invCast(list.size())
			{
				for (const auto& e : list)
					AddPair(e.first, e.second);
			}


			bool SupportsName(const String& name) const { return m_cast.Contains(name); }

			T Parse(const String& name) const { return m_cast[name]; }
			T ParseFlags(const String& combo, const String& delim) const;

			bool TryParse(const String& name, T& r) const { return m_cast.TryGetValue(name, r); }

			const String& ToString(T e) const { return m_invCast[e]; }
			String ToStringFlags(T flags, const String& delim) const;

			bool TryToString(T e, String& r) const { return m_invCast.TryGetValue(e, r); }
			
			T operator[](const String& name) const { return m_cast[name]; }
			const String& operator[](T e) const { return m_invCast[e]; }

			void DumpNames(NameList& names) const { m_invCast.FillValues(names); }
			void DumpValues(ValueList& values) const { m_cast.FillValues(values); }


			NameAccessor getNameAccessor() const { return m_invCast.getValueAccessor(); }
			ValueAccessor getValueAccessor() const { return m_cast.getValueAccessor(); }

			Iterator begin() const { return m_invCast.begin(); }
			Iterator end() const { return m_invCast.end(); }

			int32 getEntryCount() const { return m_cast.getCount(); }

		protected:
			void AddPair(const String& name, T v)
			{
				m_cast.Add(name, v);
				m_invCast.Add(v, name);
			}

			void AddPair(T v, const String& name) { AddPair(name, v); }

		private:
			CastTable m_cast;
			InverseCastTable m_invCast;
		};

		template <typename T>
		class TypeToStringConverter
		{
			struct EnumEqualityComparer
			{
				static bool Equals(const T& x, const T& y) { return x == y; }
				static int32 GetHashCode(const T& obj) { return static_cast<int32>(obj); }
			};

		public:
			typedef Apoc3D::Collections::HashMap<T, String, EnumEqualityComparer> CastTable;
			typedef Apoc3D::Collections::List<String> NameList;
			typedef Apoc3D::Collections::List<T> ValueList;

			typedef typename CastTable::Iterator Iterator;
			typedef typename CastTable::KeyAccessor NameAccessor;
			typedef typename CastTable::ValueAccessor ValueAccessor;
			
			explicit TypeToStringConverter(int32 capacity)
				: m_cast(capacity) { }

			TypeToStringConverter(std::initializer_list<std::pair<T, String>> list)
				: m_cast(list.size())
			{
				for (const auto& e : list)
					m_cast.Add(e.first, e.second);
			}
			TypeToStringConverter(std::initializer_list<std::pair<String, T>> list)
				: m_cast(list.size())
			{
				for (const auto& e : list)
					m_cast.Add(e.first, e.second);
			}

			const String& ToString(T e) const { return m_cast[e]; }
			const String& operator[](T e) const { return m_cast[e]; }
			String ToStringFlags(T flags, const String& delim) const;

			bool TryToString(T e, String& r) const { return m_invCast.TryGetValue(e); }

			void DumpNames(NameList& names) const { m_cast.FillValues(names); }
			void DumpValues(ValueList& values) const { m_cast.FillKeys(values); }

			NameAccessor getNameAccessor() const { return m_cast.getKeyAccessor(); }
			ValueAccessor getValueAccessor() const { return m_cast.getValueAccessor(); }

			Iterator begin() const { return m_cast.begin(); }
			Iterator end() const { return m_cast.end(); }

			int32 getEntryCount() const { return m_cast.getCount(); }
		private:
			CastTable m_cast;
		};

		template <typename T>
		class TypeParseConverter
		{
		public:
			typedef Apoc3D::Collections::HashMap<String, T, Apoc3D::Collections::StringEqualityComparerNoCase> CastTable;
			typedef Apoc3D::Collections::List<T> ValueList;

			typedef typename CastTable::Iterator Iterator;
			typedef typename CastTable::KeyAccessor NameAccessor;
			typedef typename CastTable::ValueAccessor ValueAccessor;

			explicit TypeParseConverter(int32 capacity)
				: m_cast(capacity) { }

			TypeParseConverter(std::initializer_list<std::pair<T, String>> list)
				: m_cast(list.size())
			{
				for (const auto& e : list)
					m_cast.Add(e.second, e.first);
			}

			bool SupportsName(const String& name) const { return m_cast.Contains(name); }

			T Parse(const String& name) const { return m_cast[name]; }
			T ParseFlags(const String& combo, const String& delim) const;
			bool TryParse(const String& name, T& r) const { return m_cast.TryGetValue(name, r); }

			T operator[](const String& name) const { return m_cast[name]; }

			void DumpValues(ValueList& values) const { m_cast.FillKeys(values); }

			ValueAccessor getValueAccessor() const { return m_cast.getValueAccessor(); }

			Iterator begin() const { return m_cast.begin(); }
			Iterator end() const { return m_cast.end(); }

			int32 getEntryCount() const { return m_cast.getCount(); }
		private:
			CastTable m_cast;
		};

		template <typename T, typename H>
		String FlagFieldsToString(T flags, H& helper, const String& delim)
		{
			String result;

			for (auto e : helper)
			{
				T fld = e.Key;
				if ((flags & fld) == fld)
				{
					result.append(e.Value);
					result.append(delim);
				}
			}

			if (result.size() > 1)
				return result.substr(0, result.size() - 2);
			return result;
		}

		template <typename T, typename H, T (H::*parse)(const String&) const>
		T ParseFlagFields(const String& combo, const H* obj, const String& delim) 
		{
			List<String> vals;
			StringUtils::Split(combo, vals, delim);
			std::underlying_type<T>::type result = 0;

			for (const String& e : vals)
			{
				result |= (obj->*parse)(e);
			}

			return static_cast<T>(result);
		}



		template <typename T>
		T TypeDualConverter<T>::ParseFlags(const String& combo, const String& delim) const 
		{
			return ParseFlagFields<T, TypeDualConverter<T>, &TypeDualConverter<T>::Parse>(combo, this, delim);
		}

		template <typename T>
		T TypeParseConverter<T>::ParseFlags(const String& combo, const String& delim) const
		{
			return ParseFlagFields<T, TypeParseConverter<T>, &TypeParseConverter<T>::Parse>(combo, this, delim);
		}

		template <typename T>
		String TypeDualConverter<T>::ToStringFlags(T flags, const String& delim) const { return FlagFieldsToString(flags, *this, delim); }

		template <typename T>
		String TypeToStringConverter<T>::ToStringFlags(T flags, const String& delim) const { return FlagFieldsToString(flags, *this, delim); }

	}
}

#endif