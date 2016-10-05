#pragma once

#ifndef APOC3D_ASYNCPROCESSOR_H
#define APOC3D_ASYNCPROCESSOR_H

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
#include "apoc3d/Collections/Queue.h"

using namespace Apoc3D::Collections;


namespace Apoc3D
{
	namespace Core
	{
		namespace Streaming
		{
			/**
			 *  Represents an operation that processes resources.
			 */
			struct ResourceOperation
			{
				enum OperationType
				{
					RESOP_Load,
					RESOP_Unload
				} Type = RESOP_Load;

				Resource* Subject = nullptr;

				ResourceOperation() { }
				ResourceOperation(Resource* res, OperationType type)
					: Subject(res), Type(type) { }

				void Invalidate() { Subject = nullptr; }
				bool isValid() const { return Subject != nullptr; }

				bool operator ==(const ResourceOperation& rhs) const { return Type == rhs.Type && Subject == rhs.Subject; }
				bool operator !=(const ResourceOperation& rhs) const { return !this->operator==(rhs); }
			};

			/**
			 *  This is used to initialize a thread for resource collection and processing.
			 *  At the same time it keeps track of current ResourceOperations and process queued ones in background .
			 */
			class APAPI AsyncProcessor
			{
			public:
				AsyncProcessor(GenerationTable* gTable, const String& name, bool isThreaded);
				~AsyncProcessor(void);

				/**
				 *  If a resource is IsIndependent(), this cancels(or removes) the corresponding opposite resource operation
				 *  from the queue.
				 */
				bool NeutralizeTask(const ResourceOperation& op);

				/**
				 *  Adds a ResourceOperation object to the queue.
				 */
				void AddTask(const ResourceOperation& op);
				/**
				 *  Removes a ResourceOperation object from the queue.
				 */
				void RemoveTask(const ResourceOperation& op);

				void RemoveTask(Resource* res);

				/**
				 *  Check if there is no queued ResourceOperations at the moment.
				 */
				bool TaskCompleted();
				/**
				 *  Gets the current number of queued ResourceOperations.
				 */
				int GetOperationCount();

				/**
				 *  Suspends the caller's thread until all queued ResourceOperations are processed.
				 */
				void WaitForCompletion();
				
				/**
				 *  Shuts down the AsyncProcessor. Terminating the background thread.
				 */
				void Shutdown();


				void ProcessPostSync(float& timeLeft);


			private:
				void LockQueue();
				void UnlockQueue();

				bool ClearMatchingResourceOperation(Resource* res, ResourceOperation::OperationType type);


				static void ThreadEntry(void* arg);
				void Main();

				Queue<ResourceOperation> m_opQueue;
				GenerationTable* m_genTable = nullptr;
				tthread::thread* m_processThread = nullptr;
				tthread::mutex* m_queueMutex = nullptr;

				Queue<ResourceOperation> m_postSyncQueue;

				bool m_closed = false;

			};
		}
	}
}
#endif