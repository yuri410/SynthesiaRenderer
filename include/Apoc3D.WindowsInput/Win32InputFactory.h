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
#ifndef WIN32INPUTFACTORY_H
#define WIN32INPUTFACTORY_H

#include "WinInputCommon.h"
#include "apoc3d/Input/InputAPI.h"
#include "apoc3d/Math/Point.h"

using namespace Apoc3D::Math;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Input
	{
		namespace Win32
		{
			class Win32InputFactory : public InputAPIFactory
			{
			public:
				Win32InputFactory()
					: InputAPIFactory(GetDescription()), m_hwnd(0), m_InputManager(NULL)
				{
					m_instance = this;
				}
				~Win32InputFactory()
				{
					m_instance = 0;
				}

				virtual void Initialize(RenderWindow* window);
				virtual Mouse* CreateMouse();
				virtual Keyboard* CreateKeyboard();

			private:
				OIS::InputManager* m_InputManager;
				HWND m_hwnd;

				String m_tempTitleParam;
				Size m_tempClientSizeParam;

				BOOL EnumWindowsProc(_In_  HWND hwnd, _In_  LPARAM lParam);


				static Win32InputFactory* m_instance;
				static BOOL CALLBACK EnumWindowsProcStatic(_In_  HWND hwnd, _In_  LPARAM lParam)
				{
					if (m_instance)
					{
						m_instance->EnumWindowsProc(hwnd, lParam);
					}
					return FALSE;
				}

				static APIDescription GetDescription();

			};
		}
	}
}

#endif