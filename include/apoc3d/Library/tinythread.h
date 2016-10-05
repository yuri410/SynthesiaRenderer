/* -*- mode: c++; tab-width: 2; indent-tabs-mode: nil; -*-
Copyright (c) 2010-2012 Marcus Geelnard

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

/*
 *  This library is modified so platform headers are not included in this header and 
 *  spreading to others.
 *  Also C++11 is assumed.
 */

#ifndef _TINYTHREAD_H_
#define _TINYTHREAD_H_

#include "../ApocExportSettings.h"

/// @file
/// @mainpage TinyThread++ API Reference
///
/// @section intro_sec Introduction
/// TinyThread++ is a minimal, portable implementation of basic threading
/// classes for C++.
///
/// They closely mimic the functionality and naming of the C++11 standard, and
/// should be easily replaceable with the corresponding std:: variants.
///
/// @section port_sec Portability
/// The Win32 variant uses the native Win32 API for implementing the thread
/// classes, while for other systems, the POSIX threads API (pthread) is used.
///
/// @section class_sec Classes
/// In order to mimic the threading API of the C++11 standard, subsets of
/// several classes are provided. The fundamental classes are:
/// @li tthread::thread
/// @li tthread::mutex
/// @li tthread::recursive_mutex
/// @li tthread::condition_variable
/// @li tthread::lock_guard
/// @li tthread::fast_mutex
///
/// @section misc_sec Miscellaneous
/// The following special keywords are available: #thread_local.
///
/// For more detailed information (including additional classes), browse the
/// different sections of this documentation. A good place to start is:
/// tinythread.h.

// Which platform are we on?
#if !defined(_TTHREAD_PLATFORM_DEFINED_)
  #if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define _TTHREAD_WIN32_
  #else
    #define _TTHREAD_POSIX_
  #endif
  #define _TTHREAD_PLATFORM_DEFINED_
#endif



// Generic includes
#include <ostream>

/// TinyThread++ version (major number).
#define TINYTHREAD_VERSION_MAJOR 1
/// TinyThread++ version (minor number).
#define TINYTHREAD_VERSION_MINOR 1
/// TinyThread++ version (full version).
#define TINYTHREAD_VERSION (TINYTHREAD_VERSION_MAJOR * 100 + TINYTHREAD_VERSION_MINOR)

// Macro for disabling assignments of objects.

  #define _TTHREAD_DISABLE_ASSIGNMENT(name) \
	private: \
      name(const name&) = delete; \
      name& operator=(const name&) = delete;



/// Main name space for TinyThread++.
/// This namespace is more or less equivalent to the @c std namespace for the
/// C++11 thread classes. For instance, the tthread::mutex class corresponds to
/// the std::mutex class.
namespace tthread
{

	/// Mutex class.
	/// This is a mutual exclusion object for synchronizing access to shared
	/// memory areas for several threads. The mutex is non-recursive (i.e. a
	/// program may deadlock if the thread that owns a mutex object calls lock()
	/// on that object).
	/// @see recursive_mutex
	class APAPI mutex
	{
	public:
		/// Constructor.
		mutex();

		/// Destructor.
		~mutex();

		/// Lock the mutex.
		/// The method will block the calling thread until a lock on the mutex can
		/// be obtained. The mutex remains locked until @c unlock() is called.
		/// @see lock_guard
		void lock();

		/// Try to lock the mutex.
		/// The method will try to lock the mutex. If it fails, the function will
		/// return immediately (non-blocking).
		/// @return @c true if the lock was acquired, or @c false if the lock could
		/// not be acquired.
		bool try_lock();

		/// Unlock the mutex.
		/// If any threads are waiting for the lock on this mutex, one of them will
		/// be unblocked.
		void unlock();

		_TTHREAD_DISABLE_ASSIGNMENT(mutex)

	private:
		struct mutex_data& get_data();
		char m_data[28];

		friend class condition_variable;
	};

	/// Recursive mutex class.
	/// This is a mutual exclusion object for synchronizing access to shared
	/// memory areas for several threads. The mutex is recursive (i.e. a thread
	/// may lock the mutex several times, as long as it unlocks the mutex the same
	/// number of times).
	/// @see mutex
	class APAPI recursive_mutex
	{
	public:
		/// Constructor.
		recursive_mutex();

		/// Destructor.
		~recursive_mutex();


