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
#ifndef APOC3D_WIN32_GAMECLOCK_H
#define APOC3D_WIN32_GAMECLOCK_H

#include "Win32Common.h"

namespace Apoc3D
{
	namespace Win32
	{
		/** A clock used to measure time with good accuracy */
		class GameClock
		{
		public:
			GameClock() { Reset(); }
			~GameClock() { }

			void Reset();

			/** Called every frame to get the time difference */
			void Step();

			double getElapsedTime() const { return m_elapsedTime; }
			double getCurrentTime() const { return m_currentTimeBase + m_currentTime; }

		private:
			bool CounterToTimeSpan(uint64 counter, uint64 base, double* result) const;

			uint64 m_baseCounter;
			uint64 m_lastCounter;

			uint64 m_frequency;

			double m_currentTimeBase;
			double m_currentTime;

			double m_elapsedTime;

			bool m_lastRealTimeValid;

		};
	}
};
#endif