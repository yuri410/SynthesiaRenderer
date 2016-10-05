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
#ifndef D3D9RENDERVIEWSET
#define D3D9RENDERVIEWSET

#include "D3D9Common.h"
#include "RawSettings.h"
#include "Apoc3D/Collections/List.h"
#include "apoc3d/Graphics/RenderSystem/RenderWindow.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			class D3D9RenderViewSet
			{

			public:
				const RawSettings* getCurrentSetting() const { return m_currentSetting; }

				D3D9RenderViewSet(IDirect3D9* d3d9);
				~D3D9RenderViewSet();

				D3DDevice* getDevice() const { return m_device; }
				IDirect3D9* getDirect3D() const { return m_direct3D9; }


				void AddControl(const RenderParameters& pm);
				void NotifyControlRemoved(const D3D9RenderView* view);

				/* Ensures that the device is properly initialized and ready to render.
				*/
				bool EnsureDevice() const
				{
					return m_device && !m_deviceLost;
				}

				/** Changes the device.
					param
					@settings The settings.
					@minimumSettings The minimum settings.
				*/
				void ChangeDevice(const RenderParameters& settings, const RenderParameters* minimumSettings);

			private:
				RawSettings* m_currentSetting = nullptr;

				List<D3D9RenderView*> m_views;
				IDirect3D9* m_direct3D9;
				D3DDevice* m_device = nullptr;
				D3D9RenderDevice* m_apiDevice;

				bool m_deviceLost = false;
				bool m_ignoreSizeChanges = false;


				void Control_BeginPaint(bool* cancel);
				void Control_EndPaint();

				void Control_UserResized();
				void Control_MonitorChanged();

				int32 GetAdapterOrdinal(HMONITOR mon);

				void PropogateSettings();
				bool CanDeviceBeReset(const RawSettings* const oldset,
					const RawSettings* const newset) const;

				HRESULT ResetDevice();
				void CreateDevice(const RawSettings &settings);
				void ChangeDevice(const RawSettings& settings);

				void InitializeDevice();
				void ReleaseDevice();

			};
		}
	}
}
#endif