#pragma once
#ifndef APOC3D_RESOURCE_H
#define APOC3D_RESOURCE_H

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

#include "apoc3d/Common.h"

#include "Streaming/AsyncProcessor.h"

#include "apoc3d/Collections/Queue.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Core::Streaming;

namespace Apoc3D
{
	namespace Core
	{
		/**
		 *  Defines some states used when a resource is being processed.
		 *
		 *  When async streaming is turn on in the corresponding resource manager, 
		 *	RS_PendingLoad and RS_PendingUnload may occur. Otherwise, the state will only
		 *	be one of the first four.
		 */
		enum struct ResourceState
		{
			/** 
			 *  Represents the resource is already unloaded or not loaded yet. 
			 */
			Unloaded = 0,
			/**
			 *  Represents the resource is already loaded.
			 */
			Loaded = 1,
			/**
			 *  Represents the resource is currently being loaded 
			 */
			Loading = 2,			
			/**
			 *  Represents the resource is currently being unloaded 
			 */
			Unloading = 3
		};
		
		/**
		 *  A resource is data or operation that uses hardware provided resources, which is limited.
		 *  Typical example for resources are textures and models. Both uses video card memory.
		 *  
		 *  A resource controlled by a resource manager is a managed resource. See resource manager for details.
		 *  A resource can be a unmanaged one. In this case, the loading/unloading is all up to the client code.
		 *  And no other management or background work is done for it.
		 *
		 *  Resources are identified by a 'hashString' which uniquely represents each of them.
		 */
		class APAPI Resource
		{
			friend class GenerationTable;
		public: 
			
			virtual ~Resource();
			
			/** 
			 *  Get the generation number.
			 *  Only returns valid if the resource is managed and async.
			 */
			int GetGeneration() const 
			{
				if (m_generation)
					return m_generation->Generation;
				return -1;
			}
			int getReferenceCount() const { return m_refCount; }

			/** 
			 *  [ASync resource only]
			 *  Tells if the resource can be unloaded when inactive.
			 */
			virtual bool IsUnloadable();

			/** Get the memory token by the resource in bytes. */
			virtual uint32 getSize() = 0;

			/**
			 *  [Managed resource only]
			 *  Touches the resource. 
			 *  If the resource is not loaded yet, this will cause the resource to load.
			 *  This is not necessary called since the resource handle will do the work unless
			 *  special requirements.
			 */
			void Use();
			void UseSync();

			/** 
			 *  Makes this resource not unloadable by the resource collector
			 */
			void Lock_Unloadable();
			/**
			 *  Makes this resource unloadable by the resource collector
			 */
			void Unlock_Unloadable();


			/**
			 *  Load the resource
			 */
			void Load();
			/**
			 *  Unload the resource
			 */
			void Unload();

			/**
			 *  Reload the resource if it is already loaded.
			 */
			void Reload();

			/** Gets a string uniquely represents the resource */
			const String& getHashString() const { return m_hashString; }

			/** Check if the resource's state is RS_Loaded. */
			bool isLoaded() { return getState() == ResourceState::Loaded; }

			/** Gets the resource's current state. */
			ResourceState getState() const;

			bool isIndependent() const { return m_isIndependent; }
			bool isPostSyncNeeded() const { return m_requiresPostSync; }

			/**
			 *  Check if the resource is managed.
			 */
			bool isManaged() const { return m_manager != nullptr; }

			/**
			 *  Gets the corresponding resource manager. 
			 *  &return The pointer to the resource manager object, nullptr if the resource is not managed.
			 */
			ResourceManager* getManager() const { return m_manager; }

			void _Ref()
			{
				assert(isManaged());
				m_refCount++;
			}
			void _Unref()
			{
				assert(isManaged());
				m_refCount--;
			}

			static void ProcessResourceOperation(const ResourceOperation& resOp);
			static void ProcessResourceOperationPostSyc(const ResourceOperation& resOp, int32 percentage);
		protected:
			
			/** Create a unmanaged resource */
			Resource();

			/** If manager is not nullptr, creates a managed resource. */
			Resource(ResourceManager* manager, const String& hashString);

			/** implement load processing here */
			virtual void load() = 0;

			/** implement unload processing here */
			virtual void unload() = 0;

			virtual void loadPostSync(int32 percentage) { load(); }
			virtual void unloadPostSync(int32 percentage) { unload(); }
			
		private:
			Resource& operator=(const Resource &rhs) = delete;

			ResourceOperation GetLoadOperation() { return ResourceOperation(this, ResourceOperation::RESOP_Load); }
			ResourceOperation GetUnloadOperation() { return ResourceOperation(this, ResourceOperation::RESOP_Unload); }

			void LoadSync();

			void setState(ResourceState st);

			/** A utility used for calculating generation number for resources based on the visit history. */
			class GenerationCalculator
			{
			public:
				GenerationCalculator(const GenerationTable* table);
				~GenerationCalculator();

				void Use(Resource* resource);
				void UpdateGeneration(float time);

				bool IsGenerationOutOfTime(float time);
			public:
				volatile int Generation;
			private:
				const GenerationTable* m_table;
				Queue<float> m_timeQueue;

				tthread::mutex* m_queueLock;
			};


			GenerationCalculator* m_generation = nullptr;
			ResourceManager* m_manager = nullptr;

			const String m_hashString;

			int m_refCount = 0;
			

			volatile ResourceState m_state = ResourceState::Unloaded;

			tthread::mutex* m_lock = nullptr;

			bool m_unloadableLock = false;
			bool m_requiresPostSync = false;
			bool m_isIndependent = true;

		};
	};
};
#endif