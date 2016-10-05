#pragma once

#ifndef APOC3D_HASHMAP_H
#define APOC3D_HASHMAP_H

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

#include "CollectionsCommon.h"
#include "apoc3d/Exception.h"

namespace Apoc3D
{
	namespace Collections
	{
		template <typename T, typename E, typename ComparerType>
		class HashMapCore
		{
		public:
			void Clear()
			{
				if (m_touchedSlots > 0)
				{
					for (int32 i = 0; i < m_bucketsLength;i++)
						m_buckets[i] = -1;
					for (int32 i = 0; i < m_touchedSlots; i++)
						m_entries[i].Clear();

					m_emptySlot = -1;
					m_touchedSlots = 0;
					m_count = 0;
				}
			}

			bool Remove(const T& item) { return RemoveGeneric<nullptr>(item); }

			bool Contains(const T& item) const { return FindEntry(item) != -1; }
			void Resize(int32 newSize)
			{
				int primeCapacity = Utils::GetHashTableSize(newSize);
				int* newBuckets = new int[primeCapacity];
				for (int i = 0; i < primeCapacity; i++)
				{
					newBuckets[i] = -1;
				}
				E* newEntries = new E[primeCapacity];
				for (int j = 0; j < m_touchedSlots; j++)
				{
					int index = m_entries[j].hashCode % primeCapacity;

					newEntries[j] = std::move(m_entries[j]);
					newEntries[j].next = newBuckets[index];
					newBuckets[index] = j;
				}
				delete[] m_buckets;
				m_buckets = newBuckets;
				m_bucketsLength = primeCapacity;
				delete[] m_entries;
				m_entries = newEntries;
			}

			int32 getPrimeCapacity() const { return m_bucketsLength; }
			int32 getCount() const { return m_count; }
			
		protected:
			static const int32 PositiveMask = 0x7fffffff;

			explicit HashMapCore(int capacity)
			{
				if (capacity > 0)
					Initialize(capacity);
			}

			HashMapCore(const HashMapCore& other)
				: m_bucketsLength(other.m_bucketsLength),
				m_touchedSlots(other.m_touchedSlots), m_count(other.m_count),
				m_emptySlot(other.m_emptySlot)
			{
				if (other.m_buckets)
				{
					m_buckets = new int[m_bucketsLength];
					memcpy(m_buckets, other.m_buckets, sizeof(int)*m_bucketsLength);
				}
				
				if (other.m_entries)
				{
					m_entries = new E[m_bucketsLength];
					for (int i = 0; i < m_bucketsLength; i++)
						m_entries[i] = other.m_entries[i];
				}
			}
			HashMapCore(HashMapCore&& other)
				: m_buckets(other.m_buckets), m_bucketsLength(other.m_bucketsLength),
				m_entries(other.m_entries), m_touchedSlots(other.m_touchedSlots),
				m_count(other.m_count), m_emptySlot(other.m_emptySlot)
			{
				other.m_buckets = nullptr;
				other.m_entries = nullptr;
				other.m_bucketsLength = 0;
				other.m_touchedSlots = 0;
				other.m_count = 0;
				other.m_emptySlot = -1;
			}
			HashMapCore& operator=(const HashMapCore& other)
			{
				if (this != &other)
				{
					if (m_bucketsLength != other.m_bucketsLength)
					{
						delete[] m_entries;
						delete[] m_buckets;

						m_buckets = nullptr;
						m_entries = nullptr;

						m_bucketsLength = other.m_bucketsLength;

						if (other.m_buckets)
							m_buckets = new int[m_bucketsLength];
						if (other.m_entries)
							m_entries = new E[m_bucketsLength];
					}
					else
					{
						for (int32 i = 0; i < m_touchedSlots; i++)
							m_entries[i].Clear();

						if (m_buckets == nullptr && other.m_buckets)
							m_buckets = new int[m_bucketsLength];
						if (m_entries == nullptr && other.m_entries)
							m_entries = new E[m_bucketsLength];
					}

					m_touchedSlots = other.m_touchedSlots;
					m_count = other.m_count;
					m_emptySlot = other.m_emptySlot;

					for (int i = 0; i < m_bucketsLength; i++)
						m_entries[i] = other.m_entries[i];

					memcpy(m_buckets, other.m_buckets, sizeof(int)*m_bucketsLength);
				}
				return *this;
			}
			HashMapCore& operator=(HashMapCore&& other)
			{
				if (this != &other)
				{
					delete[] m_entries;
					delete[] m_buckets;

					m_buckets = other.m_buckets;
					m_bucketsLength = other.m_bucketsLength;

					m_entries = other.m_entries;

					m_touchedSlots = other.m_touchedSlots;
					m_count = other.m_count;
					m_emptySlot = other.m_emptySlot;

					other.m_buckets = nullptr;
					other.m_entries = nullptr;
					other.m_bucketsLength = 0;
					other.m_touchedSlots = 0;
					other.m_count = 0;
					other.m_emptySlot = -1;
				}
				return *this;
			}
			~HashMapCore()
			{
				delete[] m_entries;
				delete[] m_buckets;
			}

