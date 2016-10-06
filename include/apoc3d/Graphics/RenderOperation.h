#pragma once
#ifndef APOC3D_RENDEROPERATION_H
#define APOC3D_RENDEROPERATION_H

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

#include "apoc3d/Math/Matrix.h"

using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{
		/** The current-frame transformations for all bones/objects in a sequential order.  */
		struct APAPI PartTransforms
		{
			const Matrix* Transfroms = nullptr;
			int32 Count = 0;
			
			bool operator ==(const PartTransforms& another) const;
			bool operator !=(const PartTransforms& other) const;
		};

		/**
		 *  Represents an operation to render a mesh part in the scene.
		 *
		 *  Usually, transformations data, material and GeometryData including buffers and declarations are part of 
		 *  this operation. Each operation is in equivalent to a DrawPrimitive or DrawIndexedPrimitive call in D3D when not
		 *  using instancing.
		 *
		 *  RenderOperation is used by the engine to manage the scene rendering pipeline.
		 */
		class APAPI RenderOperation
		{
		public:
			GeometryData* GeometryData = nullptr;
			Material* Material = nullptr;
			Matrix RootTransform;
			PartTransforms PartTransform;
			void* UserData = nullptr;

			/** Let the renderer use RootTransform as the final transformation, not multiplying obj's transform */
			bool RootTransformIsFinal = false;

			bool operator ==(const RenderOperation& other);
			bool operator !=(const RenderOperation& other);
		};
	};
};
#endif