		/// Lock the mutex.
		/// The method will block the calling thread until a lock on the mutex can
		/// be obtained. The mutex remains locked until @c unlock() is called.
		/// @see lock_guard
		void lock();

		/// Try to lock the mutex.
		/// The method will try to lock the mutex. If it fails, the function will
		/// return immediately (non-blocking).
		/// @return @c true if the lock was acquired, or @c false if the lock could
		/// not be acquired.
		bool try_lock();

		/// Unlock the mutex.
		/// If any threads are waiting for the lock on this mutex, one of them will
		/// be unblocked.
		void unlock();

		_TTHREAD_DISABLE_ASSIGNMENT(recursive_mutex)

	private:
		struct recursive_mutex_data& get_data();
		char m_data[24];

		friend class condition_variable;
	};

	/// Lock guard class.
	/// The constructor locks the mutex, and the destructor unlocks the mutex, so
	/// the mutex will automatically be unlocked when the lock guard goes out of
	/// scope. Example usage:
	/// @code
	/// mutex m;
	/// int counter;
	///
	/// void increment()
	/// {
	///   lock_guard<mutex> guard(m);
	///   ++ counter;
	/// }
	/// @endcode

	template <class T>
	class APAPI lock_guard
	{
	public:
		typedef T mutex_type;

		lock_guard() : mMutex(0) {}

		/// The constructor locks the mutex.
		explicit lock_guard(mutex_type &aMutex)
		{
			mMutex = &aMutex;
			mMutex->lock();
		}

		/// The destructor unlocks the mutex.
		~lock_guard()
		{
			if (mMutex)
				mMutex->unlock();
		}

	private:
		mutex_type * mMutex;
	};

	/// Condition variable class.
	/// This is a signalling object for synchronizing the execution flow for
	/// several threads. Example usage:
	/// @code
	/// // Shared data and associated mutex and condition variable objects
	/// int count;
	/// mutex m;
	/// condition_variable cond;
	///
	/// // Wait for the counter to reach a certain number
	/// void wait_counter(int targetCount)
	/// {
	///   lock_guard<mutex> guard(m);
	///   while(count < targetCount)
	///     cond.wait(m);
	/// }
	///
	/// // Increment the counter, and notify waiting threads
	/// void increment()
	/// {
	///   lock_guard<mutex> guard(m);
	///   ++ count;
	///   cond.notify_all();
	/// }
	/// @endcode
	class APAPI condition_variable
	{
	public:
		/// Constructor.
		condition_variable();

		/// Destructor.
		~condition_variable();

		/// Wait for the condition.
		/// The function will block the calling thread until the condition variable
		/// is woken by @c notify_one(), @c notify_all() or a spurious wake up.
		/// @param[in] aMutex A mutex that will be unlocked when the wait operation
		///   starts, an locked again as soon as the wait operation is finished.
		void wait(mutex &aMutex);

		/// Notify one thread that is waiting for the condition.
		/// If at least one thread is blocked waiting for this condition variable,
		/// one will be woken up.
		/// @note Only threads that started waiting prior to this call will be
		/// woken up.
		void notify_one();

		/// Notify all threads that are waiting for the condition.
		/// All threads that are blocked waiting for this condition variable will
		/// be woken up.
		/// @note Only threads that started waiting prior to this call will be
		/// woken up.
		void notify_all();

		_TTHREAD_DISABLE_ASSIGNMENT(condition_variable)

	private:
#if defined(_TTHREAD_WIN32_)
		void _wait();
#endif
		struct condition_variable_data& get_data();
		char m_data[36];
	};


	/// Thread class.
	class APAPI thread
	{
		friend struct thread_internal_wrapper;
	public:
#if defined(_TTHREAD_WIN32_)
		typedef void* native_handle_type;
#else
		typedef pthread_t native_handle_type;
#endif
		class id;

		/// Default constructor.
		/// Construct a @c thread object without an associated thread of execution
		/// (i.e. non-joinable).
		thread();

		/// Thread starting constructor.
		/// Construct a @c thread object with a new thread of execution.
		/// @param[in] aFunction A function pointer to a function of type:
		///          <tt>void fun(void * arg)</tt>
		/// @param[in] aArg Argument to the thread function.
		/// @note This constructor is not fully compatible with the standard C++
		/// thread class. It is more similar to the pthread_create() (POSIX) and
		/// CreateThread() (Windows) functions.
		thread(void(*aFunction)(void *), void * aArg);

