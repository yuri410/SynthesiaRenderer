#pragma once

#ifndef APOC3D_FASTQUEUE_H
#define APOC3D_FASTQUEUE_H

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

#include "List.h"

namespace Apoc3D
{
	namespace Collections
	{
		namespace QueueImpl
		{
			template <typename T>
			void CopyToNew(T* dest, const T* src, int32 arrLength, int32 head, int32 tail)
			{
				if (std::is_trivially_copyable<T>::value)
				{
					memcpy(dest, src, arrLength * sizeof(T));
				}
				else
				{
					if (head < tail)
					{
						for (int i = head; i < tail; i++)
							new (&dest[i])T(src[i]);
					}
					else
					{
						for (int i = 0; i < tail; i++)
							new (&dest[i])T(src[i]);

						for (int i = head; i < arrLength; i++)
							new (&dest[i])T(src[i]);
					}
				}
			}

			template <typename T>
			void MoveToNew(T* dest, T* src, int32 arrLength, int32 head, int32 tail)
			{
				if (std::is_trivially_copyable<T>::value)
				{
					memcpy(dest, src, arrLength * sizeof(T));
				}
				else
				{
					if (head < tail)
					{
						for (int i = head; i < tail; i++)
							new (&dest[i])T(std::move(src[i]));
					}
					else
					{
						for (int i = 0; i < tail; i++)
							new (&dest[i])T(std::move(src[i]));

						for (int i = head; i < arrLength; i++)
							new (&dest[i])T(std::move(src[i]));
					}
				}
			}


			template <typename T>
			void DoPutNew(const T& val, int32 idx, T* elm)
			{
				assert(idx >= 0);

				if (std::is_trivially_copyable<T>::value)
				{
					elm[idx] = val;
				}
				else
				{
					new (&elm[idx])T(val);
				}
			}

			template <typename T>
			void DoPutNew(T&& val, int32 idx, T* elm)
			{
				assert(idx >= 0);

				if (std::is_trivially_copyable<T>::value)
				{
					elm[idx] = std::move(val);
				}
				else
				{
					new (&elm[idx])T(std::move(val));
				}
			}

			template <typename T>
			void DoDestory(T* elm, int32 start, int32 count, int32 arrLen)
			{
				assert(start >= 0 && start + count <= arrLen);
				if (!std::is_trivially_copyable<T>::value)
				{
					for (int32 i = start; i < start + count; i++)
						elm[i].~T();
				}
			}

			template <typename T>
			void DoDestroyAll(T* elm, int32 head, int32 tail, int32 arrLen)
			{
				if (!std::is_trivially_copyable<T>::value)
				{
					if (head < tail)
						DoDestory(elm, head, tail - head, arrLen);
					else
					{
						DoDestory(elm, head, arrLen - head, arrLen);
						DoDestory(elm, 0, tail, arrLen);
					}
				}
			}

			template <typename T>
			void Clear(T* arr, int32& head, int32& tail, int32& count, int32 arrLen)
			{
				if (count > 0)
					DoDestroyAll(arr, head, tail, arrLen);

				head = tail = 0;
				count = 0;
			}

			template <typename T>
			bool Replace(const T& item, const T& item2, const T* arr, int32 head, int32 count, int32 arrLen)
			{
				int index = head;
				int counter = count;

				while (counter-- > 0)
				{
					if (arr[index] == item)
					{
						arr[index] = item2;
						return true;
					}

					index = (index + 1) % arrLen;
				}
				return false;
			}
			
			template <typename T>
			bool Contains(const T& item, const T* arr, int32 head, int32 count, int32 arrLen)
			{
				int index = head;
				int counter = count;

				while (counter-- > 0)
				{
					if (arr[index] == item)
						return true;

					index = (index + 1) % arrLen;
				}
				return false;
			}

			template <typename T>
			int32 IndexOf(const T& item, const T* arr, int32 head, int32 count, int32 arrLen)
			{
				int index = head;
				int counter = count;

				while (counter-- > 0)
				{
					if (arr[index] == item)
						return index;

					index = (index + 1) % arrLen;
				}
				return -1;
			}
			template <typename T>
			void DeletePointers(const T* arr, int32 head, int32 count, int32 arrLen)
			{
				int index = head;
				int counter = count;

				while (counter-- > 0)
				{
					delete arr[index];

					index = (index + 1) % arrLen;
				}
			}

			template <typename T>
			void DequeueOnly(T* arr, int32& head, int32& count, int32 arrLen)
			{
				assert(count > 0);

				DoDestory(arr, head, 1, arrLen);
				head = (head + 1) % arrLen;
				count--;
			}

