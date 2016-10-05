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
#ifndef D3D9RENDERSTATEMANAGER_H
#define D3D9RENDERSTATEMANAGER_H

#include "D3D9Common.h"

#include "apoc3d/Graphics/RenderSystem/RenderStateManager.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			/** This class stores states for quick state checks without any
			 *  D3D calls. Other classes check state to avoid redundant state changes.
			 */
			class NativeD3DStateManager
			{
			public:
				NativeD3DStateManager(D3D9RenderDevice* device);
				~NativeD3DStateManager();

				void SetAlphaTestParameters(bool enable, uint32 reference);
				void SetAlphaTestParameters(bool enable, CompareFunction func, uint32 reference);
				void SetAlphaBlend(bool enable, BlendFunction func, Blend srcBlend, Blend dstBlend);
				void SetAlphaBlend(bool enable, BlendFunction func, Blend srcBlend, Blend dstBlend, uint32 factor);
				void SetSeparateAlphaBlend(bool enable, BlendFunction func, Blend srcBlend, Blend dstBlend);
				void SetDepth(bool enable, bool writeEnable);
				void SetDepth(bool enable, bool writeEnable, float bias, float slopebias, CompareFunction compare);
				void SetPointParameters(float size, float maxSize, float minSize, bool pointSprite);
				void SetStencil(bool enabled, StencilOperation fail, StencilOperation depthFail, StencilOperation pass, uint32 ref, CompareFunction func, uint32 mask, uint32 writemask);
				void SetStencilTwoSide(bool enabled, StencilOperation fail, StencilOperation depthFail, StencilOperation pass, CompareFunction func);
				void SetCullMode(CullMode mode);
				void SetFillMode(FillMode mode);

				/************************************************************************/
				/* Alpha Test                                                           */
				/************************************************************************/
				bool getAlphaTestEnable() { return m_cachedAlphaTestEnable; }
				CompareFunction getAlphaTestFunction() { return m_cachedAlphaTestFunction; }
				uint32 getAlphaReference() { return m_cachedAlphaReference; }

				/************************************************************************/
				/* Alpha Blend                                                          */
				/************************************************************************/
				bool getAlphaBlendEnable() { return m_cachedAlphaBlendEnable; }
				BlendFunction getAlphaBlendOperation() { return m_cachedAlphaBlendFunction; }
				Blend getAlphaSourceBlend() { return m_cachedAlphaSourceBlend; }
				Blend getAlphaDestinationBlend() { return m_cachedAlphaDestBlend; }
				uint32 getAlphaBlendFactor() { return m_cachedAlphaBlendFactor; }

				void setAlphaBlendEnable(bool val);
				void setAlphaBlendOperation(BlendFunction func);
				void setAlphaSourceBlend(Blend val);
				void setAlphaDestinationBlend(Blend val);

				bool getSeparateAlphaBlendEnable() { return m_cachedSepAlphaBlendEnable; }
				BlendFunction getSeparateAlphaBlendOperation() { return m_cachedSepAlphaBlendFunction; } 
				Blend getSeparateAlphaSourceBlend() { return m_cachedSepAlphaSourceBlend; }
				Blend getSeparateAlphaDestinationBlend() { return m_cachedSepAlphaDestBlend; }
				//virtual uint32 getSeparateAlphaBlendFactor() { return m_cachedSepAlphaBlendFactor; }

				/************************************************************************/
				/* Depth                                                                */
				/************************************************************************/
				float getDepthBias() { return m_cachedDepthBias; }
				float getSlopeScaleDepthBias() { return m_cachedSlopeScaleDepthBias; }
				CompareFunction getDepthBufferFunction() { return m_cachedDepthBufferFunction; }
				bool getDepthBufferWriteEnabled() { return m_cachedDepthBufferWriteEnabled; }
				bool getDepthBufferEnabled() { return m_cachedDepthBufferEnabled; }

				/************************************************************************/
				/* Common                                                               */
				/************************************************************************/
				CullMode getCullMode() { return m_cachedCullMode; }
				FillMode getFillMode() { return m_cachedFillMode; }

				/************************************************************************/
				/* Point                                                                */
				/************************************************************************/
				float getPointSize() { return m_cachedPointSize; }
				float getPointSizeMax() { return m_cachedPointSizeMax; }
				float getPointSizeMin() { return m_cachedPointSizeMin; }
				bool getPointSpriteEnabled() { return m_cachedPointSpriteEnabled; }

				/************************************************************************/
				/* Stencil                                                              */
				/************************************************************************/
				bool getStencilEnabled() { return m_cachedStencilEnabled; }
				StencilOperation getStencilFail() { return m_cachedStencilFail; }
				StencilOperation getStencilPass() { return m_cachedStencilPass; }
				StencilOperation getStencilDepthFail() { return m_cachedStencilDepthFail; }
				uint32 getStencilRefrence() { return m_cachedRefrenceStencil; }
				CompareFunction getStencilFunction() { return m_cachedStencilFunction; }
				uint32 getStencilMask() { return m_cachedStencilMask; }
				uint32 getStencilWriteMask() { return m_cachedStencilWriteMask; }

				bool getTwoSidedStencilMode() { return m_cachedTwoSidedStencilMode; }

				StencilOperation getCounterClockwiseStencilFail() { return m_cachedCounterClockwiseStencilFail; }
				StencilOperation getCounterClockwiseStencilPass() { return m_cachedCounterClockwiseStencilPass; }
				StencilOperation getCounterClockwiseStencilDepthBufferFail() { return m_cachedCounterClockwiseStencilDepthBufferFail; }

				CompareFunction getCounterClockwiseStencilFunction() { return m_cachedCounterClockwiseStencilFunction; }


				/************************************************************************/
				/* Color Write                                                          */
				/************************************************************************/
				void getColorWriteEnabled0(bool& r, bool& g, bool& b, bool& a);
				void setColorWriteEnabled0(bool r, bool g, bool b, bool a);
				void getColorWriteEnabled1(bool& r, bool& g, bool& b, bool& a);
				void setColorWriteEnabled1(bool r, bool g, bool b, bool a);
				void getColorWriteEnabled2(bool& r, bool& g, bool& b, bool& a);
				void setColorWriteEnabled2(bool r, bool g, bool b, bool a);
				void getColorWriteEnabled3(bool& r, bool& g, bool& b, bool& a);
				void setColorWriteEnabled3(bool r, bool g, bool b, bool a);

				/************************************************************************/
				/* Samplers                                                             */
				/************************************************************************/
				void SetVertexSampler(int samplerIndex, const ShaderSamplerState& sampler);
				void SetPixelSampler(int samplerIndex, const ShaderSamplerState& sampler);

				const ShaderSamplerState& getPixelSampler(int samplerIndex) const;
				const ShaderSamplerState& getVertexSampler(int samplerIndex) const;

				int getTextureSlotCount() const { return m_textureSlotCount; }
				D3D9Texture* getTexture(int i) const { assert(i<m_textureSlotCount); return m_textureSlots[i]; }
				void SetTexture(int i, D3D9Texture* tex);

				void Reset() { InitializeDefaultState(); }
			private:
				void InitializeDefaultState();
				void SetSampler(DWORD samplerIndex, ShaderSamplerState& curState, const ShaderSamplerState& state);

				D3D9RenderDevice* m_device;

				bool m_cachedAlphaTestEnable;
				CompareFunction m_cachedAlphaTestFunction;
				uint32 m_cachedAlphaReference;

				bool m_cachedAlphaBlendEnable;
				BlendFunction m_cachedAlphaBlendFunction;
				Blend m_cachedAlphaSourceBlend;
				Blend m_cachedAlphaDestBlend;
				uint32 m_cachedAlphaBlendFactor;

				bool m_cachedSepAlphaBlendEnable;
				BlendFunction m_cachedSepAlphaBlendFunction;
				Blend m_cachedSepAlphaSourceBlend;
				Blend m_cachedSepAlphaDestBlend;
				//uint32 m_cachedSepAlphaBlendFactor;

				float m_cachedDepthBias;
				float m_cachedSlopeScaleDepthBias;
				CompareFunction m_cachedDepthBufferFunction;
				bool m_cachedDepthBufferWriteEnabled;
				bool m_cachedDepthBufferEnabled;

				CullMode m_cachedCullMode;
				FillMode m_cachedFillMode;

				float m_cachedPointSize;
				float m_cachedPointSizeMax;
				float m_cachedPointSizeMin;
				bool m_cachedPointSpriteEnabled;

				bool m_cachedStencilEnabled;
				StencilOperation m_cachedStencilFail;
				StencilOperation m_cachedStencilPass;
				StencilOperation m_cachedStencilDepthFail;
				uint32 m_cachedRefrenceStencil;
				CompareFunction m_cachedStencilFunction;
				uint32 m_cachedStencilMask;
				uint32 m_cachedStencilWriteMask;

				bool m_cachedTwoSidedStencilMode;

				StencilOperation m_cachedCounterClockwiseStencilFail;
				StencilOperation m_cachedCounterClockwiseStencilPass;
				StencilOperation m_cachedCounterClockwiseStencilDepthBufferFail;

				CompareFunction m_cachedCounterClockwiseStencilFunction;

				ShaderSamplerState* m_pixelSamplers = nullptr;
				ShaderSamplerState* m_vertexSamplers = nullptr;

				bool m_colorWrite0[4];
				bool m_colorWrite1[4];
				bool m_colorWrite2[4];
				bool m_colorWrite3[4];

				int m_textureSlotCount;
				D3D9Texture** m_textureSlots = nullptr;
			};

			class D3D9RenderStateManager final : public RenderStateManager
			{
			public:
				D3D9RenderStateManager(D3D9RenderDevice* device, NativeD3DStateManager* nsmgr);
				~D3D9RenderStateManager();

				virtual void SetAlphaTestParameters(bool enable, CompareFunction func, uint32 reference)
				{
					m_stMgr->SetAlphaTestParameters(enable, func, reference);
				}
				virtual void SetAlphaBlend(bool enable, BlendFunction func, Blend srcBlend, Blend dstBlend, uint32 factor)
				{
					m_stMgr->SetAlphaBlend(enable, func, srcBlend, dstBlend, factor);
				}
				virtual void SetSeparateAlphaBlend(bool enable, BlendFunction func, Blend srcBlend, Blend dstBlend)
				{
					m_stMgr->SetSeparateAlphaBlend(enable, func, srcBlend, dstBlend);
				}
				virtual void SetDepth(bool enable, bool writeEnable, float bias, float slopebias, CompareFunction compare)
				{
					m_stMgr->SetDepth(enable, writeEnable, bias, slopebias, compare);
				}
				virtual void SetPointParameters(float size, float maxSize, float minSize, bool pointSprite)
				{
					m_stMgr->SetPointParameters(size, maxSize, minSize, pointSprite);
				}
				virtual void SetStencil(bool enabled, StencilOperation fail, StencilOperation depthFail, StencilOperation pass, uint32 ref, CompareFunction func, uint32 mask, uint32 writemask)
				{
					m_stMgr->SetStencil(enabled, fail, depthFail, pass, ref, func, mask, writemask);
				}
				virtual void SetStencilTwoSide(bool enabled, StencilOperation fail, StencilOperation depthFail, StencilOperation pass, CompareFunction func)
				{
					m_stMgr->SetStencilTwoSide(enabled, fail, depthFail, pass, func);
				}
				virtual void SetCullMode(CullMode mode)
				{
					m_stMgr->SetCullMode(mode);
				}
				virtual void SetFillMode(FillMode mode)
				{
					m_stMgr->SetFillMode(mode);
				}

				/************************************************************************/
				/* Alpha Test                                                           */
				/************************************************************************/
				virtual bool getAlphaTestEnable() { return m_stMgr->getAlphaTestEnable(); }
				virtual CompareFunction getAlphaTestFunction() { return m_stMgr->getAlphaTestFunction(); }
				virtual uint32 getAlphaReference() { return m_stMgr->getAlphaReference(); }

				/************************************************************************/
				/* Alpha Blend                                                          */
				/************************************************************************/
				virtual bool getAlphaBlendEnable() { return m_stMgr->getAlphaBlendEnable(); }
				virtual BlendFunction getAlphaBlendOperation() { return m_stMgr->getAlphaBlendOperation(); }
				virtual Blend getAlphaSourceBlend() { return m_stMgr->getAlphaSourceBlend(); }
				virtual Blend getAlphaDestinationBlend() { return m_stMgr->getAlphaDestinationBlend(); }
				virtual uint32 getAlphaBlendFactor() { return m_stMgr->getAlphaBlendFactor(); }

				virtual bool getSeparateAlphaBlendEnable() { return m_stMgr->getSeparateAlphaBlendEnable(); }
				virtual BlendFunction getSeparateAlphaBlendOperation() { return m_stMgr->getSeparateAlphaBlendOperation(); } 
				virtual Blend getSeparateAlphaSourceBlend() { return m_stMgr->getSeparateAlphaSourceBlend(); }
				virtual Blend getSeparateAlphaDestinationBlend() { return m_stMgr->getSeparateAlphaDestinationBlend(); }
				//virtual uint32 getSeparateAlphaBlendFactor() { return m_cachedSepAlphaBlendFactor; }

				/************************************************************************/
				/* Depth                                                                */
				/************************************************************************/
				virtual float getDepthBias() { return m_stMgr->getDepthBias(); }
				virtual float getSlopeScaleDepthBias() { return m_stMgr->getSlopeScaleDepthBias(); }
				virtual CompareFunction getDepthBufferFunction() { return m_stMgr->getDepthBufferFunction(); }
				virtual bool getDepthBufferWriteEnabled() { return m_stMgr->getDepthBufferWriteEnabled(); }
				virtual bool getDepthBufferEnabled() { return m_stMgr->getDepthBufferEnabled(); }

				/************************************************************************/
				/* Common                                                               */
				/************************************************************************/
				virtual CullMode getCullMode() { return m_stMgr->getCullMode(); }
				virtual FillMode getFillMode() { return m_stMgr->getFillMode(); }

				/************************************************************************/
				/* Point                                                                */
				/************************************************************************/
				virtual float getPointSize() { return m_stMgr->getPointSize(); }
				virtual float getPointSizeMax() { return m_stMgr->getPointSizeMax(); }
				virtual float getPointSizeMin() { return m_stMgr->getPointSizeMin(); }
				virtual bool getPointSpriteEnabled() { return m_stMgr->getPointSpriteEnabled(); }

				/************************************************************************/
				/* Stencil                                                              */
				/************************************************************************/
				virtual bool getStencilEnabled() { return m_stMgr->getStencilEnabled(); }
				virtual StencilOperation getStencilFail() { return m_stMgr->getStencilFail(); }
				virtual StencilOperation getStencilPass() { return m_stMgr->getStencilPass(); }
				virtual StencilOperation getStencilDepthFail() { return m_stMgr->getStencilDepthFail(); }
				virtual uint32 getStencilRefrence() { return m_stMgr->getStencilRefrence(); }
				virtual CompareFunction getStencilFunction() { return m_stMgr->getStencilFunction(); }
				virtual uint32 getStencilMask() { return m_stMgr->getStencilMask(); }
				virtual uint32 getStencilWriteMask() { return m_stMgr->getStencilWriteMask(); }

				virtual bool getTwoSidedStencilMode() { return m_stMgr->getTwoSidedStencilMode(); }
				
				virtual StencilOperation getCounterClockwiseStencilFail() { return m_stMgr->getCounterClockwiseStencilFail(); }
				virtual StencilOperation getCounterClockwiseStencilPass() { return m_stMgr->getCounterClockwiseStencilPass(); }
				virtual StencilOperation getCounterClockwiseStencilDepthBufferFail() { return m_stMgr->getCounterClockwiseStencilDepthBufferFail(); }

				virtual CompareFunction getCounterClockwiseStencilFunction()
				{ return m_stMgr->getCounterClockwiseStencilFunction(); }
				

				/************************************************************************/
				/* Scissor Test                                                         */
				/************************************************************************/

				virtual bool getScissorTestEnabled();
				virtual Apoc3D::Math::Rectangle getScissorTestRect();
				virtual void setScissorTest(bool enable, const Apoc3D::Math::Rectangle* rect);

				/************************************************************************/
				/* Color Write                                                          */
				/************************************************************************/
				virtual void getColorWriteEnabled(int rtIndex, bool& r, bool& g, bool& b, bool& a);
				virtual void setColorWriteEnabled(int rtIndex, bool r, bool g, bool b, bool a);

				/************************************************************************/
				/* Samplers                                                             */
				/************************************************************************/

				virtual void SetVertexSampler(int samplerIndex, const ShaderSamplerState& sampler) { m_stMgr->SetVertexSampler(samplerIndex, sampler); }
				virtual void SetPixelSampler(int samplerIndex, const ShaderSamplerState& sampler) { m_stMgr->SetPixelSampler(samplerIndex, sampler); }

				virtual const ShaderSamplerState& getPixelSampler(int samplerIndex) const { return m_stMgr->getPixelSampler(samplerIndex); }
				virtual const ShaderSamplerState& getVertexSampler(int samplerIndex) const { return m_stMgr->getVertexSampler(samplerIndex); }

			private:
				D3D9RenderDevice* m_device;

				NativeD3DStateManager* m_stMgr;

			};
		}
	}
}

#endif