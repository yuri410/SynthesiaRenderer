#pragma once
#ifndef APOC3D_MODELDATA_H
#define APOC3D_MODELDATA_H

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

#include "apoc3d/Graphics/GraphicsCommon.h"
#include "apoc3d/Graphics/MaterialTypes.h"
#include "apoc3d/Graphics/ModelTypes.h"
#include "apoc3d/Graphics/RenderSystem/VertexElement.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Math/Color.h"
#include "apoc3d/Math/BoundingSphere.h"

#include "MaterialData.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Graphics::Animation;
using namespace Apoc3D::Math;
using namespace Apoc3D::IO;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace IO
	{
		/**
		 *  Defines one entire mesh's data stored in binary form and procedures to load/save them.
		 */
		class APAPI MeshData
		{
		public:
			/**
			 *  Vertex data, no matter what exactly the vertex elements are, this is in a binary buffer form defined in VertexElements.
			 */
			char* VertexData = nullptr;

			List<VertexElement> VertexElements;

			BoundingSphere BoundingSphere;

			String Name;
			uint32 VertexSize = 0;
			uint32 VertexCount = 0;

			/** 
			 *  A set of arrays of material data.
			 *  Each array in the set is corresponding to a sub mesh. each material data as the array element is a 
			 *  keyframe material if material animation is applicable.
			 *
			 *  When not using any material animation, this should be a set of arrays with a length of 1.
			 *  That is in equivalent to a set of MaterialData.
			 */
			MeshMaterialSet<MaterialData*> Materials;

			/** A list of triangle faces. Only triangle list is supported as primitive at this stage. */
			List<MeshFace> Faces;

			static uint32 ComputeVertexSize(const List<VertexElement>& elements);

			void LoadData(TaggedDataReader* data);
			TaggedDataWriter* SaveData();
			void SaveLite(BinaryWriter& bw);

			MeshData();
			~MeshData();

			MeshData(const MeshData&) = delete;
			MeshData& operator=(const MeshData&) = delete;

		};

		/** 
		 *  Defines one entire model's data stored in binary form and procedures to load/save them.
		 */
		class APAPI ModelData
		{
		public:
			/** A list of meshes that this model has. */
			List<MeshData*> Entities;

			ModelData() { }
			~ModelData();
			
			ModelData(const ModelData&) = delete;
			ModelData& operator=(const ModelData&) = delete;

			void Load(const ResourceLocation& rl);
			void Save(Stream& strm) const;
			void SaveLite(Stream& strm) const;
		private:
			void ReadData(TaggedDataReader* data, int32 id);
			TaggedDataWriter* WriteData() const;
		};
	}
}
#endif