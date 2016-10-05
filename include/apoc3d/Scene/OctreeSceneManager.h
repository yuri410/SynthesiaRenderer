#pragma once
#ifndef APOC3D_OCTREESCENEMANAGER_H
#define APOC3D_OCTREESCENEMANAGER_H

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


#include "SceneManager.h"
#include "SceneNode.h"

#include "apoc3d/Collections/LinkedList.h"
#include "apoc3d/Collections/Queue.h"
#include "apoc3d/Math/OctreeBox.h"
#include "apoc3d/Math/BoundingSphere.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Scene
	{
		class APAPI OctreeSceneNode : public SceneNode
		{
		public:
			/*
			000 = OCTE_NxNyNz
			001 = OCTE_NxNyPz,
			010 = OCTE_NzPyNz,
			011 = OCTE_NxPyPz,
			100 = OCTE_PxNyNz,
			101 = OCTE_PxNyPz,
			110 = OCTE_PxPyNz,
			111 = OCTE_PxPyPz,
			*/
			enum Extend
			{
				OCTE_NxNyNz=0,
				OCTE_NxNyPz,
				OCTE_NzPyNz,
				OCTE_NxPyPz,
				OCTE_PxNyNz,
				OCTE_PxNyPz,
				OCTE_PxPyNz,
				OCTE_PxPyPz,
				OCTE_Count = 8
			};



			OctreeSceneNode(OctreeSceneManager* mgr, OctreeSceneNode* parent, const OctreeBox& volume);
			~OctreeSceneNode();

			virtual void AddObject(SceneObject* sceObj);
			virtual void RemoveObject(SceneObject* sceObj);

			

			Vector3 GetMax() const
			{
				Vector3 ext = Vector3::Set(m_boundingVolume.Length*0.5f);
				return m_boundingVolume.Center +  ext;
			}
			Vector3 GetMin() const
			{
				Vector3 ext = Vector3::Set(m_boundingVolume.Length*-0.5f);
				return m_boundingVolume.Center + ext;
			}

			const BoundingSphere& getBoundingSphere() const { return m_boundingSphere; }
			OctreeSceneNode* getNode(Extend e) const { return m_childTable[e]; }
			
		private:
			
			bool RemoveAttachedObject(SceneObject* obj);
			bool RemoveObjectInternal(SceneObject* obj);

			OctreeSceneManager* m_manager;

			OctreeSceneNode* m_parent;
			OctreeSceneNode* m_childTable[8];
			OctreeBox m_boundingVolume;
			BoundingSphere m_boundingSphere;

			Extend GetExtend(const Vector3& pos) const;
			static Vector3 OffsetVectorTable[8];
		};

		class APAPI OctreeSceneManager : public SceneManager
		{
		public:
			OctreeSceneManager(const OctreeBox& range, float minBVSize);
			~OctreeSceneManager();

			virtual void AddObject(SceneObject* const obj);
			virtual bool RemoveObject(SceneObject* const obj);

			virtual void PrepareVisibleObjects(Camera* camera, BatchData* batchData);

			virtual void Update(const GameTime* time);

			virtual SceneObject* FindObject(const Ray& ray, IObjectFilter* filter);


			float getMinimumBVSize() const { return m_minimumBVSize; }

			bool QualifiesFarObject(const SceneObject* obj) const;

		private:
			LinkedList<SceneObject*> m_dynObjs;
			LinkedList<SceneObject*> m_farObjs;

			Queue<OctreeSceneNode*> m_bfsQueue;

			OctreeBox m_range;
			Vector3 m_min;
			Vector3 m_max;

			float m_minimumBVSize;

			OctreeSceneNode* m_octRootNode;

			void AddStaticObject(SceneObject* obj);

		};
	}
}
#endif