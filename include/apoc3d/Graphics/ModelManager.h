#pragma once
#ifndef APOC3D_MODELMANAGER_H
#define APOC3D_MODELMANAGER_H

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

#include "apoc3d/Core/ResourceManager.h"

using namespace Apoc3D::Core;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Graphics::Animation;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Graphics
	{
		/** 
		 *  The resource manager for all ModelSharedData.
		 *
		 *  In addition, this class can also create unmanaged resource. 
		 *  CreateUnmanagedInstance directly loads a model; then return it. Instances created in this
		 *  way need to be taken care by the client code. Deleting is required when no longer used.
		 *
		 */
		class APAPI ModelManager : public ResourceManager
		{
			SINGLETON_DECL(ModelManager);
		public:
			static int64 CacheSize;
			static bool UseCache;

			ModelManager();
			~ModelManager();

			ModelSharedData* CreateInstanceUnmanaged(RenderDevice* renderDevice, const ResourceLocation& rl);
			ResourceHandle<ModelSharedData>* CreateInstance(RenderDevice* renderDevice, const ResourceLocation& rl);
			
		};
	}
}

#endif