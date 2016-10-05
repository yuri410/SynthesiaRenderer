#pragma once
#ifndef APOC3D_ENGINE_H
#define APOC3D_ENGINE_H

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


#include "Common.h"
#include "Collections/List.h"

using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;

namespace Apoc3D
{
	/**
	 *  The engine's initialization config. 
	 *  When this is specified, this will override the engine's default settings.
	 */
	struct APAPI ManualStartConfig
	{
		/**
		 *  A list of dir of config files which is used as basic configuration 
		 *  and loaded, putted into ConfigurationManager.
		 *  ConfigurationManager::getConfiguration can be used to retrive theses configs
		 *  later.
		 */
		List<String> ConfigSet;

		/**
		 *  A list of absolute directories to be used in the engine's file system.
		 */
		List<String> WorkingDirectories;
		/**
		 *  A list of file names of plugin dynamic libraries to be loaded.
		 */
		List<String> PluginDynLibList;
		/**
		 *  A list of user specified pre-loaded plugins.
		 */
		List<Plugin*> PluginList;

		/**
		 *  Specified whether the incoming new log entries should be forwarded to standard output.
		 *  Useful when debugging.
		 */
		bool WriteLogToStd;

		/** 
		 *  Specified whether the TextureManager will be actuated to use async processing
		 */
		bool TextureAsync;
		/**
		 *  If using async processing, specifies the total cache size in bytes.
		 */
		uint TextureCacheSize;

		/**
		 *  Specified whether the ModelManager will be actuated to use async processing
		 */
		bool ModelAsync;
		/**
		 *  If using async processing, specifies the total cache size in bytes.
		 */
		uint ModelCacheSize;

		ManualStartConfig()
			: TextureCacheSize(1024*1024*100), ModelCacheSize(1024*1024*50), WriteLogToStd(false),
			TextureAsync(true), ModelAsync(true)
		{

		}
	};

	/**
	 *  Includes elementary operation on the engine like Initialize and Showdown
	 */
	namespace Engine
	{
		/** 
		 *  Initializes all fundamental services like resource management, file system, plugins, graphics API..
		 *  to prepare the engine ready for use.
		 */
		APAPI void Initialize(const ManualStartConfig* mconf = 0);

		/**
		 *  Releases all services, freeing the hardware resources used.
		 */
		APAPI void Shutdown();
	};
}

#endif