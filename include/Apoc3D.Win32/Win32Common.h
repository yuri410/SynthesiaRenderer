#pragma once
#ifndef APOC3D_WIN32_COMMON_H
#define APOC3D_WIN32_COMMON_H

#include "apoc3d/Common.h"

#include <Windows.h>

#include "apoc3d/Meta/EventDelegate.h"

#pragma comment(lib, "Apoc3D.lib")

namespace Apoc3D
{
	typedef EventDelegate<bool*> CancellableEventHandler;
	typedef EventDelegate<> EventHandler;


	namespace Win32
	{
		extern const wchar_t WindowClass[];

		class GameClock;
		class GameWindow;

	}
}

#endif