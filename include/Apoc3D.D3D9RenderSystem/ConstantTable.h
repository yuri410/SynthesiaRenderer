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

#ifndef APOC3D_D3D9_CONSTANTTABLE_H
#define APOC3D_D3D9_CONSTANTTABLE_H

#include "D3D9Common.h"
#include "apoc3d/Core/Plugin.h"
#include "apoc3d/Collections/HashMap.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			enum ShaderRegisterSetType : byte
			{
				SREG_Bool,
				SREG_Int4,
				SREG_Float4,
				SREG_Sampler
			};
			enum ShaderConstantClass : byte
			{
				SCC_Scalar,
				SCC_Vector,
				SCC_Matrix_Rows,
				SCC_Matrix_Columns,
				SCC_Object,
				SCC_Struct
			};
			enum ShaderConstantType : byte
			{
				SCT_VoidPointer,
				SCT_Boolean,
				SCT_Integer,
				SCT_Float,
				SCT_String,
				SCT_Texture,
				SCT_Texture1D,
				SCT_Texture2D,
				SCT_Texture3D,
				SCT_TextureCube,
				SCT_Sampler,
				SCT_Sampler1D,
				SCT_Sampler2D,
				SCT_Sampler3D,
				SCT_SamplerCube,
				SCT_PixelShader,
				SCT_VertexShader,
				SCT_PixelFragment,
				SCT_VertexFragment,
				SCT_Unsupported
			};
			struct ShaderConstant
			{
				String Name;
				uint16 RegisterIndex;
				uint16 RegisterCount;
				byte SamplerIndex;
				bool IsSampler;

				ShaderRegisterSetType RegisterSet;
				ShaderConstantClass Class;
				ShaderConstantType Type;
				byte Rows;
				byte Columns;
				uint16 Elements;
				uint16 StructMembers;
				uint32 SizeInBytes;
			};

			class PLUGINAPI ConstantTable
			{
			public:
				ConstantTable(const DWORD* bytes);
				virtual ~ConstantTable();

				inline const ShaderConstant& getConstant(const String& name) const;
				inline const ShaderConstant* tryGetConstant(const String& name) const { return m_table.TryGetValue(name); }

				void Read(BinaryReader* br);
				void Write(BinaryWriter* bw);

				void GetAllConstantNames(List<String>& list) const { return m_table.FillKeys(list); }

			protected:
				ConstantTable() { }

				void ReadShaderComment(char* data, int32 size);

				HashMap<String, ShaderConstant> m_table;

				NO_INLINE static void ThrowKeyNotFoundEx(const String& name);
			};

			const ShaderConstant& ConstantTable::getConstant(const String& name) const
			{
				ShaderConstant* sc = m_table.TryGetValue(name);
				
				if (sc)
				{
					return *sc;
				}
				ThrowKeyNotFoundEx(name);
				throw; // keep the compiler happy
			}

		}
	}
}


#endif