			template <typename T>
			void DequeueOnly(int32 amount, T* arr, int32& head, int32& count, int32 arrLen)
			{
				assert(amount <= count);

				if (!std::is_trivially_copyable<T>::value)
				{
					if (amount > arrLen - head)
					{
						int32 p1 = arrLen - head;

						DoDestory(arr, head, p1, arrLen);
						DoDestory(arr, 0, amount - p1, arrLen);
					}
					else
						DoDestory(arr, head, amount, arrLen);
				}

				head = (head + amount) % arrLen;
				count -= amount;
			}

			template <typename T>
			T Dequeue(T* arr, int32& head, int32& count, int32 arrLen)
			{
				assert(count > 0);

				T temp = std::move(arr[head]);
				DequeueOnly(arr, head, count, arrLen);
				return temp;
			}

			template <typename T>
			void RemoveAt(int32 idx, T* arr, int32& head, int32& tail, int32& count, int32 arrLen)
			{
				assert(count > 0);
				assert(idx >= 0 && idx < count);

				if (head < tail)
				{
					for (int32 i = head + idx + 1; i < tail; i++)
					{
						arr[i - 1] = std::move(arr[i]);
					}
				}
				else
				{
					if (head + idx < arrLen)
					{
						for (int32 i = head + idx + 1; i < arrLen; i++)
						{
							arr[i - 1] = std::move(arr[i]);
						}

						if (0 < tail)
							arr[arrLen - 1] = std::move(arr[0]);

						for (int32 i = 1; i < tail; i++)
						{
							arr[i - 1] = std::move(arr[i]);
						}
					}
					else
					{
						for (int32 i = head + idx - arrLen + 1; i < tail; i++)
						{
							arr[i - 1] = std::move(arr[i]);
						}
					}
				}

				tail--;
				if (tail < 0)
					tail = arrLen - 1;

				DoDestory(arr, tail, 1, arrLen);

				count--;
			}


		}

		template<typename T>
		class Queue
		{
		public:
			Queue()
				: m_arrLength(4)
			{
				m_array = Allocate(4);
			}
			explicit Queue(int capacity)
				: m_arrLength(capacity)
			{
				m_array = Allocate(capacity);
			}

			~Queue() { Destruct(); }

			Queue(const Queue& o)
				: m_head(o.m_head), m_tail(o.m_tail), m_count(o.m_count), m_arrLength(o.m_arrLength)
			{
				m_array = Allocate(m_arrLength);

				if (m_count > 0)
					QueueImpl::CopyToNew(m_array, o.m_array, m_arrLength, m_head, m_tail);
			}

			Queue(Queue&& o)
				: m_array(o.m_array), m_arrLength(o.m_arrLength), m_count(o.m_count), m_head(o.m_head), m_tail(o.m_tail)
			{
				o.m_array = nullptr;
				o.m_count = 0;
				o.m_arrLength = 0;
				o.m_head = 0;
				o.m_tail = 0;
			}

			Queue& operator=(const Queue& o)
			{
				if (this != &o)
				{
					Destruct();
					new (this)Queue(o);
				}
				return *this;
			}

			Queue& operator=(Queue&& o)
			{
				if (this != &o)
				{
					Destruct();
					new (this)Queue(std::move(o));
				}
				return *this;
			}

			void Clear() { QueueImpl::Clear(m_array, m_head, m_tail, m_count, m_arrLength); }

			template <typename = typename std::enable_if<std::is_pointer<T>::value && std::is_destructible<typename std::remove_pointer<T>::type>::value>::type>
			void DeleteAndClear()
			{
				QueueImpl::DeletePointers(m_array, m_head, m_count, m_arrLength);
				Clear();
			}

			bool Replace(const T& item, const T& item2) { return QueueImpl::Replace(item, item2, m_array, item, m_head, m_count, m_arrLength); }
			bool Contains(const T& item) const { return QueueImpl::Contains(item, m_array, m_head, m_count, m_arrLength); }
			
			void CopyTo(T* dest) const
			{
				int remaining = m_count;

				if (remaining > 0)
				{
					int32 head = m_head;
					int32 arrLength = m_arrLength;

					int numHeadToEnd = (arrLength - head < remaining) ? (arrLength - head) : remaining;
					for (int i = 0; i < numHeadToEnd; i++)
					{
						dest[i] = m_array[i + head];
					}
					remaining -= numHeadToEnd;
					if (remaining > 0)
					{
						for (int i = 0; i < remaining; i++)
						{
							dest[i + arrLength - head] = m_array[i];
						}
					}
				}
			}

