#pragma once
#ifndef APOC3D_CONFIGURATIONSECTION_H
#define APOC3D_CONFIGURATIONSECTION_H

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

#include "apoc3d/Math/ColorValue.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Core/Pool.h"
#include "apoc3d/Utility/TypeConverter.h"

using namespace Apoc3D::Math;
using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;

namespace Apoc3D
{
	namespace Config
	{
		/** 
		 * Represents a section in a configuration.
		 *
		 * @remarks
		 *  As tree node in configuration, each section can have a name, a value
		 *  and several attributes. Particular value parsing functions are also provided.
		 *  The parsing will convert a string representation to the target data type.
		 *
		 *  The ConfigurationSection can get value in a key-value way, beside attributes,
		 *  by treating all the sub-sections as value with the name, which is the key.
		 *
		 *  Ideally, if the sub section does not have any children, its config file
		 *  representation should be well understandable. For instance, in a xml file:
		 *  <Section>
		 *    <SubSect1>Value1</SubSect1>
		 *    <Key2>Value2</Key2>
		 *  </Section>
		 *  At the "Section" level, SubSect1 and Key2 and their value(like Value1, Value2)
		 *  can be obtained by providing their name as the key, by using getXXX method.
		 */
		class APAPI ConfigurationSection
		{
		public:
			DECL_POOL_TYPE(ConfigurationSection);

			static int32 FloatPointStoringPrecision;
			static bool FloatPointCustomStoringPrecision;

			typedef HashMap<String, String> AttributeTable;
			typedef HashMap<String, ConfigurationSection*> SubSectionTable;

			class AttributeAccessor
			{
			public:
				AttributeTable::Iterator begin() const { return m_attTable.begin(); }
				AttributeTable::Iterator end() const { return m_attTable.end(); }

				AttributeAccessor(const AttributeTable& tbl)
					: m_attTable(tbl) { }

			private:
				const AttributeTable& m_attTable;
			};

			ConfigurationSection(const String& name);
			ConfigurationSection(const String& name, int capacity);
			ConfigurationSection(const String& name, const String& value);
			ConfigurationSection(const String& name, std::initializer_list<std::pair<String, String>> attribs, std::initializer_list<ConfigurationSection> values);
			ConfigurationSection(const ConfigurationSection&);
			ConfigurationSection(ConfigurationSection&&);
			~ConfigurationSection();

			ConfigurationSection& operator=(const ConfigurationSection&);
			ConfigurationSection& operator=(ConfigurationSection&&);

			ConfigurationSection* CreateSubSection(const String& name);
			void AddSection(ConfigurationSection* section);
			void SetValue(const String& value);

			AttributeAccessor getAttributes() const { return AttributeAccessor(m_attributes); }
			SubSectionTable::ValueAccessor getSubSections() const { return m_subSection.getValueAccessor(); }

			int getAttributeCount() const { return m_attributes.getCount(); }
			int getSubSectionCount() const { return m_subSection.getCount(); }

			const String& getName() const { return m_name; }
			const String& getValue() const { return m_value; }
			const String& getValue(const String& name) const;			/** Gets the value of the sub-section with the given name. */
			const String& getAttribute(const String& name) const;
			ConfigurationSection* getSection(const String& name) const;

			bool hasAttribute(const String& name) const;

			bool tryGetValue(const String& name, String& result) const;
			bool tryGetAttribute(const String& name, String& result) const;

			const String* tryGetValue(const String& name) const;
			const String* tryGetAttribute(const String& name) const;


#define CONFIG_SECT_GETER_DECL(type, typeName) \
			type Get##typeName() const; \
			type Get##typeName(const String& key) const; \
			type GetAttribute##typeName(const String& key) const; \
			bool TryGet##typeName(const String& key, type& result) const; \
			bool TryGetAttribute##typeName(const String& key, type& result) const;

