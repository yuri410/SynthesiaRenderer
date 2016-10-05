#pragma once

#ifndef APOC3D_OBJECTPOOL_H
#define APOC3D_OBJECTPOOL_H

#include "apoc3d/Common.h"
#include "apoc3d/Collections/Queue.h"
#include "apoc3d/Library/tinythread.h"

using namespace Apoc3D::Collections;

namespace Apoc3D
{
	namespace Core
	{
		class SizedPool
		{
		public:
			explicit SizedPool(int32 instanceSize);
			~SizedPool();

			bool Checkin(void* o);

			void* CheckOut();
		private:
			void EnsureReserve(bool noLock, int32 amount);

			int32 m_instanceSize;
			tthread::mutex m_poolLock;
			Queue<void*> m_pool;
		};

		template <typename T>
		struct TypedPoolInstance
		{
			static SizedPool Instance;
		};


#define DECL_POOL_TYPE(type) \
		static void* operator new(std::size_t sz) { \
			if (sz == sizeof(type))  \
				return Apoc3D::Core::TypedPoolInstance<type>::Instance.CheckOut(); \
			return ::operator new(sz); \
		} \
		static void operator delete(void* ptr, std::size_t sz) { \
			if (sz != sizeof(type) || !Apoc3D::Core::TypedPoolInstance<type>::Instance.Checkin(ptr)) \
				::operator delete(ptr); \
		}
	}
}

#endif