			void DequeueOnly() { QueueImpl::DequeueOnly(m_array, m_head, m_count, m_arrLength); }
			void DequeueOnly(int32 amount) { QueueImpl::DequeueOnly(amount, m_array, m_head, m_count, m_arrLength); }

			T Dequeue() { return QueueImpl::Dequeue(m_array, m_head, m_count, m_arrLength); }

			void Remove(const T& item)
			{
				int32 idx = QueueImpl::IndexOf(item);
				if (idx != -1)
				{
					RemoveAt(idx);
				}
			}

			void RemoveAt(int32 idx)
			{
				int32 head = m_head;
				int32 tail = m_tail;
				int32 count = m_count;
				QueueImpl::RemoveAt(idx, m_array, head, tail, count, m_arrLength);

				m_head = head;
				m_tail = tail;
				m_count = count;
			}


			void Enqueue(const T& item)
			{
				EnsureIncrCapacity();

				QueueImpl::DoPutNew(item, m_tail, m_array);
				m_tail = (m_tail + 1) % m_arrLength;
				m_count++;
			}

			void Enqueue(T&& item)
			{
				EnsureIncrCapacity();

				QueueImpl::DoPutNew(std::move(item), m_tail, m_array);
				m_tail = (m_tail + 1) % m_arrLength;
				m_count++;
			}


			int getCount() const { return m_count; }

			T& operator[](int i) { assert(i >= 0 && i < m_count); return m_array[(m_head + i) % m_arrLength]; }
			const T& operator[](int i) const { assert(i >= 0 && i < m_count); return m_array[(m_head + i) % m_arrLength]; }

			const T& Head() const { return m_array[m_head]; }
			const T& Tail() const { return operator[](m_count - 1); }

			class Iterator
			{
			public:
				Iterator(const Queue& ctn) : m_owner(&ctn) { }
				Iterator(const Queue& ctn, int32 idx) : m_owner(&ctn), m_idx(idx) { }

				T& operator*() const { return  m_owner->m_array[(m_owner->m_head + m_idx) % m_owner->m_arrLength]; }

				bool operator==(const Iterator& o) const { return m_owner == o.m_owner && m_idx == o.m_idx; }
				bool operator!=(const Iterator& o) const { return !(*this == o); }

				Iterator& operator++() { m_idx++; if (m_idx >= m_owner->m_count) m_idx = -1; return *this; }
				Iterator operator++(int)
				{
					Iterator result = *this;
					++(*this);
					return result;
				}

			private:
				const Queue* m_owner = nullptr;
				int32 m_idx = -1;
			};

			Iterator begin() const { return Iterator(*this, m_count > 0 ? 0 : -1); }
			Iterator end() const { return Iterator(*this); }

		private:
			static T* Allocate(int32 size) { return (T*)malloc(sizeof(T)*size); }
			static void Free(T*& ptr) { free(ptr); ptr = nullptr; }

			void SetCapacity(int capacity)
			{
				T* dest = Allocate(capacity);
				if (m_count > 0)
				{
					int32 head = m_head;
					int32 tail = m_tail;
					int32 arrLength = m_arrLength;

					if (std::is_trivially_copyable<T>::value)
					{
						if (head < tail)
						{
							memcpy(dest, m_array + head, sizeof(T)*m_count);
						}
						else
						{
							memcpy(dest, m_array + head, sizeof(T)*(arrLength - head));
							memcpy(dest + arrLength - head, m_array, sizeof(T)*tail);
						}
					}
					else
					{
						if (head < tail)
						{
							for (int i = 0; i < m_count; i++)
								new (&dest[i])T(std::move(m_array[i + head]));
						}
						else
						{
							for (int i = head; i < arrLength; i++)
								new (&dest[i - head])T(std::move(m_array[i]));

							for (int i = 0; i < tail; i++)
								new (&dest[arrLength - head + i])T(std::move(m_array[i]));
						}
					}

					QueueImpl::DoDestroyAll(m_array, head, tail, arrLength);
				}
				
				Free(m_array);
				
				m_array = dest;
				m_arrLength = capacity;
				m_head = 0;
				m_tail = (m_count == capacity) ? 0 : m_count;
			}

			void EnsureIncrCapacity()
			{
				if (m_count == m_arrLength)
				{
					int newCapacity = m_arrLength * 2;
					if (newCapacity < m_arrLength + 4)
					{
						newCapacity = m_arrLength + 4;
					}
					SetCapacity(newCapacity);
				}
			}

			void Destruct()
			{
				Clear();
				Free(m_array);
			}

