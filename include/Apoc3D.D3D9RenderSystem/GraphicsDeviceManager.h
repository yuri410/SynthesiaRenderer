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
#ifndef APOC3D_D3D9_GRAPHICSDEVICEMANAGER_H
#define APOC3D_D3D9_GRAPHICSDEVICEMANAGER_H

#include "D3D9Common.h"
#include "RawSettings.h"

using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			/**
			 *  A graphics device manager manages the device. It not just takes care of creation 
			 *  with setting enumeration for a more reliable starting up,
			 *  but also handle the error and changes happened like DeviceLost.
			 */
			class GraphicsDeviceManager
			{
			public:
				const RawSettings* getCurrentSetting() const { return m_currentSetting; }

				GraphicsDeviceManager(D3D9RenderWindow* game, IDirect3D9* d3d9);
				~GraphicsDeviceManager();

				bool& UserIgnoreMonitorChanges() { return m_userIgnoreMoniorChanges; }

				/* Check if the device is properly initialized and ready to render. */
				bool IsDeviceReady() const { return m_device && !m_deviceLost; }

				/** Changes the device.
					param
					@settings The settings.
					@minimumSettings The minimum settings.
				*/
				void ChangeDevice(const RenderParameters& settings, const RenderParameters* minimumSettings);
				
				/** Changes the device.
					param
					@windowed if set to true, the application will run in windowed mode instead of full screen.
					@desiredWidth Desired width of the window.
					@desiredHeight Desired height of the window.
				*/
				void ChangeDevice(bool windowed, int desiredWidth, int desiredHeight);

				/** Changes the device.
					param
					@settings The settings.
				*/
				void ChangeDevice(const RenderParameters& settings);

				/* Toggles between full screen and windowed mode. */
				void ToggleFullScreen();
				void ReleaseDevice(bool isDeviceReset);

				D3DDevice* getDevice() const { return m_device; }
				IDirect3D9* getDirect3D() const { return m_direct3D9; }

			private:
				void PropogateSettings();
				bool CanDeviceBeReset(const RawSettings* oldset, const RawSettings* newset) const;
				void CreateDevice(const RawSettings &settings);
				void ChangeDevice(const RawSettings& settings);

				void game_FrameStart(bool* cancel);
				void game_FrameEnd();
				void Window_UserResized();
				void Window_MonitorChanged();

				void InitializeDevice(bool isDeviceReset);
				HRESULT ResetDevice();

				int32 GetAdapterOrdinal(HMONITOR mon);
				void UpdateDeviceInformation();
								
				RawSettings* m_currentSetting = nullptr;

				IDirect3D9* m_direct3D9;
				D3DDevice* m_device = nullptr;

				D3D9RenderWindow* m_game;

				bool m_userIgnoreMoniorChanges = false;

				bool m_ignoreSizeChanges = false;
				bool m_deviceLost = false;

				int32 m_fullscreenWindowWidth;
				int32 m_fullscreenWindowHeight;
				int32 m_windowedWindowWidth;
				int32 m_windowedWindowHeight;
				WINDOWPLACEMENT m_windowedPlacement;
				int64 m_windowedStyle;
				//bool m_savedTopmost


			};
		}
	}
};
#endif