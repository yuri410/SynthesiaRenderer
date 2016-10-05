#pragma once
#ifndef APOC3D_EFFECTSYSTEM_H
#define APOC3D_EFFECTSYSTEM_H

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

#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/HashMap.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Core;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace EffectSystem
		{
			/**
			 *  Keeps track of all shader effects.
			 *  All fx are recommended to be loaded at the beginning of your program.
			 */
			class APAPI EffectManager
			{
				SINGLETON_DECL(EffectManager);
			public:
				EffectManager() { }
				~EffectManager();

				bool HasEffect(const String& name) const;
				Effect* getEffect(const String& name) const;

				/** Loads all of a project's effects from a APBuild-generated xml list automatically. */
				void LoadEffectsFromList(RenderDevice* device, const ResourceLocation& rl);
				void LoadAutomaticEffect(RenderDevice* device, const ResourceLocation& rl);

				void RegisterCustomEffect(CustomShaderEffect* effect);

				void ReloadAutomaticEffects();
				
				void UnloadAllEffects();

				
				void Update(const GameTime* time);

				void FillEffects(List<Effect*>& list);

			private:
				typedef HashMap<String, Effect*> EffectTable;

				EffectTable m_fxTable;

				struct AutomaticEffectInfo
				{
					String Name;
					ResourceLocation* Location = nullptr;

					AutomaticEffectInfo() { }
					AutomaticEffectInfo(const String& name, const ResourceLocation* loc);
					~AutomaticEffectInfo();

					AutomaticEffectInfo(const AutomaticEffectInfo&) = delete;
					AutomaticEffectInfo& operator=(const AutomaticEffectInfo&) = delete;

					AutomaticEffectInfo(AutomaticEffectInfo&&);
					AutomaticEffectInfo& operator=(AutomaticEffectInfo&& rhs);
				};

				List<AutomaticEffectInfo> m_autoEffectInfo;
			};
		};
	};
};
#endif