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
#ifndef WIN32KEYBOARD_H
#define WIN32KEYBOARD_H

#include "WinInputCommon.h"
#include "apoc3d/Input/Keyboard.h"

using namespace Apoc3D::Core;

namespace Apoc3D
{
	namespace Input
	{
		namespace Win32
		{
			class Win32Keyboard : public Keyboard, public OIS::KeyListener
			{
			private:
				OIS::InputManager* m_inpMgr;
				OIS::Keyboard* m_keyboard;

			public:
				Win32Keyboard(OIS::InputManager* mgr);
				~Win32Keyboard();

				virtual void Update(const GameTime* time);


				bool keyPressed( const OIS::KeyEvent &arg );
				bool keyReleased( const OIS::KeyEvent &arg );
			};
		}
	}
}

#endif