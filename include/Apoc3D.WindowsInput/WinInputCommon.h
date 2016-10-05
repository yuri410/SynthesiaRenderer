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
#ifndef WININPUTCOMMON_H
#define WININPUTCOMMON_H

#include "apoc3d/Common.h"

#pragma warning ( push )
#pragma warning ( disable:4512 )

#include <ois-v1-3/includes/OIS.h>
#include <ois-v1-3/includes/OISInputManager.h>
#include <ois-v1-3/includes/OISException.h>
#include <ois-v1-3/includes/OISMouse.h>
#include <ois-v1-3/includes/OISKeyboard.h>
#include <ois-v1-3/includes/OISJoyStick.h>
#include <ois-v1-3/includes/OISEvents.h>

#pragma warning (pop)


#include <Windows.h>

#pragma comment(lib, "Apoc3D.lib")
#pragma comment(lib, "Apoc3D.Win32.lib")

#ifdef _DEBUG
#	ifdef APOC3D_MT
#		pragma comment(lib, "OIS_static_d_mt.lib")
#	else
#		pragma comment(lib, "OIS_static_d.lib")
#	endif
#else
#	ifdef APOC3D_MT
#		pragma comment(lib, "OIS_static_mt.lib")
#	else
#		pragma comment(lib, "OIS_static.lib")
#	endif
#endif

namespace Apoc3D
{
	namespace Input
	{
		namespace Win32
		{
			class WinInputPlugin;
			class Win32InputFactory;
			class Win32Mouse;
			class Win32Keyboard;
		}
	}
}


#endif