			CONFIG_SECT_GETER_DECL(bool, Bool);
			CONFIG_SECT_GETER_DECL(float, Single);
			CONFIG_SECT_GETER_DECL(float, Percentage);
			CONFIG_SECT_GETER_DECL(int32, Int);
			CONFIG_SECT_GETER_DECL(uint32, UInt);
			CONFIG_SECT_GETER_DECL(ColorValue, ColorValue);
			CONFIG_SECT_GETER_DECL(Vector3, Vector3);
			CONFIG_SECT_GETER_DECL(Point, Point);

#undef CONFIG_SECT_GETER_DECL

#define CONFIG_SECT_SPLITER_DECL(type, typeName) \
			type Get##typeName() const; \
			type Get##typeName(const String& key) const; \
			type GetAttribute##typeName(const String& key) const; \
			void Get##typeName(type& result) const; \
			void Get##typeName(const String& key, type& result) const; \
			void GetAttribute##typeName(const String& key, type& result) const; \
			bool TryGet##typeName(const String& key, type& result) const; \
			bool TryGetAttribute##typeName(const String& key, type& result) const;

			CONFIG_SECT_SPLITER_DECL(List<String>, Strings);
			CONFIG_SECT_SPLITER_DECL(List<float>, Singles);
			CONFIG_SECT_SPLITER_DECL(List<float>, Percentages);
			CONFIG_SECT_SPLITER_DECL(List<int32>, Ints);
			CONFIG_SECT_SPLITER_DECL(List<uint32>, UInts);
			CONFIG_SECT_SPLITER_DECL(List<Vector3>, Vector3s);
			CONFIG_SECT_SPLITER_DECL(List<Point>, Points);

#undef CONFIG_SECT_SPLITER_DECL


#define CONFIG_SECT_SPLITER_ARR_DECL(type, typeName) \
			void Get##typeName(type* v, int32 maxCount, int32* acutallCount = nullptr) const; \
			void Get##typeName(const String& key, type* v, int32 maxCount, int32* acutallCount = nullptr) const; \
			void GetAttribute##typeName(const String& key, type* v, int32 maxCount, int32* acutallCount = nullptr) const; \
			bool TryGet##typeName(const String& key, type* v, int32 maxCount, int32* acutallCount = nullptr) const; \
			bool TryGetAttribute##typeName(const String& key, type* v, int32 maxCount, int32* acutallCount = nullptr) const; \
			void Get##typeName##Checked(type* v, int32 expectedCount) const { int32 actuallCount; Get##typeName(v, expectedCount, &actuallCount); assert(actuallCount == expectedCount); } \
			void Get##typeName##Checked(const String& key, type* v, int32 expectedCount) const { int32 actuallCount; Get##typeName(key, v, expectedCount, &actuallCount); assert(actuallCount == expectedCount); } \
			void GetAttribute##typeName##Checked(const String& key, type* v, int32 expectedCount) const { int32 actuallCount; GetAttribute##typeName(key, v, expectedCount, &actuallCount); assert(actuallCount == expectedCount); } \
			bool TryGet##typeName##Checked(const String& key, type* v, int32 expectedCount) const { int32 actuallCount; bool r = TryGet##typeName(key, v, expectedCount, &actuallCount); if (r) assert(actuallCount == expectedCount); return r; } \
			bool TryGetAttribute##typeName##Checked(const String& key, type* v, int32 expectedCount) const { int32 actuallCount; bool r = TryGetAttribute##typeName(key, v, expectedCount, &actuallCount); if (r) assert(actuallCount == expectedCount); return r; } \
			template <int32 N> void Get##typeName##Checked(type (&v)[N]) const { Get##typeName##Checked(v, N); } \
			template <int32 N> void Get##typeName##Checked(const String& key, type (&v)[N]) const { Get##typeName##Checked(key, v, N); } \
			template <int32 N> void GetAttribute##typeName##Checked(const String& key, type (&v)[N]) const { GetAttribute##typeName##Checked(key, v, N); }  \
			template <int32 N> bool TryGet##typeName##Checked(const String& key, type (&v)[N]) const { return TryGet##typeName##Checked(key, v, N); } \
			template <int32 N> bool TryGetAttribute##typeName##Checked(const String& key, type (&v)[N]) const { return TryGetAttribute##typeName##Checked(key, v, N); } 

			CONFIG_SECT_SPLITER_ARR_DECL(float, Singles);
			CONFIG_SECT_SPLITER_ARR_DECL(float, Percentages);
			CONFIG_SECT_SPLITER_ARR_DECL(int32, Ints);
			CONFIG_SECT_SPLITER_ARR_DECL(uint32, UInts);
			CONFIG_SECT_SPLITER_ARR_DECL(Vector3, Vector3s);
			CONFIG_SECT_SPLITER_ARR_DECL(Point, Points);

#undef CONFIG_SECT_SPLITER_ARR_DECL


