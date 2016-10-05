#pragma once
#ifndef APOC3D_EVENTDELEGATEBINDING_H
#define APOC3D_EVENTDELEGATEBINDING_H
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

#include "EventDelegate.h"

using namespace Apoc3D::Collections;

namespace Apoc3D
{
	template <typename ClassType, typename TargetType>
	class EventDelegateBindingInterface
	{
	public:
		virtual void Bind(ClassType* thisPtr, TargetType* tgtPtr) = 0;
		virtual void Unbind(ClassType* thisPtr, TargetType* tgtPtr) = 0;
	};

	template <typename ClassType, typename TargetType, typename ... Args>
	class EventDelegateBinding final : EventDelegateBindingInterface<ClassType, TargetType>
	{
		typedef EventDelegate<Args...> EventType;
	public:
		EventDelegateBinding(void (ClassType::*funcPtr)(Args...), EventType TargetType::*targetSlot)
			: m_funcPtr(funcPtr), m_targetSlot(targetSlot) { }

		void Bind(ClassType* thisPtr, TargetType* tgtPtr) override
		{
			EventType& slot = tgtPtr->*m_targetSlot;

			slot.Bind(thisPtr, m_funcPtr);
		}

		void Unbind(ClassType* thisPtr, TargetType* tgtPtr) override
		{
			EventType& slot = tgtPtr->*m_targetSlot;

			slot.Unbind(thisPtr, m_funcPtr);
		}

	private:

		void (ClassType::*m_funcPtr)(Args...) = nullptr;
		EventType TargetType::*m_targetSlot = nullptr;
	};

	template <typename ClassType, typename TargetType>
	class EventDelegateBindingGroup
	{
		typedef EventDelegateBindingInterface<ClassType, TargetType> BindingInterfaceType;
		
	public:

		class BindingStorage
		{
		public:
			FORCE_INLINE BindingStorage() { }

			template <typename ... Args>
			FORCE_INLINE BindingStorage(void (ClassType::*funcPtr)(Args...), EventDelegate<Args...> TargetType::*targetSlot)
			{
				static_assert(sizeof(EventDelegateBinding<ClassType, TargetType, Args...>) == SizePerField,
					"Can't hold object.");

				new (m_storage)EventDelegateBinding<ClassType, TargetType, Args...>(funcPtr, targetSlot);
			}

			// use this for overload resolution
			template <typename ... Args>
			FORCE_INLINE BindingStorage(EventDelegate<Args...> TargetType::*targetSlot, void (ClassType::*funcPtr)(Args...))
			{
				static_assert(sizeof(EventDelegateBinding<ClassType, TargetType, Args...>) == SizePerField,
					"Can't hold object.");

				new (m_storage)EventDelegateBinding<ClassType, TargetType, Args...>(funcPtr, targetSlot);
			}

			BindingInterfaceType* getInterface() const { return (BindingInterfaceType*)m_storage; }
		private:
			static const int32 SizePerField = sizeof(EventDelegateBinding<BindingStorage, BindingStorage, int>);

			char m_storage[SizePerField];
		};

		EventDelegateBindingGroup(std::initializer_list<BindingStorage> fields)
			: m_fields(fields) { }

		void BindAll(ClassType* thisPtr, TargetType* tgtPtr) const
		{
			for (const BindingStorage& fs : m_fields)
			{
				fs.getInterface()->Bind(thisPtr, tgtPtr);
			}
		}

		void UnbindAll(ClassType* thisPtr, TargetType* tgtPtr) const
		{
			for (const BindingStorage& fs : m_fields)
			{
				fs.getInterface()->Unbind(thisPtr, tgtPtr);
			}
		}

	private:
		List<BindingStorage> m_fields;

	};
	
}

#endif