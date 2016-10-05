#pragma once
#ifndef APOC3D_LOCKDATA_H
#define APOC3D_LOCKDATA_H

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
#include "apoc3d/Graphics/PixelFormat.h"

using namespace Apoc3D::VFS;
using namespace Apoc3D::Core;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			class APAPI DataRectangle
			{
			public:
				static const DataRectangle Empty;

				DataRectangle(int pitch, void* pointer, int width, int height, PixelFormat fmt)
					: m_pitch(pitch), m_pointer(pointer), m_width(width), m_height(height), m_format(fmt)
				{ }

				PixelFormat getFormat() const { return m_format; }
				int32 getPitch() const { return m_pitch; }
				int32 getWidth() const { return m_width; }
				int32 getHeight() const { return m_height; }
				int32 getMemorySize() const
				{
					return PixelFormatUtils::GetMemorySize(m_width, m_height, 1, m_format);
				}
				bool isCompressed() const
				{
					return PixelFormatUtils::IsCompressed(m_format);
				}
				void* getDataPointer() const { return m_pointer; }
				
				/**
				 *  Return whether this buffer is laid out consecutive in memory (ie the pitches
				 *  are equal to the dimensions)
				 */
				bool isConsecutive() const
				{
					return m_pitch == m_width;
				}

			private:
				int32 m_pitch;
				void* m_pointer;
				int32 m_width;
				int32 m_height;
				PixelFormat m_format;

			};

			class APAPI DataBox
			{
			public:
				static const DataBox Empty;

				PixelFormat getFormat() const { return m_format; }
				int32 getWidth() const { return m_width; }
				int32 getHeight() const { return m_height; }
				int32 getDepth() const { return m_depth; }

				bool isCompressed() const
				{
					return PixelFormatUtils::IsCompressed(m_format);
				}
				/** 
				 *  Return whether this buffer is laid out consecutive in memory (ie the pitches
				 *  are equal to the dimensions)
				 */
				bool isConsecutive() const
				{
					return m_rowPitch == m_width && m_slicePitch == (m_width*m_height);
				}

				int32 getRowPitch() const { return m_rowPitch; }
				int32 getSlicePitch() const { return m_slicePitch; }
				void* getDataPointer() const { return m_pointer; }
				int32 getMemorySize() const
				{
					return PixelFormatUtils::GetMemorySize(m_width, m_height, m_depth, m_format);
				}

				DataBox(int width, int height, int depth, int rowPitch, int slicePitch, 
					void* pointer, PixelFormat fmt)
					: m_rowPitch(rowPitch), m_slicePitch(slicePitch), m_pointer(pointer), m_format(fmt),
					m_width(width), m_height(height), m_depth(depth) { }

			private:
				int32 m_rowPitch;
				int32 m_slicePitch;
				void* m_pointer;
				int32 m_width;
				int32 m_height;
				int32 m_depth;

				PixelFormat m_format;


			};
		}
	}
}

#endif