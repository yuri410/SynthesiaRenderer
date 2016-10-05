#pragma once
#ifndef APOC3D_VERTEXDECLARATION_H
#define APOC3D_VERTEXDECLARATION_H

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


#include "VertexElement.h"
#include "apoc3d/Collections/List.h"

using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{			
			typedef List<VertexElement> VertexElementList;

			class APAPI VertexDeclaration
			{
			public:
				VertexDeclaration(const List<VertexElement> &e);

				virtual ~VertexDeclaration();

				/**
				 *  Finds a VertexElement with the given semantic, and index if there is more than
				 *  one element with the same semantic.
				 */
				const VertexElement* FindElementBySemantic(VertexElementUsage semantic) const
				{
					return FindElementBySemantic(semantic, -1);
				}

				/**
				 *  Finds a VertexElement with the given semantic, and index if there is more than
				 *  one element with the same semantic.
				 */
				const VertexElement* FindElementBySemantic(VertexElementUsage semantic, int32 index) const;
				
				int GetVertexSize() const;														/** Gets the vertex size defined by this declaration. */

				void CopyTo(VertexElementList& target) const;

				const VertexElement& operator[](int32 i) const { return m_elements[i]; }

				const VertexElement& getElement(int32 i) const  { return m_elements[i]; }		/** Gets the VertexElement at the specified index. */
				int getElementCount() const { return m_elements.getCount(); }					/** Gets the number of elements in the declaration. */

				const VertexElement* begin() const { return m_elements.begin(); }
				const VertexElement* end() const { return m_elements.end(); }

			protected:
				VertexElementList m_elements;

			};
		}
	}
}
#endif