			void AddStringValue(const String& name, const String& value);
			void AddAttributeString(const String& name, const String& value);


#define CONFIG_SECT_ADDER_DECL(type, typeName) \
			void Add##typeName(const String& key, type value); \
			void AddAttribute##typeName(const String& key, type value); 

			CONFIG_SECT_ADDER_DECL(bool, Bool);
			CONFIG_SECT_ADDER_DECL(float, Single);
			CONFIG_SECT_ADDER_DECL(float, Percentage);
			CONFIG_SECT_ADDER_DECL(int32, Int);
			CONFIG_SECT_ADDER_DECL(uint32, UInt);
			CONFIG_SECT_ADDER_DECL(ColorValue, ColorValue);
			CONFIG_SECT_ADDER_DECL(const Vector3&, Vector3);
			CONFIG_SECT_ADDER_DECL(const Point&, Point);

#undef CONFIG_SECT_ADDER_DECL


#define CONFIG_SECT_COMBINER_DECL_NOLIST(type, typeName) \
			void Add##typeName(const String& key, const type* v, int32 count); \
			void AddAttribute##typeName(const String& name, const type* v, int32 count); \
			template <int32 N> void Add##typeName(const String& key, const type (&v)[N]) { Add##typeName(key, v, N); } \
			template <int32 N> void AddAttribute##typeName(const String& name, const type (&v)[N]) { AddAttribute##typeName(name, v, N); }

#define CONFIG_SECT_COMBINER_DECL(type, typeName) \
			CONFIG_SECT_COMBINER_DECL_NOLIST(type, typeName); \
			void Add##typeName(const String& key, const List<type>& v) { Add##typeName(key, v.getElements(), v.getCount()); } \
			void AddAttribute##typeName(const String& name, const List<type>& v) { AddAttribute##typeName(name, v.getElements(), v.getCount()); }

			CONFIG_SECT_COMBINER_DECL(String, Strings);
			CONFIG_SECT_COMBINER_DECL(float, Singles);
			CONFIG_SECT_COMBINER_DECL(float, Percentages);
			CONFIG_SECT_COMBINER_DECL(int32, Ints);
			CONFIG_SECT_COMBINER_DECL(uint32, UInts);
			CONFIG_SECT_COMBINER_DECL_NOLIST(Vector3, Vector3s);
			CONFIG_SECT_COMBINER_DECL_NOLIST(Point, Points);

#undef CONFIG_SECT_COMBINER_DECL
#undef CONFIG_SECT_COMBINER_DECL_NOLIST


			void SetStringValue(const String& name, const String& value);

			void SetBool(bool value);
			void SetSingle(float value);
			void SetPercentage(float value);
			void SetInt(int32 value);
			void SetUInt(uint32 value);
			void SetColorValue(ColorValue value);
			void SetVector3(const Vector3& v);

			void SetStrings(const String* v, int count);
			void SetSingles(const float* v, int count);
			void SetPercentages(const float* v, int count);
			void SetInts(const int32* v, int count);
			void SetUInts(const uint32* v, int count);
			void SetVector3s(const Vector3* v, int count);

			void SetStrings(const List<String>& v);
			void SetSingles(const List<float>& v);
			void SetPercentages(const List<float>& v);
			void SetInts(const List<int32>& v);
			void SetUInts(const List<uint32>& v);


			void SetBool(const String& name, bool value);
			void SetSingle(const String& name, float value);
			void SetPercentage(const String& name, float value);
			void SetInt(const String& name, int32 value);
			void SetUInt(const String& name, uint32 value);
			void SetColorValue(const String& name, ColorValue value);
			void SetVector3(const String& name, const Vector3& v);


			template <typename T>
			bool TryGetEnum(const String& name, T& result, const Utility::TypeDualConverter<T>& conv) const
			{
				const String* tmp = tryGetValue(name);
				if (tmp)
					return conv.TryParse(*tmp, result);
				return false;
			}

