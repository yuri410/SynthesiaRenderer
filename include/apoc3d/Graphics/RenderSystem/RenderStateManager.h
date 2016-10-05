#pragma once
#ifndef APOC3D_RENDERSTATEMANAGER_H
#define APOC3D_RENDERSTATEMANAGER_H

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
#include "apoc3d/Math/Plane.h"
#include "apoc3d/Math/Rectangle.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			class APAPI RenderStateManager
			{
			public:
				/************************************************************************/
				/* Alpha Test                                                           */
				/************************************************************************/
				virtual bool getAlphaTestEnable() = 0;
				virtual CompareFunction getAlphaTestFunction() = 0;
				virtual uint32 getAlphaReference() = 0;

				virtual void SetAlphaTestParameters(bool enable, CompareFunction func, uint32 reference) = 0;

				/************************************************************************/
				/* Alpha Blend                                                          */
				/************************************************************************/
				virtual bool getAlphaBlendEnable() = 0;
				virtual BlendFunction getAlphaBlendOperation() = 0;
				virtual Blend getAlphaSourceBlend() = 0;
				virtual Blend getAlphaDestinationBlend() = 0;
				virtual uint32 getAlphaBlendFactor() = 0;

				virtual void SetAlphaBlend(bool enable, BlendFunction func, Blend srcBlend, Blend dstBlend, uint32 factor) = 0;

				virtual bool getSeparateAlphaBlendEnable() = 0;
				virtual BlendFunction getSeparateAlphaBlendOperation() = 0;
				virtual Blend getSeparateAlphaSourceBlend() = 0;
				virtual Blend getSeparateAlphaDestinationBlend() = 0;
				//virtual uint32 getSeparateAlphaBlendFactor() = 0;

				virtual void SetSeparateAlphaBlend(bool enable, BlendFunction func, Blend srcBlend, Blend dstBlend) = 0;

				/************************************************************************/
				/* Depth                                                                */
				/************************************************************************/
				virtual float getDepthBias() = 0;
				virtual float getSlopeScaleDepthBias() = 0;
				virtual CompareFunction getDepthBufferFunction() = 0;
				virtual bool getDepthBufferWriteEnabled() = 0;
				virtual bool getDepthBufferEnabled() = 0;

				virtual void SetDepth(bool enable, bool writeEnable, float bias, float slopebias, CompareFunction compare) = 0;

				/************************************************************************/
				/* Common                                                               */
				/************************************************************************/
				virtual CullMode getCullMode() = 0;
				virtual FillMode getFillMode() = 0;
				virtual void SetCullMode(CullMode mode) = 0;
				virtual void SetFillMode(FillMode mode) = 0;
				/************************************************************************/
				/* Point                                                                */
				/************************************************************************/
				virtual float getPointSize() = 0;
				virtual float getPointSizeMax() = 0;
				virtual float getPointSizeMin() = 0;
				virtual bool getPointSpriteEnabled() = 0;

				virtual void SetPointParameters(float size, float maxSize, float minSize, bool pointSprite) = 0;
				/************************************************************************/
				/* Stencil                                                              */
				/************************************************************************/
				virtual bool getStencilEnabled() = 0;
				virtual StencilOperation getStencilFail() = 0;
				virtual StencilOperation getStencilPass() = 0;
				virtual StencilOperation getStencilDepthFail() = 0;
				virtual uint32 getStencilRefrence() = 0;
				virtual CompareFunction getStencilFunction() = 0;
				virtual uint32 getStencilMask() = 0;				
				virtual uint32 getStencilWriteMask() = 0;

				virtual void SetStencil(bool enabled, StencilOperation fail, StencilOperation depthFail, StencilOperation pass, uint32 ref, CompareFunction func, uint32 mask, uint32 writemask) = 0;
				virtual void SetStencilTwoSide(bool enabled, StencilOperation fail, StencilOperation depthFail, StencilOperation pass, CompareFunction func) = 0;

				virtual bool getTwoSidedStencilMode() = 0;
				
				virtual StencilOperation getCounterClockwiseStencilFail() = 0;
				virtual StencilOperation getCounterClockwiseStencilPass() = 0;
				virtual StencilOperation getCounterClockwiseStencilDepthBufferFail() = 0;

				virtual CompareFunction getCounterClockwiseStencilFunction() = 0;
				
				/************************************************************************/
				/* Scissor Test                                                         */
				/************************************************************************/
				virtual bool getScissorTestEnabled() = 0;
				virtual Apoc3D::Math::Rectangle getScissorTestRect() = 0;
				virtual void setScissorTest(bool enable, const Apoc3D::Math::Rectangle* rect) = 0;
				
				/************************************************************************/
				/* ColorWrite                                                           */
				/************************************************************************/
				virtual void getColorWriteEnabled(int rtIndex, bool& r, bool& g, bool& b, bool& a) = 0;
				virtual void setColorWriteEnabled(int rtIndex, bool r, bool g, bool b, bool a) = 0;
				
				/************************************************************************/
				/* Samplers                                                             */
				/************************************************************************/
				virtual void SetVertexSampler(int samplerIndex, const ShaderSamplerState& sampler) = 0;
				virtual void SetPixelSampler(int samplerIndex, const ShaderSamplerState& sampler) = 0;

				virtual const ShaderSamplerState& getPixelSampler(int samplerIndex) const = 0;
				virtual const ShaderSamplerState& getVertexSampler(int samplerIndex) const = 0;

			protected:
				RenderStateManager(RenderDevice* device);
			private:
				RenderDevice* m_renderDevice;
			};

			class APAPI ScopeRenderTargetChange
			{
			public:
				ScopeRenderTargetChange(RenderDevice* device, int32 idx, RenderTarget* rt);
				ScopeRenderTargetChange(RenderDevice* device, std::initializer_list<std::pair<int32, RenderTarget* >> list);

				ScopeRenderTargetChange(RenderDevice* device, int32 idx, RenderTarget* rt, DepthStencilBuffer* dsb);
				ScopeRenderTargetChange(RenderDevice* device, std::initializer_list<std::pair<int32, RenderTarget* >> list, DepthStencilBuffer* dsb);
				~ScopeRenderTargetChange();

				template <typename Func>
				void Run(Func f) { f(); }
			private:
				void ChangeRenderTarget(int32 idx, RenderTarget* rt);
				void ChangeDepthStencilBuffer(DepthStencilBuffer* dsb);

				RenderDevice* m_device;

				RenderTarget* m_oldRenderTargets[4];
				bool m_oldRenderTargetChanged[4];
				
				DepthStencilBuffer* m_oldDSB = nullptr;
				bool m_oldDSBChanged = false;

				// no allocation unless involved
				HashMap<int32, RenderTarget*> m_additionalOldRenderTarget;
			};
		}
	}
}
#endif