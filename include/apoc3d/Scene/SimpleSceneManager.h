#pragma once
#ifndef APOC3D_SIMPLESCENEMANAGER_H
#define APOC3D_SIMPLESCENEMANAGER_H

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

using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Scene
	{
		class APAPI SimpleSceneNode : public SceneNode
		{
		public:
			SimpleSceneNode(){}
			~SimpleSceneNode(){}

			virtual void AddObject(SceneObject* sceObj);
			virtual void RemoveObject(SceneObject* sceObj);
		};

		class APAPI SimpleSceneManager : public SceneManager
		{
		private:
			SimpleSceneNode* m_defaultNode;

		public:
			SimpleSceneManager();
			~SimpleSceneManager();

			virtual void AddObject(SceneObject* const obj);
			virtual bool RemoveObject(SceneObject* const obj);

			virtual void PrepareVisibleObjects(Camera* camera, BatchData* batchData);
			virtual SceneObject* FindObject(const Ray& ray, IObjectFilter* filter);
		};
	}
}
#endif