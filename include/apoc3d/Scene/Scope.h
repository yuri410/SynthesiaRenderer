#pragma once
#ifndef CMN_SCOPE_H
#define CMN_SCOPE_H

#include "Common.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "RTTI.h"

namespace os
{
	/**
	 *  Represents a table of key-value pairs where values can be sub tables.
	 */
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		Scope();
		explicit Scope(uint32 capacity);
		Scope(const Scope& scope);
		virtual ~Scope();

		Scope& operator=(const Scope& rhs);

		/**
		 *  Find and the Datum associated with the given name.
		 *  @return the Datum associated with the given name if it exists. nullptr otherwise.
		 */
		Datum* Find(const String& name);

		/**
		 *  Recursively find the Datum associated with the given name from the most-closely nested to the farther ones.
		 *  @param foundContainer The Scope which contains the result
		 *  @returns the Datum found otherwise nullptr.
		 */
		Datum* Search(const String& name, Scope** foundContainer = nullptr);

		/**
		 *  Create a Datum with the given name associated if it does not exist. Otherwise
		 *  the pre-existing one will be used.
		 *  @returns the created or existing Datum.
		 */
		Datum& Append(const String& name);

		/**
		 *  Create a Scope with the given name associated if it does not exist. Otherwise
		 *  the pre-existing one will be used.
		 *  @param datumIndex the index in the Datum the Scope will be stored at.
		 *  @return the scope with the associated name.
		 */
		Scope& AppendScope(const String& name, uint32 datumIndex = 0);

		/**
		 *  Place the scope child with the given name at the given index.
		 */
		void Adopt(Scope* child, const String& name, uint32 datumIndex);

		/**
		 *  Find the name of the given child.
		 */
		bool FindName(const Scope* child, String& name) const;

		/**
		 *  Clear the subtree
		 */
		void Clear();

		/** 
		 *  Returns the parent scope
		 */
		Scope* GetParent() { return m_parent; }

		/**
		 *  Get the number of items at the current level of this scope
		 */
		uint32 Size() const { return m_indexer.Size(); }

		Datum& operator [](const String& name) { return Append(name); }
		Datum& operator [](uint32 index);

		void FillNames(Vector<String>& nameList);

		bool operator==(Scope& rhs) const;
		bool operator!=(Scope& rhs) const { return !(*this == rhs); }
	private:
		typedef HashMap<String, Datum*> LookupTable;

		typedef SList<std::pair<String, Datum>> StorageList;
		
		Datum& AddDatum(const String& name);

		/** 
		 *  Disconnect the the current scope from the given child
		 */
		void Orphan(Scope* ch);

		static bool CompareScopeDatum(Datum& lhs, Datum& rhs);
		static uint32 GetActuallDatumScopeCount(Datum& d);

		static const uint32 DefaultCapacity = 10;


		Scope* m_parent;

		LookupTable m_lookupTable;
		StorageList m_storage;
		Vector<StorageList::Iterator> m_indexer;
		
	};
}

#endif