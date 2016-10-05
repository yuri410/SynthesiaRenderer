#pragma once
#ifndef APOC3D_SCENE_PROCEDURE_H
#define APOC3D_SCENE_PROCEDURE_H

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

#include "ScenePassTypes.h"

#include "apoc3d/Math/Vector.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::VFS;
using namespace Apoc3D::Math;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Graphics::EffectSystem;

namespace Apoc3D
{
	namespace Scene
	{
		/* 
			pass config <Pass></Pass>
				attributes:
				Name
				SelectorID  -- use -1 to select all objects
				CameraID  --  index of camera registered in SceneRenderer object, try not to change camera too often


			Commands (aka. function call)
				<C S = "func" params/>

		*/

		/**
		 *  Represent a sequence of scene passes that can finally 
		 *  generate the end result.
		 *
		 *  A SceneProcedure can be either normal passes (like shadow
		 *  mapping) or post effect passes (like bloom & HDR).
		 *
		 *  This class also manages resources and vars used in rendering.
		 * 
		 * @remarks
		 *   For the structure or the use of the script, please refer
		 *   to the sample file.
		 *   
		 *   Commands(aka. function call):
		 *    Clear
		 *    UseRT
		 *    Render
		 *    RenderQuad
		 *    VisibleTo
		 *    State
		 *
		 *   Variable Types
		 *    GaussBlurFilter - see VARTYPE_GaussBlurFilter
		 *    Texture - see VARTYPE_Texture
		 *    RenderTarget - see VARTYPE_RenderTarget
		 *    Matrix - see VARTYPE_Matrix
		 *
		 *   Immediate Value Types
		 *
		 *
		 */
		class APAPI SceneProcedure
		{
		public:
			SceneProcedure(RenderDevice* device);
			~SceneProcedure();

			/**
			 *  Check if the SceneProcedure is available. A SceneProcedure is unavailable if the techniques
			 *  used is not supported. (eg. A type of render target format is not supported)
			 */
			bool IsAvailable();

			void Load(SceneRenderer* renderer, const ResourceLocation& rl);

			/** Execute the procedure, respectively invoking a series of Scene Passes. */
			void Invoke(const List<Camera*> cameras, SceneManager* sceMgr, BatchData* batchData);
			
			/**
			 *  Gets the current camera in the rendering process.
			 *  If this method is call when not rendering, eg. updating, the last camera use will be returned.
			 */
			const Camera* getLastCamera() const { return m_lastCamera; }

			ScenePass* getPass(int32 index) const { return m_passes[index]; }
			int32 getPassCount() const { return m_passes.getCount(); }
			
			/**
			 *  Find a variable define in the procedure script by name; 
			 *  then returns as a RenderTarget.
			 */
			RenderTarget* FindRenderTargetVar(const String& name) const;
			DepthStencilBuffer* FindDepthStencilVar(const String& name) const;

			void SetTextureVar(const String& name, ResourceHandle<Texture>* tex);
			void SetBooleanVar(const String& name, bool val);
			void SetVector4Var(const String& name, const Vector4& val);
			void SetVector2Var(const String& name, const Vector2& val);
			void SetFloatVar(const String& name, const float val);

		private:
			struct ProcGaussBlurFilter
			{
				GaussBlurFilter* Object;
				bool HasPercentageLock;
				float WidthPercentage;
				float HeightPercentage;
			};
			void CheckDimensions();

			RenderDevice* m_renderDevice;

			List<ScenePass*> m_passes;
			List<SceneVariable*> m_variables;

			List<RenderTarget*> m_createdRenderTarget;
			List<DepthStencilBuffer*> m_createDepthStencil;
			List<ResourceHandle<Texture>*> m_createdTextures;
			List<ProcGaussBlurFilter> m_createdGaussFilters;

			bool m_isAvailable = false;
			String m_name;

			const Camera* m_lastCamera;
		};
	};
};
#endif