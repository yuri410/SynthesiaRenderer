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
#ifndef D3D9PIXELSHADER_H
#define D3D9PIXELSHADER_H

#include "D3D9Shader.h"

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			class D3D9PixelShader : public D3D9Shader<IDirect3DPixelShader9, 
				&IDirect3DDevice9::CreatePixelShader, 
				&IDirect3DDevice9::SetPixelShaderConstantF,
				&IDirect3DDevice9::SetPixelShaderConstantI, 
				&IDirect3DDevice9::SetPixelShaderConstantB>
			{
			public:
				IDirect3DPixelShader9* getD3DPS() const { return m_shader; }

				D3D9PixelShader(D3D9RenderDevice* device, const byte* byteCode)
					: D3D9Shader(device, byteCode) { }
				D3D9PixelShader(D3D9RenderDevice* device, const ResourceLocation& rl)
					: D3D9Shader(device, rl) { }
				~D3D9PixelShader() { }

				virtual void SetTexture(int samIndex, Texture* tex);
				virtual void SetSamplerState(int samIndex, const ShaderSamplerState &state);

				virtual void SetTexture(const String& paramName, Texture* tex);
				virtual void SetSamplerState(const String& paramName, const ShaderSamplerState &state);

				virtual ShaderType getType() const { return ShaderType::Pixel; }
			};

			class D3D9VertexShader : public D3D9Shader<IDirect3DVertexShader9, 
				&IDirect3DDevice9::CreateVertexShader, 
				&IDirect3DDevice9::SetVertexShaderConstantF,
				&IDirect3DDevice9::SetVertexShaderConstantI, 
				&IDirect3DDevice9::SetVertexShaderConstantB>
			{
			public:
				IDirect3DVertexShader9* getD3DVS() const { return m_shader; }

				D3D9VertexShader(D3D9RenderDevice* device, const byte* byteCode)
					: D3D9Shader(device, byteCode) { }
				D3D9VertexShader(D3D9RenderDevice* device, const ResourceLocation& rl)
					: D3D9Shader(device, rl) { }
				~D3D9VertexShader() { }

				virtual void SetTexture(int samIndex, Texture* tex);
				virtual void SetSamplerState(int samIndex, const ShaderSamplerState &state);

				virtual void SetTexture(const String& paramName, Texture* tex);
				virtual void SetSamplerState(const String& paramName, const ShaderSamplerState& state);

				virtual ShaderType getType() const { return ShaderType::Vertex; }
			};
		}
	}
}

#endif