			template <typename T>
			bool TryGetAttributeEnum(const String& name, T& result, const Utility::TypeDualConverter<T>& conv) const
			{
				const String* tmp = tryGetAttribute(name);
				if (tmp)
					return conv.TryParse(*tmp, result);
				return false;
			}


			template <typename T>
			T GetEnum(const Utility::TypeDualConverter<T>& conv) const { return conv.Parse(m_value); }

			template <typename T>
			T GetEnum(const String& name, const Utility::TypeDualConverter<T>& conv) const { return conv.Parse(getValue(name)); }

			template <typename T>
			T GetAttributeEnum(const String& name, const Utility::TypeDualConverter<T>& conv) const { return conv.Parse(getAttribute(name)); }


			template <typename T>
			void AddEnum(const String& name, T result, const Utility::TypeDualConverter<T>& conv) { AddStringValue(name, conv.ToString(result)); }

			template <typename T>
			void AddAttributeEnum(const String& name, T result, const Utility::TypeDualConverter<T>& conv) { AddAttributeString(name, conv.ToString(result)); }

			template <typename T>
			void SetEnum(const String& name, T result, const Utility::TypeDualConverter<T>& conv) { SetStringValue(name, conv.ToString(result)); }




			template <typename T>
			bool TryGetGeneric(const String& name, T& result) const
			{
				const String* tmp = tryGetValue(name);
				if (tmp)
				{
					result.Parse(*tmp);
					return true;
				}
				return false;
			}

			template <typename T>
			bool TryGetAttributeGeneric(const String& name, T& result) const
			{
				const String* tmp = tryGetAttribute(name);
				if (tmp)
				{
					result.Parse(*tmp);
					return true;
				}
				return false;
			}


			template <typename T>
			void GetGeneric(T& r) const { return r.Parse(m_value); }

			template <typename T>
			void GetGeneric(const String& name, T& r) const { return r.Parse(getValue(name)); }

			template <typename T>
			void GetAttributeGeneric(const String& name, T& r) const { return r.Parse(getAttribute(name)); }


			template <typename T>
			void AddGeneric(const String& name, T& result) { AddStringValue(name, result.ToString()); }

			template <typename T>
			void AddAttributeGeneric(const String& name, T& result) { AddAttributeString(name, result.ToString()); }

			template <typename T>
			void SetGeneric(const String& name, T& result) { SetStringValue(name, result.ToString()); }

			void Merge(const ConfigurationSection* thatSect, bool noMessages);
			void Merge(const ConfigurationSection* thatSect, bool noMessages, const String& newSectionName);

			void RemoveIntersection(const ConfigurationSection* thatSect);

			void RemoveAttribute(const String& name);


			int32 GetHashCode() const;

		protected:

			String m_name;
			String m_value;

			AttributeTable m_attributes;
			SubSectionTable m_subSection;

		private:
			void DeepCopySubsections();
		};


		class ParameterDictionary : public HashMap<String, String>
		{
		public:

#define PDICT_GETER_DECL(type, typeName) \
			type Get##typeName(const String& key) const; \
			bool TryGet##typeName(const String& key, type& result) const;

			PDICT_GETER_DECL(bool, Bool);
			PDICT_GETER_DECL(float, Single);
			PDICT_GETER_DECL(float, Percentage);
			PDICT_GETER_DECL(int32, Int);
			PDICT_GETER_DECL(uint32, UInt);
			PDICT_GETER_DECL(ColorValue, ColorValue);
			PDICT_GETER_DECL(Vector3, Vector3);
			PDICT_GETER_DECL(Point, Point);

#undef PDICT_GETER_DECL


#define PDICT_SPLITER_DECL(type, typeName) \
			type Get##typeName(const String& key) const; \
			void Get##typeName(const String& key, type& result) const; \
			bool TryGet##typeName(const String& key, type& result) const;

