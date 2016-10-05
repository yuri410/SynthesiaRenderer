#pragma once
#ifndef APOC3D_DEVICECONTEXT_H
#define APOC3D_DEVICECONTEXT_H

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

#include "apoc3d/Graphics/GraphicsCommon.h"
#include "apoc3d/Graphics/PixelFormat.h"
#include "apoc3d/Collections/List.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Graphics;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			struct RenderParameters
			{
				int32 AdapterIndex = 0;

				bool IsWindowed = false;

				/** Can the window created be resized. */
				bool IsFixedWindow = false;

				bool IgnoreMonitorChange = false;
				
				/** 
				 *  Represents when IsWindowd, whether
				 *  the engine should create a RenderWindow or(true), or
				 *  using a user specified render area as a RenderView(false).
				 */
				bool IsFullForm = false;

				int BackBufferWidth = 0;
				int BackBufferHeight = 0;

				DepthFormat DepthBufferFormat = DEPFMT_Depth16;
				PixelFormat ColorBufferFormat = FMT_Unknown;

				uint32 FSAASampleCount = 0;
				bool EnableVSync = false;
				bool TripleBuffering = false;

				bool IsMultithreaded = true;
				int32 RefreshRate = 0;

				/** 
				 *  When IsFullForm==false, this is a platform specific handle indicating the target area for presenting the render result.
				 */
				uint64 TargetHandle = 0;

				void* UserData = 0;

				RenderParameters() { }
			};

			struct RenderDisplayMode
			{
				int AdapterIndex;

				int Width;
				int Height;

				uint32 RefreshRate;
				uint32 FSAASampleCount;
				bool FullScreen;

				String GraphicsCardName;
				String DisplayName;
				List<String> MonitorNames;
			};


			/**
			 *  Represent a graphics device of a graphics API. It can create one or more RenderViews.
			 */
			class APAPI DeviceContext
			{
			public:
				/** Indicates whether if the API can draw in pre-existing windows or areas. */
				virtual bool SupportsRenderControl() { return m_supportsRenderControl; }

				/**
				 *  Creates a render view. The creation can create a render window or render view. 
				 *  See RenderParameters for details.
				 */
				RenderView* Create(const RenderParameters &pm);
				void Destroy(RenderView* rc);
				
				virtual List<RenderDisplayMode> GetSupportedDisplayModes() = 0;
				virtual String GetHardwareName() = 0;

				virtual RenderDevice* getRenderDevice() = 0;

			protected:
				virtual RenderView* create(const RenderParameters &pm) = 0;

				DeviceContext(bool supportsRenderCtrl)
					: m_supportsRenderControl(supportsRenderCtrl)
				{
				}

			private:
				List<RenderView*> m_renderPorts;
				bool m_supportsRenderControl;
			};
		}
	}
}

#endif