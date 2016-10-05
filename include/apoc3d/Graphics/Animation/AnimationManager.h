#pragma once
#ifndef APOC3D_ANIMATIONMANAGER_H
#define APOC3D_ANIMATIONMANAGER_H

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
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace Animation
		{
			typedef HashMap<String, AnimationData*> AnimHashTable;

			/** 
			 *  Animation is only a helper class to make sure animation data is not loaded multiple times.
			 *
			 *  Unlike other real resource management classes, this does not provide features like streaming.
			 *
			 *  All created animation data by this class can not be deleted. 
			 *  They will be deleted by the manager when the manager is shut down. 
			 *  Actually, user can load AnimationData manually if this AnimationManager is felt tricky.
			 *  locating a file and call Animation::Load will do the job, only few lines of code.
			 */
			class APAPI AnimationManager
			{
				SINGLETON_DECL(AnimationManager);
			public:
				AnimationManager();
				~AnimationManager();

				const AnimationData* CreateInstance(const ResourceLocation& rl);

			private:
				AnimHashTable m_hashTable;

			};
		}
	}
}

#endif