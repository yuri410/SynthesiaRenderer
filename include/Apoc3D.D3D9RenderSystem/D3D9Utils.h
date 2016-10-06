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

#ifndef D3D9UTILS_H
#define D3D9UTILS_H

#include "D3D9Common.h"
#include "apoc3d/Core/Plugin.h"
#include "apoc3d/Graphics/PixelFormat.h"
#include "apoc3d/Graphics/GraphicsCommon.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			namespace D3D9Utils
			{
				
				BufferUsageFlags GetBufferUsage(DWORD usage);
				PLUGINAPI PixelFormat ConvertBackPixelFormat(DWORD format);
				DepthFormat ConvertBackDepthFormat(DWORD format);
				

				VertexElementFormat ConvertBackVertexElementFormat(D3DDECLTYPE type);
				VertexElementUsage ConvertBackVertexElementUsage(D3DDECLUSAGE usage);


				D3DMULTISAMPLE_TYPE ConvertMultisample(uint32 sampleCount);
				uint32 ConvertBackMultiSample(D3DMULTISAMPLE_TYPE type);


				DWORD ConvertLockMode(LockMode mode);
				DWORD ConvertClearFlags(ClearFlags flags);

				DWORD ConvertBufferUsage(BufferUsageFlags usage);

				D3DPRIMITIVETYPE ConvertPrimitiveType(PrimitiveType type);

				D3DDECLUSAGE ConvertVertexElementUsage(VertexElementUsage usage);

				D3DDECLTYPE ConvertVertexElementFormat(VertexElementFormat type);

				D3DFILLMODE ConvertFillMode(FillMode mode);

				D3DCULL ConvertCullMode(CullMode mode);



				D3DBLEND ConvertBlend(Blend dv);

				D3DCMPFUNC ConvertCompare(CompareFunction fun);

				D3DBLENDOP ConvertBlendFunction(BlendFunction fun);

				D3DSTENCILOP ConvertStencilOperation(StencilOperation op);

				PLUGINAPI D3DFORMAT ConvertPixelFormat(PixelFormat format);
				D3DFORMAT ConvertDepthFormat(DepthFormat format);

				D3DTEXTUREFILTERTYPE ConvertTextureFilter(TextureFilter filter);

				DWORD ConvertTextureUsage(TextureUsage usage);
				D3DCUBEMAP_FACES ConvertCubeMapFace(CubeMapFace face);

				D3DTEXTUREADDRESS ConvertTextureAddress(TextureAddressMode ta);
			


				int32 GetD3DTextureWidth(D3DTexture2D* tex);
				int32 GetD3DTextureHeight(D3DTexture2D* tex);
				PixelFormat GetD3DTextureFormat(D3DTexture2D* tex);
				TextureUsage GetD3DTextureUsage(D3DTexture2D* tex);


				int32 GetD3DTextureWidth(D3DTexture3D* tex);
				int32 GetD3DTextureHeight(D3DTexture3D* tex);
				int32 GetD3DTextureDepth(D3DTexture3D* tex);
				PixelFormat GetD3DTextureFormat(D3DTexture3D* tex);
				TextureUsage GetD3DTextureUsage(D3DTexture3D* tex);


				int32 GetD3DTextureLength(D3DTextureCube* tex);
				PixelFormat GetD3DTextureFormat(D3DTextureCube* tex);
				TextureUsage GetD3DTextureUsage(D3DTextureCube* tex);


				int32 GetSurfaceWidth(IDirect3DSurface9* surface);
				int32 GetSurfaceHeight(IDirect3DSurface9* surface);
				DepthFormat GetSurfaceDepthFormat(IDirect3DSurface9* surface);
				PixelFormat GetSurfaceColorFormat(IDirect3DSurface9* color);
				uint32 GetSurfaceMultiSampleCount(IDirect3DSurface9* color);

				void logRTFailure(PixelFormat colorFormat, DepthFormat depFormat, const String& multisampleType, bool cubemap = false);
			};
		}
	}
}

#endif