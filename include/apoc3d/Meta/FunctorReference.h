#pragma once
#ifndef APOC3D_FUNCTORREFERENCE_H
#define APOC3D_FUNCTORREFERENCE_H
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
	template <typename T>
	class FunctorReference;  // no impl

	template <class Ret, class ...Args>
	class FunctorReference < Ret(Args...) >
	{
		using InvokerType = Ret(*)(void*, Args&&...);
		using RawFunctionType = Ret(*)(Args...);

	public:
		FunctorReference() { }

		FunctorReference(std::nullptr_t) { }

		template <typename F>
		FunctorReference(const F& lambda)
		{
			m_instance = const_cast<F*>(&lambda);
			m_invoker = &Invoker_ConstMethod<F, &F::operator()> ;
		}

		template <typename F> // prevent const in T
		FunctorReference(typename std::enable_if<std::is_const<F>::value, F>::type& lambda)
		{
			m_instance = const_cast<F*>(&lambda);
			m_invoker = &Invoker_Method < F, &F::operator() > ;
		} 

		FunctorReference(RawFunctionType functionPtr)
			: m_rawFunction(functionPtr) { }


		template <class ClassType, Ret(ClassType::*method_ptr)(Args...)>
		FunctorReference& Bind(ClassType* ct)
		{
			m_instance = const_cast<ClassType*>(ct);
			m_invoker = &Invoker_Method<ClassType, method_ptr>;
			return *this;
		}

		template <class ClassType, Ret(ClassType::*method_ptr)(Args...) const>
		FunctorReference& Bind(const ClassType* ct)
		{
			m_instance = const_cast<ClassType*>(ct);
			m_invoker = &Invoker_ConstMethod<ClassType, method_ptr> ;
			return *this;
		}

		Ret operator()(Args... args) const
		{
			if (m_rawFunction)
				return m_rawFunction(std::forward<Args>(args)...);

			return m_invoker(m_instance, std::forward<Args>(args)...);
		}

		bool isNull() const { return m_instance == nullptr && m_rawFunction == nullptr; }
	private:
		RawFunctionType m_rawFunction = nullptr;
		void* m_instance = nullptr;
		InvokerType m_invoker = 0;

		template <class ClassType, Ret(ClassType::*method_ptr)(Args...)>
		static Ret Invoker_Method(void* object_ptr, Args&&... args)
		{
			return (static_cast<ClassType*>(object_ptr)->*method_ptr)(std::forward<Args>(args)...);
		}

		template <class ClassType, Ret(ClassType::*method_ptr)(Args...) const>
		static Ret Invoker_ConstMethod(void* object_ptr, Args&&... args)
		{
			return (static_cast<const ClassType*>(object_ptr)->*method_ptr)(std::forward<Args>(args)...);
		}
	};

}

#endif