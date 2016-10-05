#pragma once
#ifndef APOC3D_ANIMATIONDATA_H
#define APOC3D_ANIMATIONDATA_H

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

#include "AnimationTypes.h"
#include "apoc3d/Collections/HashMap.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;
using namespace Apoc3D::IO;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace Animation
		{
			/** Combines all the animation data needed to animate a object. */
			class APAPI AnimationData
			{
			public:
				typedef HashMap<String, ModelAnimationClip*> ClipTable;
				typedef HashMap<String, MaterialAnimationClip*> MtrlClipTable;
			public:

				bool hasMtrlClip() const { return m_hasMtrlClip; }
				bool hasSkinnedClip() const { return m_hasSkinnedClip; }
				bool hasRigidClip() const { return m_hasRigidClip; }

				const List<Bone>& getBones() const { return m_bones; }
				const int32 getRootBone() const { return m_rootBone; }

				/**
				 *  Gets a collection of animation clips that operate a whole mesh entity in a model.
				 *  These are stored by name in a map, so there could for instance be 
				 *  clips for "Walk", "Run", "JumpReallyHigh", etc.
				 */
				const ClipTable& getRigidAnimationClips() const { return m_rigidAnimationClips; }

				/** Gets a collection of model animation clips. These are stored by name in a
				 *  map, so there could for instance be clips for "Walk", "Run",
				 *  "JumpReallyHigh", etc.
				 */
				const ClipTable& getSkinnedAnimationClips() const { return m_skinnedAnimationClips; }
				
				const MtrlClipTable& getMaterialAnimationClips() const { return m_mtrlAnimationClips; }

				const List<Bone>* getBones() { return &m_bones; }

				void setBones(const List<Bone>& bones)
				{
					m_bones = bones;
				}
				void setRigidAnimationClips(const ClipTable& table) { m_rigidAnimationClips = table; m_hasRigidClip = table.getCount() != 0;}
				void setSkinnedAnimationClips(const ClipTable& table) { m_skinnedAnimationClips = table; m_hasSkinnedClip = table.getCount() != 0; }
				void setMaterialAnimationClips(const MtrlClipTable& table) { m_mtrlAnimationClips = table; m_hasMtrlClip = table.getCount() != 0; }
				

				/**  Load animation data from a Tagged Data Block  */
				void ReadData(TaggedDataReader* data);
				TaggedDataWriter* WriteData() const;


				void Load(const ResourceLocation& rl);
				void Save(Stream& strm) const;

				AnimationData()
				{ }

				~AnimationData();

				AnimationData(const AnimationData&) = delete;
				AnimationData& operator=(const AnimationData&) = delete;

				int RigidEntityCount = 0;

			private:
				ClipTable m_rigidAnimationClips;
				ClipTable m_skinnedAnimationClips;
				MtrlClipTable m_mtrlAnimationClips;

				bool m_hasMtrlClip = false;
				bool m_hasSkinnedClip = false;
				bool m_hasRigidClip = false;


				List<Bone> m_bones;
				int32 m_rootBone = -1;

			};
		}
	}
}

#endif