			int* m_buckets = nullptr;
			E* m_entries = nullptr;

			int m_bucketsLength = 0;
			int m_touchedSlots = 0;

			int m_count = 0;
			int m_emptySlot = -1;

			void Initialize(int capacity)
			{
				int primeCapacity = Utils::GetHashTableSize(capacity);
				m_bucketsLength = primeCapacity;
				m_entries = new E[primeCapacity];
				m_buckets = new int[primeCapacity];
				for (int i = 0; i < primeCapacity; i++)
				{
					m_buckets[i] = -1;
				}
			}

			int FindEntry(const T& item) const
			{
				if (m_buckets)
				{
					int hash = ComparerType::GetHashCode(item) & PositiveMask;
					int index = hash % m_bucketsLength;

					return FindEntry(item, hash, index);
				}
				return -1;
			}

			int FindEntry(const T& item, int32 hash, int32 index) const
			{
				for (int i = m_buckets[index]; i >= 0; i = m_entries[i].next)
				{
					if (m_entries[i].hashCode == hash &&
						ComparerType::Equals(m_entries[i].getData(), item))
					{
						return i;
					}
				}
				return -1;
			}

			template <typename TT, typename ... ADDT>
			void InsertEntry(bool noDuplicationCheck, TT&& item, ADDT&& ... additionals)
			{
				if (m_buckets == nullptr)
				{
					Initialize(7);
				}

				int hash = ComparerType::GetHashCode(std::forward<TT>(item)) & PositiveMask;
				int index = hash % m_bucketsLength;

				if (!noDuplicationCheck && FindEntry(std::forward<TT>(item), hash, index) != -1)
					throw AP_EXCEPTION(ExceptID::Duplicate, Utils::ToString(item));

				
				int pos = m_emptySlot;
				if (pos >= 0)
				{
					m_emptySlot = m_entries[pos].next;
				}
				else
				{
					if (m_touchedSlots == m_bucketsLength)
					{
						Resize(m_touchedSlots * 2);
						index = hash % m_bucketsLength;
					}
					pos = m_touchedSlots++;
				}
				m_count++;

				m_entries[pos].Set(hash, std::forward<TT>(item), std::forward<ADDT>(additionals)...);
				m_entries[pos].next = m_buckets[index];

				m_buckets[index] = pos;
			}

			template <void (*additionalAction)(E&)>
			bool RemoveGeneric(const T& item)
			{
				if (m_buckets)
				{
					int hash = ComparerType::GetHashCode(item) & PositiveMask;
					int index = hash % m_bucketsLength;
					int prev = -1;
					for (int i = m_buckets[index]; i >= 0; i = m_entries[i].next)
					{
						if (m_entries[i].hashCode == hash &&
							ComparerType::Equals(m_entries[i].getData(), item))
						{
							if (prev < 0)
							{
								m_buckets[index] = m_entries[i].next;
							}
							else
							{
								m_entries[prev].next = m_entries[i].next;
							}
							if (additionalAction)
								additionalAction(m_entries[i]);
							m_entries[i].Clear();
							//m_entries[i].hashCode = -1;
							m_entries[i].next = m_emptySlot;
							m_emptySlot = i;
							m_count--;

							return true;
						}
						prev = i;
					}
				}
				return false;
			}
		};

		template <typename T, typename S, typename ComparerType = Apoc3D::Collections::EqualityComparer<T>>
		class HashMap : public HashMapCore<T, Utils::HashMapEntry<T,S>, ComparerType>
		{
			typedef HashMap<T, S, ComparerType> HashMapType;
			typedef Utils::HashMapEntry<T, S> Entry;
		public:
			class IteratorBase
			{
				friend class HashMap;
			public:
				IteratorBase& operator++()
				{
					MoveToNext();
					return *this;
				}
				IteratorBase operator++(int) { IteratorBase result = *this; ++(*this); return result; }

