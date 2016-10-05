#pragma once
#ifndef APOC3D_MESH_H
#define APOC3D_MESH_H

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


#include "ModelTypes.h"
#include "RenderOperationBuffer.h"
#include "Renderable.h"
#include "RenderSystem/VertexElement.h"
#include "apoc3d/Collections/CollectionsCommon.h"
#include "apoc3d/Math/BoundingSphere.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace Graphics
	{
		/** 
		 *  The geometry data including vertices as well as indices and materials.
		 *
		 *  Every mesh has several materials. These materials cut the mesh into
		 *  sub parts, where each part has only one material.
		 */
		class APAPI Mesh : public Renderable
		{
			static Material*& GetItem(const Mesh* m, int32 i, int32 j) { return m->m_subParts[i].MaterialFrames[j]; }
			static int32 GetCountI(const Mesh* m) { return m->getPartCount(); }
			static int32 GetCountJ(const Mesh* m, int32 i) { return m->getMaterialFrames(i).getCount(); }

		public:

			struct MaterialIterator
			{
			public:
				using Iterator = Apoc3D::Collections::Iterator2D < Material*, Mesh, GetItem, GetCountI, GetCountJ > ;

				MaterialIterator(const Mesh* m) : m_mesh(m) { }

				Iterator begin() const { return Iterator(m_mesh); }
				Iterator end() const { return Iterator(m_mesh, -1, -1); }

			private: 
				const Mesh* m_mesh;
			};

			Mesh(RenderDevice* device, const MeshData* data);
			~Mesh();

			/** Copies the indices to a given buffer. */
			void GetIndices(uint* dest) const;

			virtual RenderOperationBuffer* GetRenderOperation(int level);

			void Save(MeshData* data);

			/** Passes triangle primitives one by one, while call the callback for each one. */
			void ProcessAllTriangles(IMeshTriangleCallBack* callback) const;

			int32 CalculateSizeInBytes() const;


			VertexBuffer* getVertexBuffer() const { return m_vertexBuffer; }
			VertexDeclaration* getVertexDecl() const { return m_vtxDecl; }
			//const List<IndexBuffer*>& getIndexBuffers() const { return m_indexBuffers; }

			//const List<VertexElement>& getVertexElement() const { return m_vertexElements; }
			//const int32* getPartPrimitiveCount() const { return m_partPrimitiveCount; }
			//const int32* getPartVertexCount() const { return m_partVertexCount; } 
			int32 getPartCount() const { return m_subParts.getCount(); }

			int32 getPartPrimitiveCount(int32 partIdx) const { return m_subParts[partIdx].PrimitiveCount; }
			int32 getPartVertexCount(int32 partIdx) const { return m_subParts[partIdx].VertexCount; }

			IndexBuffer* getIndexBuffer(int32 partIdx) const { return m_subParts[partIdx].Indices; }
			List<Material*>& getMaterialFrames(int32 partIdx) const { return m_subParts[partIdx].MaterialFrames; }
			//int32 getMaterialFramesCount(int32 partIdx) const { return m_subParts[partIdx].MaterialFrames.getCount(); }
			Material* getMaterial(int32 partIdx, int32 frameIdx) const { return m_subParts[partIdx].MaterialFrames[frameIdx]; }

			MaterialIterator getMaterials() const { return MaterialIterator(this); }

			/** 
			 *  A set of arrays of materials.
			 *  Each array in the set is corresponding to a sub mesh. each material as the array element is a 
			 *  keyframe material if material animation is applicable.
			 *
			 *  When not using any material animation, this should be a set of arrays with a length of 1.
			 *  That is in equivalent to a set of Material.
			 */
			//MeshMaterialSet<Material*>* getMaterials() { return &m_materials; }

			int32 getVertexSize() const { return m_vertexSize; }
			int32 getVertexCount() const { return m_vertexCount; }
			int32 getPrimitiveConut() const { return m_primitiveCount; }
			int32 getIndexCount() const { return m_primitiveCount * 3; }

			const String& getName() const { return m_name; }
			RenderDevice* getRenderDevice() const { return m_renderDevice; }


			void setName(const String& value) { m_name = value; }

			const BoundingSphere& getBoundingSphere() const { return m_boundingSphere; }
			void setBoundingSphere(const BoundingSphere& sphere) { m_boundingSphere = sphere; }

		private:
			// a sub part(sub mesh) divided by materials
			struct SubPart
			{
				List<Material*> MaterialFrames;
				IndexBuffer* Indices = nullptr;

				int32 PrimitiveCount = 0;
				int32 VertexCount = 0;

				int32 VertexRangeUsedStart = 0;
				int32 VertexRangeUsedCount = 0;

				SubPart(const SubPart& o) = delete;
				SubPart& operator=(const SubPart& o) = delete;

				SubPart(SubPart&& o);
				SubPart& operator=(SubPart&& o);
				SubPart(){}
				~SubPart();

				template <typename T, IndexBufferType IBT>
				void SetIndexData(ObjectFactory* fac, const List<uint>& pi, int32 vertexCount, bool* used);
			};

			VertexDeclaration* m_vtxDecl;
			int32 m_vertexSize;
			bool m_opBufferBuilt = false;
			RenderOperationBuffer m_bufferedOp;
			VertexBuffer* m_vertexBuffer;

			List<SubPart> m_subParts;
			
			RenderDevice* m_renderDevice;
			ObjectFactory* m_factory;

			String m_name;

			int32 m_primitiveCount;
			int32 m_vertexCount;


			BoundingSphere m_boundingSphere;


		};
	}
}
#endif