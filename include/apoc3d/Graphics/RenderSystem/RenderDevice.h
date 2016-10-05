#pragma once
#ifndef APOC3D_RENDERDEVICE_H
#define APOC3D_RENDERDEVICE_H

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

#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Graphics/GraphicsCommon.h"
#include "apoc3d/Graphics/RenderOperation.h"
#include "apoc3d/Graphics/PixelFormat.h"
#include "apoc3d/Math/Viewport.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Math;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			/**
			 *  Applications use RenderDevice to perform DrawPrimitive level rendering and 
			 *  obtain related objects such as RenderStateManager and RenderTargets.
			 */
			class APAPI RenderDevice
			{
				RTTI_BASE;
			public:
				static bool HasBatchReportRequest;

				virtual ~RenderDevice() { }

				virtual Capabilities* const getCapabilities() const = 0;
				
				/** Gets the color pixel format of default render target. */
				virtual PixelFormat GetDefaultRTFormat() = 0;

				/** Gets the depth pixel format of default render target. */
				virtual DepthFormat GetDefaultDepthStencilFormat() = 0;

				virtual int32 GetAvailableVideoRamInMB() = 0;

				/**
				 *  Initialize. 
				 *  This is called when the graphics device, window,  have been created and called just before 
				 *  when the Game::Initialize method is called.
				 */
				virtual void Initialize() = 0;

				
				const String& getName() const { return m_rdName; }						/** Gets the device's name. */

				ObjectFactory* getObjectFactory() { return m_objectFactory; }			/** Gets the device's ObjectFactory object. */
				RenderStateManager* getRenderState() { return m_renderStates; }			/** Gets the device's RenderStateManager object. */

				/** Notify the RenderDevice a new frame is began to draw. */
				virtual void BeginFrame();

				/** Notify the RenderDevice the current frame rendering is over. */
				virtual void EndFrame();

				
				/** Clears all the currently binded render targets. */
				virtual void Clear(ClearFlags flags, uint color, float depth, int stencil) = 0;

				/** Sets the current render target at given index. A value of 0 will reset the rendertarget to default. */
				virtual void SetRenderTarget(int index, RenderTarget* rt) = 0;
				virtual RenderTarget* GetRenderTarget(int index) = 0;

				virtual void SetDepthStencilBuffer(DepthStencilBuffer* buf) = 0;
				virtual DepthStencilBuffer* GetDepthStencilBuffer() = 0;

				virtual void BindVertexShader(Shader* shader) = 0;
				virtual void BindPixelShader(Shader* shader) = 0;

				/**
				 *  Draws a list of RenderOperations.
				 *  @param mtrl All RenderOperations are expected to have the same material as this param.
				 *  @param passSelID An index used to tell which shader effect inside materials to be used. 
				 *   A number of -1 mean the first available shader effect will be used. 
				 */
				virtual void Render(Material* mtrl, const RenderOperation* op, int count, int passSelID);

				virtual Viewport getViewport() = 0;
				virtual void setViewport(const Viewport& vp) = 0;

				/** Gets the number of draw calls on the last frame if the frame is done, or the current draw call count if not. */
				uint getBatchCount() const { return m_batchCount; }

				/** Gets the number of primitives drawn on the last frame if the frame is done, or the current value if not. */
				uint getPrimitiveCount() const { return m_primitiveCount; }

				/** Gets the number of vertices drawn on the last frame if the frame is done, or the current value if not. */
				uint getVertexCount() const { return m_vertexCount; }

			private:
				struct BatchReportEntry
				{
					String DebugName;

					int32 DP = 0;
					int32 Vertex = 0;
					int32 Primitive = 0;

					int32 VertexInstanced = 0;
					int32 PrimitiveInstanced = 0;
					int32 DPInstanced = 0;
					int32 InstancingBatch = 0;

					int32 EmptyROP = 0;
					int32 EmptyInstancedROP = 0;

					int32 Sorter = 0;

					void CalculateSorter();
				};

				String m_rdName;

				Apoc3D::Collections::HashMap<void*, BatchReportEntry>* m_reportTableByMaterial = nullptr;

			protected:
				//Capabilities m_caps;
				uint m_batchCount = 0;
				uint m_primitiveCount = 0;
				uint m_vertexCount = 0;

				ObjectFactory* m_objectFactory = nullptr;
				RenderStateManager* m_renderStates = nullptr;

				RenderDevice(const String &renderSysName)
					: m_rdName(renderSysName) { }
			};

			/** An interface for check if some feature are supported. */
			class APAPI Capabilities
			{
			public:
				virtual ~Capabilities() { }

				virtual bool SupportsRenderTarget(const String& multisampleMode, PixelFormat pixFormat, DepthFormat depthFormat) = 0;
				virtual bool SupportsPixelShader(const char* implType, int majorVer, int minorVer) = 0;
				virtual bool SupportsVertexShader(const char* implType, int majorVer, int minorVer) = 0;

				virtual void EnumerateRenderTargetMultisampleModes(PixelFormat pixFormat, DepthFormat depthFormat, Apoc3D::Collections::List<String>& modes) = 0;
				virtual const String* FindClosesetMultisampleMode(uint32 sampleCount, PixelFormat pixFormat, DepthFormat depthFormat) = 0;

				virtual int GetMRTCount() = 0;
				virtual bool SupportsMRTDifferentBits() = 0;
			};

			/** 
			 *  Factory that can create resources like textures, buffers and shaders. 
			 *  Application can use this to create resources not managed by resource managers.
			 */
			class APAPI ObjectFactory
			{
			public:
				virtual ~ObjectFactory() { }

				/**
				 *  Creates a texture from a ResourceLocation. 
				 * @param managed tell if the Texture will be managed.
				 * @remarks
				 *  Texture Manager use this to create managed textures, with managed=true. 
				 *  The client application can call this:
				 *    1. with managed=false. This will be almost the same as TextureManager::CreateUnmanagedInstance,
				 *       except that TextureManager have more features like ResourceLocation redirecting for 
				 *       debugging purposes. If the texture is unmanaged, it will be loaded with this call.
				 *    2. with managed=true. The texture should be managed by the TextureManager.
				 *       The texture can still be created here without using TextureManager. In this case the texture should be loaded manually.
				 */
				virtual Texture* CreateTexture(const ResourceLocation& rl, TextureUsage usage, bool managed) = 0;


				/**
				 *  Creates a blank 2D or 1D texture.
				 *  An 1D texture will be created if either width or height is one.
				 * @remarks
				 *  This as well as all the following methods has nothing to do with any kind of Resource Managers.
				 *  They are almost purely API level.
				 *  Thus, no more comment unless clarification is needed.
				 */
				virtual Texture* CreateTexture(int width, int height, int levelCount, TextureUsage usage, PixelFormat format) = 0;
				Texture* CreateTexture(const Point& size2d, int levelCount, TextureUsage usage, PixelFormat format);

				/**
				 *  Creates a blank 2D, 1D or 3D texture.
				 *  In addition to the above one, a volume texture will be created if depth is more than 1.
				 */
				virtual Texture* CreateTexture(int width, int height, int depth, int levelCount, TextureUsage usage, PixelFormat format) = 0;

				/** Creates a blank Cube map. */
				virtual Texture* CreateTexture(int length, int levelCount, TextureUsage usage, PixelFormat format) = 0;

				/**
				 *  Creates a RenderTarget. 
				 *  For a list of supported multisampleModes, use Capabilities::EnumerateRenderTargetMultisampleModes
				 *  Use "" or "none" to disable multisampling.
				 * 
				 * Notice:
				 *  When creating manually in client code, do check the device capabilities.
				 *
				 *  When using render targets in scene-render scripts, do not worry about checking the capabilities,
				 *  as the script will auto fall back if not supported.
				 */
				virtual RenderTarget* CreateRenderTarget(int width, int height, PixelFormat clrFmt, const String& multisampleMode) = 0;
				virtual DepthStencilBuffer* CreateDepthStencilBuffer(int32 width, int32 height, DepthFormat depFmt, const String& multisampleMode) = 0;
				
				RenderTarget* CreateRenderTarget(const Point& size2d, PixelFormat clrFmt, const String& multisampleMode);
				RenderTarget* CreateRenderTarget(const Point& size2d, PixelFormat clrFmt);
				RenderTarget* CreateRenderTarget(int width, int height, PixelFormat clrFmt);

				DepthStencilBuffer* CreateDepthStencilBuffer(const Point& size2d, DepthFormat depFmt, const String& multisampleMode);
				DepthStencilBuffer* CreateDepthStencilBuffer(const Point& size2d, DepthFormat depFmt);
				DepthStencilBuffer* CreateDepthStencilBuffer(int32 width, int32 height, DepthFormat depFmt);

				virtual CubemapRenderTarget* CreateCubemapRenderTarget(int32 length, PixelFormat clrFmt) = 0;

			

				virtual IndexBuffer* CreateIndexBuffer(IndexBufferType type, int count, BufferUsageFlags usage) = 0;
				virtual VertexBuffer* CreateVertexBuffer(int vertexCount, VertexDeclaration* vtxDecl, BufferUsageFlags usage) = 0;


				virtual VertexDeclaration* CreateVertexDeclaration(const Apoc3D::Collections::List<VertexElement>& elements) = 0;

				/** 
				 *  Creates a vertex shader from a piece of code.
				 *  The code could be byteCode(D3D9) or source(OpenGL) depends on 
				 *  the specific API.
				 */
				virtual Shader* CreateVertexShader(const byte* byteCode) = 0;
				virtual Shader* CreatePixelShader(const byte* byteCode) = 0;

				virtual Shader* CreateVertexShader(const ResourceLocation& resLoc) = 0;
				virtual Shader* CreatePixelShader(const ResourceLocation& resLoc) = 0;


				virtual Sprite* CreateSprite() = 0;

				//virtual InstancingData* CreateInstancingData() = 0;

			protected:
				ObjectFactory(RenderDevice* rd)
					: m_renderDevice(rd) { }

			private:
				RenderDevice* m_renderDevice;

			};
		}
	}
}
#endif