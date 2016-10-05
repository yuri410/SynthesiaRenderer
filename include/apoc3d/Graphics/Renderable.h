#pragma once
#ifndef APOC3D_IRENDERABLE_H
#define APOC3D_IRENDERABLE_H

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

namespace Apoc3D
{
	namespace Graphics
	{
		/** Represents drawable object in the scene. */
		class APAPI Renderable
		{
		public:
			/** Gets the render operation of this renderable object at a ceratin LOD level */
			virtual RenderOperationBuffer* GetRenderOperation(int level) = 0;

			/** Gets the render operation of this renderable object at the default LOD level */
			virtual RenderOperationBuffer* GetRenderOperation() { return GetRenderOperation(0); }

		protected:
			Renderable()
			{
			}

			virtual ~Renderable()
			{
			}
		};
	};
};
#endif