		/// Destructor.
		/// @note If the thread is joinable upon destruction, @c std::terminate()
		/// will be called, which terminates the process. It is always wise to do
		/// @c join() before deleting a thread object.
		~thread();

		/// Wait for the thread to finish (join execution flows).
		/// After calling @c join(), the thread object is no longer associated with
		/// a thread of execution (i.e. it is not joinable, and you may not join
		/// with it nor detach from it).
		void join();

		/// Check if the thread is joinable.
		/// A thread object is joinable if it has an associated thread of execution.
		bool joinable() const;

		/// Detach from the thread.
		/// After calling @c detach(), the thread object is no longer assicated with
		/// a thread of execution (i.e. it is not joinable). The thread continues
		/// execution without the calling thread blocking, and when the thread
		/// ends execution, any owned resources are released.
		void detach();

		/// Return the thread ID of a thread object.
		id get_id() const;

		/// Get the native handle for this thread.
		/// @note Under Windows, this is a @c HANDLE, and under POSIX systems, this
		/// is a @c pthread_t.
		inline native_handle_type native_handle()
		{
			return mHandle;
		}

		/// Determine the number of threads which can possibly execute concurrently.
		/// This function is useful for determining the optimal number of threads to
		/// use for a task.
		/// @return The number of hardware thread contexts in the system.
		/// @note If this value is not defined, the function returns zero (0).
		static unsigned hardware_concurrency();

		_TTHREAD_DISABLE_ASSIGNMENT(thread)

	private:
		native_handle_type mHandle;   ///< Thread handle.

		mutable mutex mDataMutex;     ///< Serializer for access to the thread private data.
		bool mNotAThread;             ///< True if this object is not a thread of execution.

#if defined(_TTHREAD_WIN32_)
		unsigned int mWin32ThreadID;  ///< Unique thread ID (filled out by _beginthreadex).
#endif

	};

	/// Thread ID.
	/// The thread ID is a unique identifier for each thread.
	/// @see thread::get_id()
	class APAPI thread::id
	{
	public:
		/// Default constructor.
		/// The default constructed ID is that of thread without a thread of
		/// execution.
		id() : mId(0) {};

		id(unsigned long int aId) : mId(aId) {};

		id(const id& aId) : mId(aId.mId) {};

		unsigned long int hash() const { return mId; }

		inline id & operator=(const id &aId)
		{
			mId = aId.mId;
			return *this;
		}

		inline friend bool operator==(const id &aId1, const id &aId2)
		{
			return (aId1.mId == aId2.mId);
		}

		inline friend bool operator!=(const id &aId1, const id &aId2)
		{
			return (aId1.mId != aId2.mId);
		}

		inline friend bool operator<=(const id &aId1, const id &aId2)
		{
			return (aId1.mId <= aId2.mId);
		}

		inline friend bool operator<(const id &aId1, const id &aId2)
		{
			return (aId1.mId < aId2.mId);
		}

		inline friend bool operator>=(const id &aId1, const id &aId2)
		{
			return (aId1.mId >= aId2.mId);
		}

		inline friend bool operator>(const id &aId1, const id &aId2)
		{
			return (aId1.mId > aId2.mId);
		}

		inline friend std::ostream& operator <<(std::ostream &os, const id &obj)
		{
			os << obj.mId;
			return os;
		}

	private:
		unsigned long int mId;
	};


	// Related to <ratio> - minimal to be able to support chrono.
	typedef long long __intmax_t;

	/// Minimal implementation of the @c ratio class. This class provides enough
	/// functionality to implement some basic @c chrono classes.
	template <__intmax_t N, __intmax_t D = 1> class ratio {
	public:
		static double _as_double() { return double(N) / double(D); }
	};


	/// The namespace @c this_thread provides methods for dealing with the
	/// calling thread.
	namespace this_thread
	{
		/// Return the thread ID of the calling thread.
		APAPI thread::id get_id();

		/// Yield execution to another thread.
		/// Offers the operating system the opportunity to schedule another thread
		/// that is ready to run on the current processor.
		APAPI void yield();
	}

}

// Define/macro cleanup
#undef _TTHREAD_DISABLE_ASSIGNMENT

#endif // _TINYTHREAD_H_
