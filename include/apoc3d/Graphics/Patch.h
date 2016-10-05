#pragma once
#ifndef APOC3D_PATCH_H
#define APOC3D_PATCH_H

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

#include "Renderable.h"
#include "RenderSystem/HardwareBuffer.h"
#include "RenderSystem/VertexElement.h"
#include "RenderOperationBuffer.h"

#include "apoc3d/Math/Matrix.h"

using namespace Apoc3D::Math;
using namespace Apoc3D::Collections;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		class APAPI Patch : public Renderable
		{
		public:
			Patch(RenderDevice* device, const void* vertexData, int vertexCount, const List<VertexElement>& vtxElems);
			~Patch();

			virtual RenderOperationBuffer* GetRenderOperation(int lod);

			void setMaterial(Material* mtrl)
			{
				m_mtrl = mtrl;
			}
			void setTransform(const Matrix& trans) { m_transfrom = trans; }
		private:
			VertexBuffer* m_vertexBuffer;
			IndexBuffer* m_indexBuffer;
			VertexDeclaration* m_vertexDecl;

			int m_vertexSize;
			int m_vertexCount;
			int m_primitiveCount;

			Matrix m_transfrom;
			Material* m_mtrl = nullptr;
			GeometryData* m_geoData = nullptr;
			RenderOperationBuffer m_opBuffer;

		};
	}
}

#endif