			PDICT_SPLITER_DECL(List<String>, Strings);
			PDICT_SPLITER_DECL(List<float>, Singles);
			PDICT_SPLITER_DECL(List<float>, Percentages);
			PDICT_SPLITER_DECL(List<int32>, Ints);
			PDICT_SPLITER_DECL(List<uint32>, UInts);
			PDICT_SPLITER_DECL(List<Vector3>, Vector3s);
			PDICT_SPLITER_DECL(List<Point>, Points);

#undef PDICT_SPLITER_DECL


#define PDICT_SPLITER_ARR_DECL(type, typeName) \
			void Get##typeName(const String& key, type* v, int32 maxCount, int32* acutallCount = nullptr) const; \
			bool TryGet##typeName(const String& key, type* v, int32 maxCount, int32* acutallCount = nullptr) const; \
			void Get##typeName##Checked(const String& key, type* v, int32 expectedCount) const { int32 actuallCount; Get##typeName(key, v, expectedCount, &actuallCount); assert(actuallCount == expectedCount); } \
			bool TryGet##typeName##Checked(const String& key, type* v, int32 expectedCount) const { int32 actuallCount; bool r = TryGet##typeName(key, v, expectedCount, &actuallCount); if (r) assert(actuallCount == expectedCount); return r; } \
			template <int32 N> void Get##typeName##Checked(const String& key, type (&v)[N]) const { Get##typeName##Checked(key, v, N); } \
			template <int32 N> bool TryGet##typeName##Checked(const String& key, type (&v)[N]) const { return TryGet##typeName##Checked(key, v, N); } 

			PDICT_SPLITER_ARR_DECL(float, Singles);
			PDICT_SPLITER_ARR_DECL(float, Percentages);
			PDICT_SPLITER_ARR_DECL(int32, Ints);
			PDICT_SPLITER_ARR_DECL(uint32, UInts);
			PDICT_SPLITER_ARR_DECL(Vector3, Vector3s);
			PDICT_SPLITER_ARR_DECL(Point, Points);

#undef PDICT_SPLITER_ARR_DECL

			//////////////////////////////////////////////////////////////////////////

#define CONFIG_SECT_ADDER_DECL(type, typeName) void Add##typeName(const String& key, type value);

			CONFIG_SECT_ADDER_DECL(bool, Bool);
			CONFIG_SECT_ADDER_DECL(float, Single);
			CONFIG_SECT_ADDER_DECL(float, Percentage);
			CONFIG_SECT_ADDER_DECL(int32, Int);
			CONFIG_SECT_ADDER_DECL(uint32, UInt);
			CONFIG_SECT_ADDER_DECL(ColorValue, ColorValue);
			CONFIG_SECT_ADDER_DECL(const Vector3&, Vector3);
			CONFIG_SECT_ADDER_DECL(const Point&, Point);

#undef CONFIG_SECT_ADDER_DECL


#define CONFIG_SECT_COMBINER_DECL_NOLIST(type, typeName) void Add##typeName(const String& key, const type* v, int32 count); \

#define CONFIG_SECT_COMBINER_DECL(type, typeName) \
			CONFIG_SECT_COMBINER_DECL_NOLIST(type, typeName); \
			void Add##typeName(const String& key, const List<type>& v) { Add##typeName(key, v.getElements(), v.getCount()); } \

			CONFIG_SECT_COMBINER_DECL(String, Strings);
			CONFIG_SECT_COMBINER_DECL(float, Singles);
			CONFIG_SECT_COMBINER_DECL(float, Percentages);
			CONFIG_SECT_COMBINER_DECL(int32, Ints);
			CONFIG_SECT_COMBINER_DECL(uint32, UInts);
			CONFIG_SECT_COMBINER_DECL_NOLIST(Vector3, Vector3s);
			CONFIG_SECT_COMBINER_DECL_NOLIST(Point, Points);

#undef CONFIG_SECT_COMBINER_DECL
#undef CONFIG_SECT_COMBINER_DECL_NOLIST

			template <typename T>
			bool TryGetEnum(const String& name, const Utility::TypeDualConverter<T>& conv, T& result) const
			{
				const String* tmp = TryGetValue(name);
				if (tmp)
					return conv.TryParse(*tmp, result);
				return false;
			}

			template <typename T>
			T GetEnum(const String& name, const Utility::TypeDualConverter<T>& conv) const 
			{
				return conv.Parse(operator[](name));
			}

			template <typename T>
			void AddEnum(const String& name, const Utility::TypeDualConverter<T>& conv, T result)
			{
				AddStringValue(name, conv.ToString(result));
			}

			template <typename T>
			void SetEnum(const String& name, const Utility::TypeDualConverter<T>& conv, T result)
			{
				SetStringValue(name, conv.ToString(result));
			}
		};
	}
}
#endif