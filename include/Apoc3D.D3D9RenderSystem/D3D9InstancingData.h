#pragma once
/*
-----------------------------------------------------------------------------
This source file is part of Apoc3D Engine

Copyright (c) 2009+ Tao Xin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  if not, write to the Free Software Foundation, 
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.txt.

-----------------------------------------------------------------------------
*/
#ifndef D3D9INSTANCINGDATA_H
#define D3D9INSTANCINGDATA_H

#include "D3D9Common.h"
#include "apoc3d/Math/Math.h"
#include "apoc3d/Graphics/RenderSystem/InstancingData.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			/** The instancing is done using vertex streams by mixing a per-instance index to
			 *  the geometry vertex in vertex element TEX15.
			 *
			 *  The expected way of instancing in d3d shaders is use the TEX15 as the index
			 *  of the EPUSAGE_Trans_InstanceWorlds array to obtain the transformation,
			 *  then use this as the world transform.
			 */
			class D3D9InstancingData final : public InstancingData
			{
			public:
				D3D9InstancingData(D3D9RenderDevice* device);
				~D3D9InstancingData();

				virtual int Setup(const RenderOperation* op, int count, int beginIndex);


				IDirect3DVertexBuffer9* GetInstanceBuffer() const { return m_vertexBuffer; }
				IDirect3DVertexDeclaration9* ExpandVertexDecl(VertexDeclaration* decl);

				int getInstanceDataSize() const { return sizeof(float); }
			private:
				HashMap<VertexDeclaration*, IDirect3DVertexDeclaration9*> m_vtxDeclExpansionTable;

				/** Per-instance index stored in this vertex buffer */
				IDirect3DVertexBuffer9* m_vertexBuffer;
				D3D9RenderDevice* m_d3ddev;
			};
		}
	}
}
#endif