				bool operator==(const IteratorBase& rhs) const { return m_dict == rhs.m_dict && m_next == rhs.m_next; }
				bool operator!=(const IteratorBase& rhs) const { return !this->operator==(rhs); }

			protected:

				explicit IteratorBase(const HashMapType* dict)
					: m_dict(dict)
				{
					MoveToNext();
				}

				IteratorBase(const HashMapType* dict, int idx)
					: m_dict(dict), m_next(idx) { }


				void MoveToNext()
				{
					while (m_next < m_dict->m_touchedSlots)
					{
						if (m_dict->m_entries[m_next++].hashCode >= 0)
						{
							return;
						}
					}
					m_next = m_dict->m_touchedSlots + 1;
				}

				const HashMapType* m_dict;
				int m_next = 0;
			};

			template <bool IsAccessingKey>
			class IteratorKV : public IteratorBase
			{
				friend class HashMap;
			public:
				typedef typename std::conditional<IsAccessingKey, const T, S>::type ReturnType;

				ReturnType& operator*() const
				{
					return std::conditional<IsAccessingKey, KeyGetter, ValueGetter>::type::Get(m_dict, m_next);
				}

			private:

				explicit IteratorKV(const HashMapType* dict)
					: IteratorBase(dict) { }

				IteratorKV(const HashMapType* dict, int idx)
					: IteratorBase(dict, idx) { }

				struct KeyGetter
				{
					static const T& Get(const HashMapType* dict, int idx)
					{
						assert(idx > 0 && idx <= dict->m_touchedSlots);
						return dict->m_entries[idx - 1].getData();
					}
				};
				struct ValueGetter
				{
					static S& Get(const HashMapType* dict, int idx)
					{
						assert(idx > 0 && idx <= dict->m_touchedSlots);
						return dict->m_entries[idx - 1].getValue();
					}
				};
			};

			class Iterator : public IteratorBase
			{
				friend class HashMap;
			public:

				KeyPairValue<const T&, S&> operator*() const
				{
					assert(m_next > 0 && m_next <= m_dict->m_touchedSlots);
					Entry& e = m_dict->m_entries[m_next - 1];
					return { e.getData(), e.getValue() };
				}
			private:
				explicit Iterator(const HashMapType* dict)
					: IteratorBase(dict) { }

				Iterator(const HashMapType* dict, int idx)
					: IteratorBase(dict, idx) { }
			};


			template <bool IsAccessingKey>
			class Accessor
			{
				friend class HashMap;
			public:
				IteratorKV<IsAccessingKey> begin() { return IteratorKV<IsAccessingKey>(m_dict); }
				IteratorKV<IsAccessingKey> end() { return IteratorKV<IsAccessingKey>(m_dict, m_dict->m_touchedSlots + 1); }

			private:
				Accessor(const HashMapType* dict)
					: m_dict(dict) { }

				const HashMapType* m_dict;
			};

			typedef Accessor<true> KeyAccessor;
			typedef Accessor<false> ValueAccessor;


			HashMap() 
				: HashMapCore(0) { }
			explicit HashMap(int capacity)
				: HashMapCore(capacity) { }

			HashMap(std::initializer_list<KeyPairValue<T, S>> list)
				: HashMapCore((int32)list.size())
			{
				if (list.size())
				{
					for (const auto& e : list)
						Add(e.Key, e.Value);
				}
			}

			~HashMap() { }

			void Add(const T& item, const S& value) { InsertEntry(false, item, value); }
			void Add(const T& item, S&& value) { InsertEntry(false, item, std::move(value)); }
			void Add(T&& item, const S& value) { InsertEntry(false, std::move(item), value); }
			void Add(T&& item, S&& value) { InsertEntry(false, std::move(item), std::move(value)); }

			void AddOrReplace(const T& item, const S& value)
			{
				int index = FindEntry(item);
				if (index >= 0)
				{
					m_entries[index].SetValue(value);
				}
				else
				{
					InsertEntry(true, item, value);
				}
			}

			void AddOrReplace(const T& item, S&& value)
			{
				int index = FindEntry(item);
				if (index >= 0)
				{
					m_entries[index].SetValue(std::move(value));
				}
				else
				{
					InsertEntry(true, item, std::move(value));
				}
			}


