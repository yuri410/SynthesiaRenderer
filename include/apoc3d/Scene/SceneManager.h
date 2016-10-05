#pragma once
#ifndef APOC3D_SCENEMANAGER_H
#define APOC3D_SCENEMANAGER_H

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

#include "apoc3d/Collections/List.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Scene
	{
		/** 
		 *  A callback interface used for ray-based selection check.
		 */
		class APAPI IObjectFilter
		{
		public:
			/**
			 *  Check if an intersected object should be accepted
			 */
			virtual bool Check(SceneObject* obj) = 0;
			virtual bool Check(SceneNode* node) { return true; }
		};


		/**
		 *  SceneManager keeps tracks of all scene objects.
		 */
		class APAPI SceneManager
		{
		public:
			SceneManager();
			virtual ~SceneManager();
		
			/**
			 *  Adds a new scene object into scene
			 */
			virtual void AddObject(SceneObject* const obj);
			/**
			 *  Removes a scene object from scene
			 */
			virtual bool RemoveObject(SceneObject* const obj);

			virtual void PrepareVisibleObjects(Camera* camera, BatchData* batchData) = 0;

			virtual void Update(const GameTime* time);

			virtual SceneObject* FindObject(const Ray& ray, IObjectFilter* filter) = 0;

			const List<SceneObject*>& getAllObjects() const { return m_objects; }
		private:
			List<SceneObject*> m_objects;
		protected:

		};
	};
};

#endif