#pragma once
#ifndef APOC3D_RESOURCEMANAGER_H
#define APOC3D_RESOURCEMANAGER_H

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

#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Collections/List.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Core::Streaming;

namespace Apoc3D
{
	namespace Core
	{
		typedef HashMap<String, Resource*> ResHashTable;

		/**
		 *  A resource manager keeps track of certain type of resources. It does 2 jobs:
		 *    1. Ensure a resource is only loaded onces. In the case of textures, a wall texture used everywhere 
		 *		 is supposed to be loaded only once so that the memory will not be wasted.
		 *	  2. When async streaming is turn on for this resource manager, it also participate in loading
		 *		 demanding resources and unload inactive resource in background to make sure the space usage 
		 *		 is within a given size, while the front client application is minimum affected.
		 *
		 *	As being said, a resource manager can work either in async mode or sync mode. 
		 *  In sync mode the processing is done in the same thread as the rendering's. Lag may occur if the resource
		 *	processing is time consuming. And as only the first function will only be supported, automatically unloading 
		 *  inactive one to release memory spaces is not supported in this mode.
		 *
		 *  In async mode, resource can be seen as in a cache, only most recent and active ones are likely to survive
		 *  longer. While seldom used ones are more possible to be loaded on demand. 
		 */
		class APAPI ResourceManager
		{
			friend class Resource;
		public:
			typedef Apoc3D::Collections::List<ResourceManager*> ManagerList;

			/**
			 * Initializes the resource manager. 
			 * &param name  The name for the resource manager. See getName() for details.
			 * &param cacheSize  See getTotalCacheSize() and setTotalCacheSize() for details. Can be any number if not using async mode.
			 * &param useAsync  True if using async mode.
			 */
			ResourceManager(const String& name, int64 cacheSize, bool useAsync = true);
			virtual ~ResourceManager();

			/** Finalizes the resource manager. */
			void Shutdown();

			/** Make all managed resources to reload. */
			void ReloadAll();

			/**
			 *  [Only applicable when working in async mode.]
			 *  Check if no async processing task is running in the background. (i.e. check if queue is empty)
			 */
			bool IsIdle() const;
			/**
			 *  [Only applicable when working in async mode.]
			 *  Suspend the current thread until the background tasks are all finished.
			 */
			void WaitForIdle() const;
			/**
			 *  [Only applicable when working in async mode.]
			 *  Gets the number of background tasks currently.
			*/
			int GetCurrentOperationCount() const;

			void ProcessPostSync(float& timeLeft);

			/**
			 * Check if a resource identified by a string is already loaded before.
			 *
			 * @return The pointer to the resource object if it is loaded before, otherwise 0.
			 */
			Resource* Exists(const String& hashString);

			int64 CalculateTotalResourceSize() const;

			/**
			 *  Gets the name of the resource manager. 
			 *  The name usually tells what kind of resources the manager takes care of.
			 */
			const String& getName() const { return m_name; }

			/**  Check if the resource manager is working in async mode. */
			bool usesAsync() const { return m_asyncProc != nullptr; }

			/**
			 *  [Only applicable when working in async mode.]
			 *  Gets the reference limits that resources can use. 
			 */
			int64 getTotalCacheSize() const { return m_totalCacheSize; }
			/**
			 *  [Only applicable when working in async mode.]
			 *  Sets the reference limits that resources can use. 
			 *  Once the space usage is over using, the resource manager will unload the inactive resources.
			 */
			void setTotalCacheSize(int64 size) { m_totalCacheSize = size; }
			/**
			 *  [Only applicable when working in async mode.]
			 *  Gets the currently used spaces by the resources.
			 */
			int64 getUsedCacheSize() const { return m_curUsedCache; }
			
			int32 getResourceCount() const { return m_hashTable.getCount(); }

			static void PerformAllPostSync(float timelimit);
			static const ManagerList& getManagerInstances() { return s_managers; }

		protected:
			
			/**  Notifies the resource manager a new resource is created, and should be managed. */
			void NotifyNewResource(Resource* res);

			/**  Notifies the resource manager a resource is release, and should be removed from management. */
			void NotifyReleaseResource(Resource* res);

			void NotifyResourceLoaded(Resource* res);
			void NotifyResourceUnloaded(Resource* res);

		private:
			
			/**
			 *  [Only applicable when working in async mode.]
			 *  If a resource is IsIndependent(), this cancels(or removes) the corresponding opposite resource operation
			 *  from the task queue. Say a load ResourceOperation can cancel an unload ResourceOperation for the same resources
			 *  if the unload ResourceOperation is yet to be processed.
			 * 
			 * &return True if successfully canceled.
			 */
			bool NeutralizeTask(const ResourceOperation& op) const;

			/**
			 *  [Only applicable when working in async mode.]
			 *  Adds a task represented by a ResourceOperation object to the task queue, waiting to be processed.
			 */
			void AddTask(const ResourceOperation& op) const;

			/**
			 *  [Only applicable when working in async mode.]
			 *  Remove a task from the task queue, if the it is not the operation's turn in the background.
			*/
			void RemoveTask(const ResourceOperation& op) const;

			void RemoveTask(Resource* res) const;

			void CheckAsync() const; 

			ResHashTable m_hashTable;
			
			int64 m_totalCacheSize;
			int64 m_curUsedCache;

			GenerationTable* m_generationTable;
			AsyncProcessor* m_asyncProc;

			String m_name;

			bool m_isShutDown;

			
			static ManagerList s_managers;
		};
	}
}

#endif