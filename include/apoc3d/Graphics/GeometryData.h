#pragma once
#ifndef APOC3D_GEOMETRYDATA_H
#define APOC3D_GEOMETRYDATA_H

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

#include "GraphicsCommon.h"

using namespace Apoc3D::Core;
using namespace Apoc3D::Scene;

namespace Apoc3D
{
	namespace Graphics
	{
		/** Define buffers and information used to render a geometry */
		class APAPI GeometryData
		{
		public:
			RenderSystem::VertexBuffer* VertexBuffer = nullptr;			/** the vertex buffer used to draw the geometry */
			RenderSystem::IndexBuffer* IndexBuffer = nullptr;				/** the index buffer used to draw the geometry */
			RenderSystem::VertexDeclaration* VertexDecl = nullptr;		/** the vertex declaration for vertex in the geometry */

			int32 BaseVertex = 0;							/** The starting vertex offset in the vertex buffer */
			int32 StartIndex = 0;

			int32 PrimitiveCount = 0;
			RenderSystem::PrimitiveType PrimitiveType = RenderSystem::PT_PointList;

			int32 VertexCount = 0;
			int32 VertexSize = 0;

			int32 UsedVertexRangeStart = 0;
			int32 UsedVertexRangeCount = 0;

			void* UserData = nullptr;

			bool Discard = false;

			bool usesIndex() const { return IndexBuffer != nullptr; }

			GeometryData()
			{ }
			~GeometryData() { }

			void Setup(RenderSystem::VertexBuffer* vb, RenderSystem::IndexBuffer* ib, RenderSystem::VertexDeclaration* decl,
				RenderSystem::PrimitiveType pt);
			
		};
	};
};
#endif