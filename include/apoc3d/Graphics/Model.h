#pragma once
#ifndef APOC3D_MODEL_H
#define APOC3D_MODEL_H

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


#include "Renderable.h"
#include "RenderOperationBuffer.h"
#include "ModelTypes.h"
#include "Animation/AnimationTypes.h"
#include "apoc3d/Meta/EventDelegate.h"

#include "apoc3d/Core/Resource.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Core;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::Animation;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::VFS;
using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace Graphics
	{
		/** 
		 *  Model's geometry part, which is considered as resources of the vertex and index data.
         *  
		 *  Internally, models can share the same ModelSharedData, if the geometry is from the same source(i.e. the same model .mesh file).
		 *  At the same time, they can use different animation files. 
		 *
		 *  Model's data, ModelSharedData, consist of meshes and resource management operations.
		 *  
		 */
		class APAPI ModelSharedData : public Resource
		{
		public:
			ModelSharedData(RenderDevice* device, const List<Mesh*>& entities);
			ModelSharedData(RenderDevice* device, const ModelData* mdlData);
			ModelSharedData(RenderDevice* device, const ResourceLocation& rl, bool managed = true);
			virtual ~ModelSharedData();

			virtual uint32 getSize();

			List<Mesh*>& getEntities() { return m_entities; }

			void Save(ModelData* data);

		protected:
			virtual void load();
			virtual void unload();

		private:
			RenderDevice* m_renderDevice;

			List<Mesh*> m_entities;

			ResourceLocation* m_resourceLocation;
		};


		enum AnimationType
		{
			ANIMTYPE_Rigid = 1,
			ANIMTYPE_Skinned = 2,
			ANIMTYPE_Material = 4
		};

		typedef EventDelegate<AnimationType> ModelAnimationCompletedHandler;

		/**
		 *  A model is a set of subsets called Mesh, with additionally animation controller and data.
		 *   When loading model, ModelManager is recommended, the model manager will create ModelSharedData, which is then
		 *   used as Model's constructor's parameter.
		 */
		class APAPI Model : public Renderable
		{
		public:
			enum AnimationPlaybackState
			{
				APS_Playing,
				APS_Paused,
				APS_Stopped
			};

			/**
			 *  Initializes a new model from ModelSharedData and an optional AnimationData.
			 *   The AnimationData need to be deleted manually when no longer used, as the Model class will not modify it.
			 */
			Model(ResourceHandle<ModelSharedData>* data, const AnimationData* animData = 0);
			~Model();
		
			/**
			 *  Gets the ModelSharedData currently being used. 
			 *  The caller's thread will be suspended if ModelSharedData is loaded.
			 */
			ModelSharedData* GetData();

			void RebuildROPCache();

			virtual RenderOperationBuffer* GetRenderOperation(int lod);
			RenderOperationBuffer* GetRenderOperationSubEntity(int index);

			RenderOperationBuffer* GetRenderOperationSimple();
			RenderOperationBuffer* GetRenderOperationSubEntitySimple(int index);
			RenderOperationBuffer* GetRenderOperationSubEntityRaw(int index);


			/** The update will do the animation works if the model has animation. */
			void Update(const GameTime* time);

			void PlayAnimation(AnimationType type = (AnimationType)(ANIMTYPE_Material | ANIMTYPE_Rigid | ANIMTYPE_Skinned));
			void PauseAnimation(AnimationType type = (AnimationType)(ANIMTYPE_Material | ANIMTYPE_Rigid | ANIMTYPE_Skinned));
			void ResumeAnimation(AnimationType type = (AnimationType)(ANIMTYPE_Material | ANIMTYPE_Rigid | ANIMTYPE_Skinned));
			void StopAnimation(AnimationType type = (AnimationType)(ANIMTYPE_Material | ANIMTYPE_Rigid | ANIMTYPE_Skinned));

			void ReloadAnimation(AnimationType type);	
			void ReloadMaterialAnimation();
			void ReloadSkinAnimation();
			void ReloadRigidAnimation();

			void SetSelectedClipName(const String& name);

			const ModelAnimationClip* GetSkinAnimationSelectedClip() const;
			const ModelAnimationClip* GetRigidAnimationSelectedClip() const;
			const MaterialAnimationClip* GetMaterialDurationSelectedClip() const;
			
			float GetCurrentAnimationDuration(AnimationType type) const;
			bool HasAnimation(AnimationType type) const;
			
			int GetCurrentAnimationKeyFrame(AnimationType type);
			void SetCurrentAnimationKeyFrame(AnimationType type, int index);

			ModelAnimationCompletedHandler& eventAnimationCompeleted() { return m_eventAnimCompleted; }

			List<ModelAnimationPlayerBase*>& getCustomAnimation() { return m_animInstance; }

			AnimationPlaybackState getMaterialAnimationState() const { return m_mtrlState; }
			AnimationPlaybackState getSkinAnimationState() const { return m_skinState; }
			AnimationPlaybackState getRigidAnimationState() const { return m_rigidState; }
		private:
			enum AnimationControl
			{
				AC_Play,
				AC_Stop,
				AC_Resume,
				AC_Pause
			};

			void ControlSkinnedAnimation(AnimationControl ctrl);
			void ControlRigidAnimation(AnimationControl ctrl);
			void ControlMaterialAnimation(AnimationControl ctrl);

			void RigidAnim_Competed();
			void SkinAnim_Completed();
			void MtrlAnim_Completed();

			void InitializeAnimation();
			void UpdateAnimation();

			void BuildROPBuffer();


			/** Whether the RenderOperationBuffer is pre-calculated and stored. 
			 *  The RenderOperations inside are build first time the model is drawn for performance considerations.
			 *  This will not affect animation; because even though the RenderOperations are pre-calculated, the 
			 *  matrices are re-calculated based on animation each frame.
			 */
			bool m_isOpBufferBuilt;
			RenderOperationBuffer m_opBuffer;

			int* m_renderOpEntID;			/** table cast render operation index to entity index */
			int* m_renderOpEntPartID;		/** table cast render operation index to entity part index */

			/** states used for looping check. does not go public */
			bool m_rigidAnimCompleted;
			bool m_skinAnimCompleted;
			bool m_mtrlAnimCompleted;

			ResourceHandle<ModelSharedData>* m_data;
			const AnimationData* m_animData;

			SkinnedAnimationPlayer* m_skinPlayer;
			RigidAnimationPlayer* m_rigidPlayer;
			MaterialAnimationPlayer* m_mtrlPlayer;

			AnimationPlaybackState m_skinState;
			AnimationPlaybackState m_rigidState;
			AnimationPlaybackState m_mtrlState;

			List<ModelAnimationPlayerBase*> m_animInstance;

			bool m_autoLoop;
			String m_selectedClipName;

			ModelAnimationCompletedHandler m_eventAnimCompleted;

		};
	}
}
#endif