#pragma once
#ifndef APOC3D_VERTEXELEMENT_H
#define APOC3D_VERTEXELEMENT_H

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

#include "apoc3d/Graphics/GraphicsCommon.h"

using namespace Apoc3D::Collections;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			class APAPI VertexElement
			{
			public:
				
				VertexElement() { }
				VertexElement(int offset, VertexElementFormat type, VertexElementUsage semantic)		
					: m_offset(offset), m_type(type), m_semantic(semantic), m_index(0)
				{
				}

				VertexElement(int offset, VertexElementFormat type, VertexElementUsage semantic, int index)
					: m_offset(offset), m_type(type), m_semantic(semantic), m_index(index)
				{
				}

				/** Gets the offset in the buffer that this element starts at. */
				int getOffset() const { return m_offset; }
				
				/** Gets the type of element. */
				VertexElementFormat getType() const { return m_type; }
				
				/** Gets the meaning of the element. */
				VertexElementUsage getUsage() const { return m_semantic; }
				
				/** Gets index of the item, only applicable for some elements like texture coords */
				int getIndex() const { return m_index; }
				int getSize() const { return GetTypeSize(m_type); }


				bool operator ==(const VertexElement& another)
				{
					return m_index == another.m_index &&
						m_offset == another.m_offset &&
						m_semantic == another.m_semantic &&
						m_type == another.m_type;
				}

				bool operator !=(const VertexElement& another) { return !(this->operator ==(another)); }


				static VertexElement* FindElementBySemantic(const List<VertexElement>& elem, VertexElementUsage semantic);
				
				static bool Compare(const List<VertexElement>& e1, const List<VertexElement>& e2);

				/**
				 *  Calculate the size of a specified vertex element format
				 */
				static int GetTypeSize(VertexElementFormat type);
		
				/**
				 *  Utility method which returns the count of values in a given type.
				 */
				static int GetTypeCount(VertexElementFormat type);

			private:
				int m_offset;
				VertexElementFormat m_type;
				VertexElementUsage m_semantic;
				int m_index;
			};
		}
	}
}

#endif