			S& operator [](const T& key) const
			{
				int index = FindEntry(key);
				if (index >= 0)
				{
					return m_entries[index].getValue();
				}
				throw AP_EXCEPTION(ExceptID::KeyNotFound, Utils::ToString(key));
			}

			bool TryGetValue(const T& key, S& value) const
			{
				int index = FindEntry(key);
				if (index >= 0)
				{
					value = m_entries[index].getValue();
					return true;
				}
				return false;
			}
			S* TryGetValue(const T& key) const
			{
				int index = FindEntry(key);
				if (index >= 0)
				{
					return &m_entries[index].getValue();
				}
				return nullptr;
			}

			template <typename = typename std::enable_if<std::is_pointer<S>::value && std::is_destructible<typename std::remove_pointer<S>::type>::value>::type>
			bool RemoveAndDelete(const T& item) { return RemoveGeneric<EntryValueDeleter>(item); }

			template <typename = typename std::enable_if<std::is_pointer<S>::value && std::is_destructible<typename std::remove_pointer<S>::type>::value>::type>
			void DeleteValuesAndClear()
			{
				auto& c = *this;
				for (S& val : c.getValueAccessor())
				{
					delete val;
				}

				Clear();
			}

			void FillKeys(List<T>& list) const
			{
				if (list.getCount() == 0)
					list.ResizeDiscard(getCount());

				for (const T& key : getKeyAccessor())
					list.Add(key);
			}

			void FillValues(List<S>& list) const
			{
				if (list.getCount() == 0)
					list.ResizeDiscard(getCount());

				for (const S& val : getValueAccessor())
					list.Add(val);
			}

			//////////////////////////////////////////////////////////////////////////

			KeyAccessor getKeyAccessor() const { return KeyAccessor(this); }
			ValueAccessor getValueAccessor() const { return ValueAccessor(this); }

			Iterator begin() const { return Iterator(this); }
			Iterator end() const { return Iterator(this, m_touchedSlots + 1); }

		private:
			static void EntryValueDeleter(Entry& e)
			{
				delete e.getValue();
			}
		};

		template <typename T, typename ComparerType = Apoc3D::Collections::EqualityComparer<T>>
		class HashSet : public HashMapCore < T, Utils::HashSetEntry<T>, ComparerType >
		{
			typedef HashSet<T, ComparerType> HashSetType;			
			typedef Utils::HashSetEntry<T> Entry;

		public:
		
			class Iterator
			{
				friend class HashSet;
			public:
				const T& operator*() const
				{
					assert(m_next > 0 && m_next <= m_dict->m_touchedSlots);
					Entry& e = m_dict->m_entries[m_next - 1];
					return e.getData();
				}

				Iterator& operator++()
				{
					MoveToNext();
					return *this;
				}
				Iterator operator++(int) { Iterator result = *this; ++(*this); return result; }

				bool operator==(const Iterator& rhs) const { return m_dict == rhs.m_dict && m_next == rhs.m_next; }
				bool operator!=(const Iterator& rhs) const { return !this->operator==(rhs); }
			private:

				explicit Iterator(const HashSetType* dict)
					: m_dict(dict)
				{
					MoveToNext();
				}

				Iterator(const HashSetType* dict, int idx)
					: m_dict(dict), m_next(idx) { }


				void MoveToNext()
				{
					while (m_next < m_dict->m_touchedSlots)
					{
						if (m_dict->m_entries[m_next++].hashCode >= 0)
						{
							return;
						}
					}
					m_next = m_dict->m_touchedSlots + 1;
				}

				const HashSetType* m_dict;
				int m_next = 0;
			};


			HashSet()
				: HashMapCore(0) { }
			explicit HashSet(int capacity)
				: HashMapCore(capacity) { }

			HashSet(std::initializer_list<T> list)
				: HashMapCore((int32)list.size())
			{
				if (list.size())
				{
					for (const T& e : list) Add(e);
				}
			}

			~HashSet() { }

			void Add(const T& item) { InsertEntry(false, item); }
			void Add(T&& item) { InsertEntry(false, std::move(item)); }

			void FillItems(List<T>& list) const
			{
				if (list.getCount() == 0)
					list.ResizeDiscard(getCount());

				auto& c = *this;
				for (const T& key : c)
					list.Add(key);
			}


			//////////////////////////////////////////////////////////////////////////

			Iterator begin() const { return Iterator(this); }
			Iterator end() const { return Iterator(this, m_touchedSlots + 1); }
		
		};


	}
}

#endif