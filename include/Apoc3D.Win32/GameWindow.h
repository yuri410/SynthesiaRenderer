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
#ifndef APOC3D_WIN32_GAMEWINDOW_H
#define APOC3D_WIN32_GAMEWINDOW_H

#include "Win32Common.h"

#include "apoc3d/Math/Point.h"

using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Win32
	{
		/* Implements a specialized window for games and rendering. */
		class GameWindow
		{
		public:
			GameWindow(const String& name, const String& wndTitle);
			~GameWindow();

			/** Creates a win32 window using the given parameters. */
			void Load(int32 width, int32 height, bool fixed);

			void Close();

			/** Enable/disable the maximum button based on the given value */
			void MakeFixedSize(bool v);

			String getWindowTitle() const { return m_title; }
			void setWindowTitle(const String& txt);
			/** Gets the current client size */
			Size getCurrentSize() const;

			HWND getHandle() const { return m_hWnd; }

			/** Gets the mouse wheel amount from window message */
			int32 getMouseWheel() const { return m_mouseWheel; }
			bool getIsMinimized() const { return m_minimized; }
			bool getIsMaximized() const { return m_maximized; }

			EventHandler eventPaint;
			EventHandler eventResume;
			EventHandler eventUserResized;
			EventHandler eventSuspend;
			EventHandler eventApplicationActivated;
			EventHandler eventApplicationDeactivated;
			EventHandler eventSystemSuspend;
			EventHandler eventSystemResume;
			EventHandler eventMonitorChanged;
			CancellableEventHandler eventScreensaver;

		protected:
			virtual void OnUserResized();
			virtual void OnSuspend();
			virtual void OnApplicationActivated();
			virtual void OnApplicationDeactivated();
			virtual void OnSystemSuspend();
			virtual void OnSystemResume();
			virtual void OnScreensaver(bool * cancel);
			virtual void OnResume();
			virtual void OnPaint();

			virtual void OnMonitorChanged();

		private:
			static GameWindow* ms_Window;

			BOOL InitInstance(HINSTANCE hInstance, int32 width, int32 height, bool fixed, const String& wndClass, const String& wndTitle);

			static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			void UpdateMonitor();

			bool m_minimized = false;
			bool m_maximized = false;
			bool m_inSizeMove = false;
			String m_title;
			String m_className;
			Size m_cachedSize;
			HINSTANCE m_hInst;

			HWND m_hWnd;
			HINSTANCE m_hInstance;

			HMONITOR m_currentMonitor;


			int32 m_mouseWheel = 0;

		};
	}

};
#endif