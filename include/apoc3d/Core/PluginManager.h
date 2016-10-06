#pragma once
#ifndef APOC3D_PLUGINMANAGER_H
#define APOC3D_PLUGINMANAGER_H

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

using namespace Apoc3D::Platform;
using namespace Apoc3D::Collections;

namespace Apoc3D
{
	namespace Core
	{
		typedef fastdelegate::FastDelegate<void(const Plugin*, int32, int32)> PluginLoadCallBack;

		typedef fastdelegate::FastDelegate<void(const Plugin*)> PluginErrorBack;

		/** A 
		*/
		class APAPI PluginManager
		{
			SINGLETON_DECL(PluginManager);
		public:
			PluginManager();
			~PluginManager();

			PluginLoadCallBack& eventPluginLoad() { return m_eventPluginLoad; }
			PluginErrorBack& eventPluginError() { return m_eventPluginError; }

			void LoadPlugins(const List<Plugin*>& plugins);
			void LoadPlugins(const List<String>& plugins);
			void LoadPlugins();
			void UnloadPlugins();

			int32 getPluginCount() const { return m_plugins.getCount(); }
			
			const Plugin* getPlugin(const String& name) const;
			
		private:
			typedef HashMap<String, Plugin*> PluginTable;
			typedef List<Library*> LibraryList;

			void LoadPlugin(const String& name);

			void OnPluginLoad(const Plugin* plg, int32 index, int32 count);
			void OnPluginError(const Plugin* plg);
			
			PluginLoadCallBack m_eventPluginLoad;
			PluginErrorBack m_eventPluginError;

			PluginTable m_plugins;
			LibraryList m_libraries;

		};
	}
}

#endif