			T* m_array;
			int32 m_arrLength;
			int32 m_count = 0;

			int32 m_head = 0;
			int32 m_tail = 0;
		};

		template<typename T, int32 N>
		class FixedQueue
		{
		public:
			static const int32 Size = N;

			FixedQueue() { }
			FixedQueue(const FixedQueue& o)
				: m_head(o.m_head), m_tail(o.m_tail), m_count(o.m_count)
			{
				if (m_count > 0)
					QueueImpl::CopyToNew((T*)m_storage, (T*)o.m_storage, N, m_head, m_tail);
			}
			
			FixedQueue(FixedQueue&& o)
				: m_head(o.m_head), m_tail(o.m_tail), m_count(o.m_count)
			{
				if (m_count > 0)
				{
					QueueImpl::MoveToNew((T*)m_storage, (T*)o.m_storage, N, m_head, m_tail);
					o.Clear();
				}
			}

			~FixedQueue() { Clear(); }

			FixedQueue& operator=(const FixedQueue& o)
			{
				if (this != &o)
				{
					Clear();
					new (this)FixedQueue(o);
				}
				return *this;
			}
			FixedQueue& operator=(FixedQueue&& o)
			{
				if (this != &o)
				{
					Clear();
					new (this)FixedQueue(std::move(o));
				}
				return *this;
			}

			void Clear() { QueueImpl::Clear((T*)m_storage, m_head, m_tail, m_count, N); }

			template <typename = typename std::enable_if<std::is_pointer<T>::value && std::is_destructible<typename std::remove_pointer<T>::type>::value>::type>
			void DeleteAndClear()
			{
				QueueImpl::DeletePointers((T*)m_storage, m_head, m_count, m_arrLength);
				Clear();
			}

			bool Replace(const T& item, const T& item2) { return QueueImpl::Replace(item, item2, (T*)m_storage, item, m_head, m_count, N); }
			bool Contains(const T& item) const { return QueueImpl::Contains(item, (T*)m_storage, m_head, m_count, N); }

			void CopyTo(T* dest, int32 amount) const
			{
				assert(amount <= m_count);
				int remaining = amount;

				if (remaining > 0)
				{
					T* arr = (T*)m_storage;

					int32 head = m_head;

					int numHeadToEnd = (N - head < remaining) ? (N - head) : remaining;
					for (int i = 0; i < numHeadToEnd; i++)
					{
						dest[i] = arr[i + head];
					}
					remaining -= numHeadToEnd;
					if (remaining > 0)
					{
						for (int i = 0; i < remaining; i++)
						{
							dest[i + N - head] = arr[i];
						}
					}
				}
			}

			void DequeueOnly() { QueueImpl::DequeueOnly((T*)m_storage, m_head, m_count, N); }
			void DequeueOnly(int32 amount) { QueueImpl::DequeueOnly(amount, (T*)m_storage, m_head, m_count, N); }

			T Dequeue() { return QueueImpl::Dequeue((T*)m_storage, m_head, m_count, N); }

			void RemoveAt(int32 idx)
			{
				int32 head = m_head;
				int32 tail = m_tail;
				int32 count = m_count;
				QueueImpl::RemoveAt(idx, (T*)m_storage, head, tail, count, N);

				m_head = head;
				m_tail = tail;
				m_count = count;
			}

			void Enqueue(const T& item)
			{
				assert(m_count < N);

				T* arr = (T*)m_storage;

				QueueImpl::DoPutNew(item, m_tail, arr);
				
				m_tail = (m_tail + 1) % N;
				m_count++;
			}
			void Enqueue(T&& item)
			{
				assert(m_count < N);

				T* arr = (T*)m_storage;

				QueueImpl::DoPutNew(std::move(item), m_tail, arr);

				m_tail = (m_tail + 1) % N;
				m_count++;
			}
			void Enqueue(const T* items, int32 count)
			{
				assert(m_count + count <= N);

				T* arr = (T*)m_storage;

				int32 partCount = Math::Min(count, N - m_tail);

				if (partCount > 0)
				{
					if (std::is_trivially_copyable<T>::value)
					{
						memcpy(arr + m_tail, items, partCount * sizeof(T));
						m_tail += partCount;
					}
					else
					{
						for (int32 i = 0; i < partCount; i++)
							QueueImpl::DoPutNew(items[i], m_tail++, arr);
					}

					m_tail = m_tail % N;
					m_count += partCount;

					items += partCount;
					count -= partCount;
				}

				if (count > 0)
				{
					if (std::is_trivially_copyable<T>::value)
					{
						memcpy(arr + m_tail, items, count * sizeof(T));
						m_tail += count;
					}
					else
					{
						for (int32 i = 0; i < count; i++)
							QueueImpl::DoPutNew(items[i], m_tail++, arr);
					}

					m_tail = m_tail % N;
					m_count += count;
				}
			}

