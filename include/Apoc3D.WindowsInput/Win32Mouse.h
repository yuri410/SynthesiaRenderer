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
#ifndef WIN32MOUSE_H
#define WIN32MOUSE_H

#include "WinInputCommon.h"
#include "apoc3d/Input/Mouse.h"

using namespace Apoc3D::Core;

namespace Apoc3D
{
	namespace Input
	{
		namespace Win32
		{
			class OldSchoolMouse : public Mouse
			{
			public:
				OldSchoolMouse(HWND hwnd);
				~OldSchoolMouse();

				virtual void SetPosition(const Point& loc) override;

				void Update(const GameTime* time);
			private:
				int m_accumlatedMouseWheel;

				LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
				LONG m_oldWndProc;
				HWND m_hwnd;

				static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
				{
					if (m_instance)
					{
						return m_instance->WndProc(hWnd, message, wParam, lParam);
					}
					return 0;
				}

				static OldSchoolMouse* m_instance;
			};

			class Win32Mouse : public Mouse, public OIS::MouseListener
			{
			public:
				Win32Mouse(OIS::InputManager* manager);
				~Win32Mouse();

				virtual void SetPosition(const Point& loc) override;

				void Update(const GameTime* time);

				bool mouseMoved( const OIS::MouseEvent &arg );
				bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
				bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

			private:
				OIS::InputManager* m_inpMgr;
				OIS::Mouse* m_mouse;
			};
		}
	}
}

#endif