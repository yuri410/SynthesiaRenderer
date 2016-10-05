#pragma once
#ifndef APOC3D_SCENERENDERSCRIPTPARSER_H
#define APOC3D_SCENERENDERSCRIPTPARSER_H

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

#include "apoc3d/Collections/HashMap.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::VFS;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Graphics::EffectSystem;

class TiXmlNode;
class TiXmlElement;
class TiXmlDocument;

namespace Apoc3D
{
	namespace Scene
	{
		//template class APAPI unordered_map<String, String>;
		typedef HashMap<String, String> BlockArgs;
		class APAPI SceneRenderScriptParser
		{
		public:
			List<ScenePassData> PassData;
			List<SceneVariable*> GlobalVars;

			SceneRenderScriptParser(RenderDevice* dev);

			/**
			 *  Build the script from xml config.
			 */
			void Parse(const ResourceLocation& rl);

			const String& getSceneName() const { return m_sceneName; }

		private:
			String m_sceneName;
			RenderDevice* m_renderDevice;
			
			/**
			 *  Build instructions for expressions
			 */
			void FillInstructions(const std::string& cmd, List<SceneInstruction>& instructions);
			/**
			 *  Build instructions for commands.
			 */
			void FillFunctionCall(const TiXmlElement* node, List<SceneInstruction>& instructions, const TiXmlDocument& doc);

			/**
			 *  Parse and add all the global vars to the list
			 */
			void ParseGlocalVarNode(const TiXmlElement* node, const TiXmlDocument& doc);

			/**
			 *  Build instructions for the RenderQuad command
			 */
			void FillRenderQuad(const TiXmlElement* node, List<SceneInstruction>& instructions, const TiXmlDocument& doc);
			/**
			 *  Build instructions for code blocks
			 */
			void BuildInstructions(const TiXmlElement* node, ScenePassData* data, const TiXmlDocument& doc);

			void BuildPass(const TiXmlElement* node, const TiXmlDocument& doc);
			void BuildNode(const TiXmlNode* node, const TiXmlDocument& doc);

			
		};
	}
}
#endif