			int getCount() const { return m_count; }
			int32 getCapacity() const { return N; }

			T& operator[](int i) { assert(i >= 0 && i < m_count); return ((T*)m_storage)[(m_head + i) % N]; }
			const T& operator[](int i) const { assert(i >= 0 && i < m_count); return ((T*)m_storage)[(m_head + i) % N]; }

			const T& Head() const { return ((T*)m_storage)[m_head]; }
			const T& Tail() const { return operator[](m_count - 1); }

			class Iterator
			{
			public:
				Iterator(const FixedQueue& ctn) : m_owner(&ctn) { }
				Iterator(const FixedQueue& ctn, int32 idx) : m_owner(&ctn), m_idx(idx) { }

				T& operator*() const { return ((T*)m_owner->m_storage)[(m_owner->m_head + m_idx) % N]; }

				bool operator==(const Iterator& o) const { return m_owner == o.m_owner && m_idx == o.m_idx; }
				bool operator!=(const Iterator& o) const { return !(*this == o); }

				Iterator& operator++() { m_idx++; if (m_idx >= m_owner->m_count) m_idx = -1; return *this; }
				Iterator operator++(int)
				{
					Iterator result = *this;
					++(*this);
					return result;
				}
				
			private:
				const FixedQueue* m_owner = nullptr;
				int32 m_idx = -1;
			};

			Iterator begin() const { return Iterator(*this, m_count > 0 ? 0 : -1); }
			Iterator end() const { return Iterator(*this); }

		private:
			int32 m_count = 0;

			int32 m_head = 0;
			int32 m_tail = 0;

			char m_storage[N * sizeof(T)];
		};


		template<typename T, int32 (*Comparer)(const T& a, const T& b) = &OrderComparer<T>>
		class PriorityQueue
		{
		public:
			PriorityQueue() 
			{ }

			PriorityQueue(int32 capacity) 
				: m_innerList(capacity) { }


			void Enqueue(const T& item)
			{
				int32 p = m_innerList.getCount();
				int32 p2;

				m_innerList.Add(item);

				for (;;)
				{
					if (p == 0) break;
					p2 = (p - 1) / 2;

					if (CompareListItem(p, p2) < 0)
					{
						SwapListItem(p, p2);
						p = p2;
					}
					else break;
				}
			}

			T Dequeue()
			{
				assert(m_innerList.getCount() > 0);

				const T result = std::move(m_innerList[0]);

				int32 p = 0;

				m_innerList[0] = m_innerList[m_innerList.getCount() - 1];
				m_innerList.RemoveAt(m_innerList.getCount() - 1);

				for (;;)
				{
					int32 pn = p;
					int32 p1 = p * 2 + 1;
					int32 p2 = p * 2 + 2;

					if (m_innerList.getCount() > p1 && CompareListItem(p, p1) > 0)
						p = p1;
					if (m_innerList.getCount() > p2 && CompareListItem(p, p2) > 0)
						p = p2;

					if (p == pn) break;

					SwapListItem(p, pn);
				}

				return result;
			}
			T& Top() { return m_innerList[0]; }

			void UpdatePriorityForChange(int32 index)
			{
				int32 p = index;

				for (;;)
				{
					if (p == 0) break;

					int32 p2 = (p - 1) / 2;

					if (CompareListItem(p, p2) < 0)
					{
						SwapListItem(p, p2);
						p = p2;
					}
					else break;
				}

				if (p < index) return;

				for (;;)
				{
					int32 pn = p;
					int32 p1 = p * 2 + 1;
					int32 p2 = p * 2 + 2;

					if (m_innerList.getCount() > p1 && CompareListItem(p, p1) > 0)
						p = p1;
					if (m_innerList.getCount() > p2 && CompareListItem(p, p2) > 0)
						p = p2;

					if (p == pn) break;

					SwapListItem(p, pn);
				}
			}


			void Clear() { m_innerList.Clear(); }

			List<T>& getInternalList() { return m_innerList; }

			int32 getCount() const { return m_innerList.getCount(); }

		private:
			void SwapListItem(int32 a, int32 b) { std::swap(m_innerList[a], m_innerList[b]); }
			int32 CompareListItem(int32 a, int32 b) { return Comparer(m_innerList[a], m_innerList[b]); }
			
			List<T> m_innerList;
		};
	}
}
#endif