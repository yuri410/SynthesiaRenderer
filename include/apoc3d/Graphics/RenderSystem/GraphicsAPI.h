#pragma once
#ifndef APOC3D_GRAPHICSAPI_H
#define APOC3D_GRAPHICSAPI_H

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
#include "apoc3d/Platform/API.h"

using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;
using namespace Apoc3D::Platform;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			/**
			 *  Manages all registered graphics APIs. 
			 *  Creates device content best suited for the platform.
			 */
			class APAPI GraphicsAPIManager
			{
				SINGLETON_DECL(GraphicsAPIManager);

			public:
				GraphicsAPIManager() { }
			
				virtual ~GraphicsAPIManager();

				void RegisterGraphicsAPI(GraphicsAPIFactory* fac);
				void UnregisterGraphicsAPI(const String& name);
				void UnregisterGraphicsAPI(GraphicsAPIFactory* fac);
				
				DeviceContext* CreateDeviceContext();

			private:
				/**
				 *  Obtains the information for each graphics API.h
				 *  Platform mark is the API's score indicating how the API fits the current platform running on, scaled to 0-100.
				 */
				struct Entry
				{
					GraphicsAPIFactory* Factory;
					int PlatformMark;

				};
				typedef List<Entry> APIList;
				typedef HashMap<String, APIList*> PlatformTable;
				PlatformTable m_factories;
				
			};


			/** Factory that create device context of particular Graphics API. */
			class APAPI GraphicsAPIFactory
			{
			public:
				const APIDescription& getDescription() const { return m_description; }

				/** Gets the name of the graphics API. */
				const String& getName() { return m_description.Name; }

				/** Create the device content of the Graphics API. */
				virtual DeviceContext* CreateDeviceContext() = 0;


			protected:
				GraphicsAPIFactory(const APIDescription& desc)
					: m_description(desc)
				{ }
			private:
				APIDescription m_description;


			};
		}
	}
}

#endif