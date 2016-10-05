#pragma once

#ifndef APOC3D_GENERATIONTABLE_H
#define APOC3D_GENERATIONTABLE_H

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
#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/HashMap.h"

using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;

namespace Apoc3D
{
	namespace Core
	{
		namespace Streaming
		{
			/**
			 *  A GenerationTable puts resources with different gen numbers into different tables,
			 *  and works on resource collecting and gen number updating based on the them.
			 */
			class APAPI GenerationTable
			{
			public:
				static const int32 MaxGeneration = 4;

				static const float GenerationLifeTime[];

			public:

				GenerationTable(ResourceManager* mgr);
				~GenerationTable();


				void SubTask_GenUpdate();
				void SubTask_Collect();


				//ExistTable<Resource*>& getGeneration(int index) const { return m_generations[index]; }

				void AddResource(Resource* res);
				void RemoveResource(Resource* res);

				/**
				 *  Remove the resource from the table with oldGeneration number in m_generations, and add to the new table of newGeneration.
				 */
				void UpdateGeneration(int oldGeneration, int newGeneration, Resource* resource);

				void ShutDown() 
				{
					m_isShutdown = true;

					//m_thread->join();
				}

			private:
				/**
				 *  An array of tables represents that resources with gen i are in i+1 th table.
				 */
				HashSet<Resource*>* m_generations;
				/**
				 *  A list of all pointers to resource objects.
				 */
				List<Resource*> m_generationList;
				
				ResourceManager* m_manager;

				bool m_isShutdown;
				tthread::mutex* m_genLock;

				bool CanUnload(Resource* res) const;
			};
		}
		
	}
}

#endif