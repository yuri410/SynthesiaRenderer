#pragma once
#ifndef APOC3D_GAMETIME_H
#define APOC3D_GAMETIME_H

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

#include "apoc3d/Common.h"

namespace Apoc3D
{
	namespace Core
	{
		/** Contains the current timing state of the game. */
		struct APAPI GameTime
		{
			GameTime();
			GameTime(float elapsedRTime, float fps);
			GameTime(float elapsedTime, float elapsedRTime, float elapsedTimeSubstep, int32 iteractionCount, float fps, bool isRenderingSlow);
			~GameTime();

			float getElapsedTime() const { return ElapsedTime; }

			float getElapsedRealTime() const { return ElapsedRealTime; }

			float getFPS() const { return FPS; }

			/**
			 * The elapsed game time since last update, in seconds.
			 *
			 * In fixed time step system, ElapsedTime is always the intended TargetElapsedTime,
			 * and Update is called 1 or more times to catch up with rendering overhead.
			 *
			 * Otherwise this is same as ElapsedRealTime.
			 */
			float ElapsedTime = 0;

			/**
			 * The elapsed real game time since last update, in seconds.
			 *
			 * This "real" version of ElapsedTime is intended for to get the real ElapsedTime instead of the TargetElapsedTime
			 * when fixed time step is used.
			 *
			 * In fixed time step system, Update() may be called more than one while Render() is only invoked once in a frame.
			 * ElapsedTime is always the intended TargetElapsedTime, and Update is called 1 or more times to catch up with rendering.
			 *
			 */
			float ElapsedRealTime = 0;

			float ElapsedTimeSubstep = 0;

			/** The number of iterations in a fixed time step system. */
			int32 IterationCount = 1;

			float FPS = 0;

			bool IsRenderingSlow = false;


			void Write(IO::BinaryWriter* bw);
			void Read(IO::BinaryReader* br);
		};
	};
};

#endif