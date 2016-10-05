#pragma once
#ifndef APOC3D_FIELDDATA_H
#define APOC3D_FIELDDATA_H

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

namespace Apoc3D
{
	template <typename ClassType>
	struct DictionaryFieldInterface
	{
		virtual void Read(ClassType* inst, IO::TaggedDataReader* data) = 0;
		virtual void Write(ClassType* inst, IO::TaggedDataWriter* data) = 0;
	};

	template <typename ClassType, typename T>
	class DictionaryDataField : public DictionaryFieldInterface<ClassType>
	{
	public:
		DictionaryDataField(const char name[], T ClassType::* ptr)
			: m_name(name), m_member(ptr) { }

		void Read(ClassType* inst, IO::TaggedDataReader* data) override
		{
			data->TryGetAuto(m_name, inst->*m_member);
		}
		void Write(ClassType* inst, IO::TaggedDataWriter* data) override
		{
			data->AddAuto(m_name, inst->*m_member);
		}

	private:
		const char* m_name;
		T ClassType::* m_member;
	};

	template <typename ClassType>
	class DictionaryDataFieldStorage
	{
		static const int32 StorageSize = sizeof(DictionaryDataField<ClassType, int32>);
	public:
		typedef DictionaryFieldInterface<ClassType> InterfaceType;

		template <typename T>
		DictionaryDataFieldStorage(DictionaryDataField<ClassType, T>& fld)
		{
			static_assert(sizeof(m_storage) >= sizeof(fld), "Can't hold object");
			new (m_storage)DictionaryDataField<ClassType, T>(fld);
		}

		InterfaceType* operator->() const { return (InterfaceType*)m_storage; }

	private:
		char m_storage[StorageSize];
	};

#define DICT_FIELD(classType, exp) DictionaryDataField<classType, decltype(exp)>(#exp, &classType::exp)

}

#endif
