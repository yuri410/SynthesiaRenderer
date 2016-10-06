#pragma once
#ifndef APOC3D_BATCHMODELBUILDER_H
#define APOC3D_BATCHMODELBUILDER_H

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
#include "RenderOperationBuffer.h"
#include "ModelTypes.h"
#include "apoc3d/Vfs/ResourceLocation.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Core;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::Animation;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::VFS;
using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace Graphics
	{
		/**
		 *  A static model batching combiner.
		 *  This class joins several small models into a big one with each transformed by a matrix.
		 *  All input models are expected to have the same vertex format.
		 */
		class APAPI BatchModelBuilder
		{
		public:
			/** Add an instance of a given source, with a given transform matrix. */
			void AddEntry(int source, const Matrix& transform);

			/** Add the sources of models represented by FileLocation */
			void AddSource(const FileLocation& fl);

			/** Build the model data, when finished adding sources and entries. */
			ModelData* BuildData();

			BatchModelBuilder();
			~BatchModelBuilder();
		private:
			struct Entry
			{
				int SourceId;
				Matrix Transform;
			};

			List<FileLocation> m_modelTable;

			List<Entry> m_entires;
		};

	}
}
#endif