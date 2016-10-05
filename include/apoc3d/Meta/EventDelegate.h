#pragma once
#ifndef APOC3D_EVENTDELEGATE_H
#define APOC3D_EVENTDELEGATE_H
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

#include "apoc3d/Collections/List.h"

using namespace Apoc3D::Collections;

namespace Apoc3D
{
	template <typename ... Args>
	class EventDelegate
	{
	public:
		typedef fastdelegate::FastDelegate<void(Args...)> DeleType;
		typedef Apoc3D::Collections::List <  DeleType > ListType;

		EventDelegate() { }
		~EventDelegate() { assert(!m_locked); }

		void Bind(const DeleType& v) { m_delegates.Add(v); }

		void Bind(void(*function_to_bind)(Args...)) { Bind(DeleType(function_to_bind)); }

		template < class X, class Y >
		void Bind(Y *pthis, void (X::* function_to_bind)(Args...)) { Bind(DeleType(pthis, function_to_bind)); }

		template < class X, class Y >
		void Bind(const Y *pthis, void (X::* function_to_bind)(Args...) const) { Bind(DeleType(pthis, function_to_bind)); }


		void Unbind(const DeleType& v) { assert(!m_locked); m_delegates.Remove(v); }

		void Unbind(void(*function_to_bind)(Args...)) { Unbind(DeleType(function_to_bind)); }

		template < class X, class Y >
		void Unbind(Y *pthis, void (X::* function_to_bind)(Args...)) { Unbind(DeleType(pthis, function_to_bind)); }

		template < class X, class Y >
		void Unbind(const Y *pthis, void (X::* function_to_bind)(Args...) const) { Unbind(DeleType(pthis, function_to_bind)); }


		void Reset() { assert(!m_locked); m_delegates.Clear(); }

		EventDelegate& operator +=(const DeleType& v) { Bind(v); return *this; }
		EventDelegate& operator +=(void(*function_to_bind)(Args...)) { Bind(function_to_bind); return *this; }

		EventDelegate& operator -=(const DeleType& v) { Unbind(v); return *this; }
		EventDelegate& operator -=(void(*function_to_bind)(Args...)) { Unbind(function_to_bind); return *this; }

		//void operator()() { Invoke(); }

		void Invoke(Args... args)
		{
			if (m_delegates.getCount())
			{
				if (UseDestructProtection)
				{
					ListType copy = m_delegates;
					for (const auto& e : copy)
						e(args...);
				}
				else
				{
					assert(!m_locked);
					m_locked = true;
					for (const auto& e : m_delegates)
						e(args...);
					m_locked = false;
				}
			}
		}

		int getCount() const { return m_delegates.getCount(); }

		bool UseDestructProtection = false;

	private:
		ListType m_delegates;
		bool m_locked = false;
	};
}

#endif