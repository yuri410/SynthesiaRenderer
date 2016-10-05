#pragma once
#ifndef APOC3D_SCENERENDERER_H
#define APOC3D_SCENERENDERER_H

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
#include "apoc3d/Collections/Queue.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Config;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Scene
	{
		typedef List<RenderOperation> OperationList;
		typedef HashMap<GeometryData*, OperationList*> GeometryTable;
		typedef HashMap<Material*, GeometryTable*> MaterialTable;
		typedef HashMap<uint32, MaterialTable*> PriorityTable;

		/**
		 *  A pool used to quick obtain tables and lists for storing render operations
		 *  This also contain other preallocated buffer used at render time.
		 *
		 */
		class APAPI BatchDataBufferCache
		{
		public:
			typedef List<GeometryData*> InvalidGeoPointerList;
			typedef List<Material*> InvalidMtrlPointerList;

			BatchDataBufferCache(int32 opListCount, int32 geoTableCount, int32 mtrlTableCount,
				int32 minGeoTableSize, int32 minMtrlTableSize, int32 minOpListSize);
			~BatchDataBufferCache();

			OperationList* ObtainNewOperationList();
			GeometryTable* ObtainNewGeometryTable();
			MaterialTable* ObtainNewMaterialTable();

			void RecycleOperationList(OperationList* obj);
			void RecycleGeometryTable(GeometryTable* obj);
			void RecycleMaterialTable(MaterialTable* obj);

			void Reserve(int32 opListCount, int32 geoTableCount, int32 mtrlTableCount,
				int32 minGeoTableSize, int32 minMtrlTableSize, int32 minOpListSize);

			InvalidMtrlPointerList* getInvalidMtrlPointerBuffer() const { return m_invalidMtrlPointers; }
			InvalidGeoPointerList* getInvalidGeoPointerBuffer() const { return m_invalidGeoPointers; }
		private:
			Queue<OperationList*> m_retiredOpList;
			Queue<GeometryTable*> m_retiredGeoTable;
			Queue<MaterialTable*> m_retiredMtrlTable;

			int32 m_minGeoTableSize;
			int32 m_minMtrlTableSize;
			int32 m_minOpListSize;

			InvalidMtrlPointerList* m_invalidMtrlPointers;
			InvalidGeoPointerList* m_invalidGeoPointers;
		};

		/**
		 *  A hirerachy of tables to store classified render operations.
		 *
		 *  Before rendering with graphics API. Visible objects are detected and their render operations will
		 *  be inserted into this sheet, grouped according to their sources, materials, priorities..
		 *  The classification is done by hashing; similar render op will be 
		 *  grouped together. This is good for minimizing render state changes if grouped 
		 *  render operations are drawn one time. 
		 *  Also instancing is automatic as long as the shader effect supports.
		 */
		class APAPI BatchData
		{
		public:
			static const uint MaxPriority = 32;

		public:

			BatchData();
			~BatchData();

			
			/**
			 *  Renders this batch data
			 */
			void RenderBatch(RenderDevice* device, int selectorID);

			/**
			 *  Adds an object's render operations into the internal table.
			 */
			void AddVisisbleObject(SceneObject* obj, int level);
			void AddRenderOperation(const RenderOperationBuffer& ops);

			/**
			 *  Check if any materials with passFlag corresponding to selectMask are already added
			 *  into the table.
			 */
			bool HasObject(uint64 selectMask);

			/**
			 *  Clears all the render operations added.
			 */
			void Clear();

			/**
			 *  Gets a reference to the internal table
			 */
			const PriorityTable& getTable() const { return m_priTable; }

			/**
			 *  Gets the total number of objects added since last Clear
			 */
			int getObjectCount() const { return m_objectCount; }

			void Reset();

			BatchDataBufferCache& getBufferCache() { return m_bufferCache; }
		private:
			PriorityTable m_priTable;
			int m_objectCount;

			BatchDataBufferCache m_bufferCache;

		};

		/**
		 *  Renders a scene with a particular render script.
		 * 
		 * @remarks
		 *  For the script listing config, please refer to the sample file.
		 */
		class APAPI SceneRenderer
		{
		public:
			SceneRenderer(RenderDevice* dev);
			~SceneRenderer();

			/**
			 *  The scene renderer loads from a config which lists several render script (SceneProcedure) files.
			 *  Each render script (SceneProcedure xml) will be evaluated for compatibility.
			 *  So the renderer can choose a valid one. 
			 *  The input list is expected to be sorted form high ended SceneProcedures
			 *  to low ended ones. A fall back will be performed if the prior ones are 
			 *  not supported by the hardware.
			 *  SceneRenderer will eventually select one SceneProcedure. And if the hardware
			 *  does not support any of the SceneProcedures, nothing will be selected; scene will not
			 *  be drawn in this case. In the future, a explanation for reason will be displayed on the expected
			 *  RenderTarget.
			 */
			void Load(Configuration* config);
			void Load(const String& configName);

			void RegisterCamera(Camera* camera)
			{
				m_cameraList.Add(camera);
			}
			void UnregisterCamera(Camera* camera)
			{
				m_cameraList.Remove(camera);
			}

			/**
			 *  Begin the whole rendering process. The client application should use this method to
			 *  draw the scene.
			 */
			void RenderScene(SceneManager* sceMgr);

			/** 
			 *  Renders the current batch data produced by instructions in the scene pass.
			 */
			void RenderBatch(int selectorID);

			
			/**
			 *  Gets the index of selected SceneProcedure.
			 *  If no SceneProcedure is selected, returns -1.
			 */
			int getSelectedProcID() const { return m_selectedProc; }
			/**
			 *  Gets the selected SceneProcedure.
			 *  If no SceneProcedure is selected, returns 0.
			 */
			SceneProcedure* getSelectedProc() const
			{
				if (m_selectedProc!=-1)
					return m_procFallbacks[m_selectedProc];
				return 0;
			}

			const BatchData& getBatchData() const { return m_batchData; }

			void ResetBatchTable();

			int GlobalCameraOverride;

		private:
			RenderDevice* m_renderDevice;
			//List<ScenePass*> m_passes;
			BatchData m_batchData;

			List<SceneProcedure*> m_procFallbacks;
			int m_selectedProc;

			List<Camera*> m_cameraList;
		};
	};
};
#endif