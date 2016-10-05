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

#ifndef APOC3DEX_GAME_H
#define APOC3DEX_GAME_H

#include "EssentialCommon.h"

#include "apoc3d/Core/GameTime.h"
#include "apoc3d/Graphics/RenderSystem/RenderWindowHandler.h"
#include "apoc3d/Input/InputAPI.h"


using namespace Apoc3D;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Math;
using namespace Apoc3D::UI;
using namespace Apoc3D::Input;

namespace Apoc3DEx
{
	/** A class as a framework, which looks similar to XNA's, but with the whole engine's support.
	 *  This class is the event handler for all messages from render window. The game's drawing, updating in the main loop and initialization are
	 *  all handled in this class.
	 *
	 */
	class APEXAPI Game : public RenderWindowHandler
	{
	public:
		Game(RenderWindow* wnd);
		virtual ~Game();

		/** This is the place to load or initialize something before any other 
		 *  events being handled. Remember to call this if overrided.
		 */
		virtual void Initialize();
		/** This is opposed to Initialize, which is called last(after Unload)
		 *  and is supposed release the resources created in Initialize here.
		 */
		virtual void Finalize();

		/** Load will be called to load contents. 
		 *  Graphics content should be loaded here, if there are any to load as the game starts up.
		 */
		virtual void Load();

		/** Opposite to Load, this is the place to unload content, called once the game window is closed.
		*/
		virtual void Unload();

		/** Allows the game to run logic such as updating the world,
		 *  checking for collisions, gathering input, playing audio and etc.
		 */
		virtual void Update(const GameTime* time);

		/** This is called when the game should draw a frame.
		*/
		virtual void Draw(const GameTime* time);

		/** This is called before drawing a new frame.
		*/
		virtual void OnFrameStart();
		/** This is called after a frame is drawn.
		*/
		virtual void OnFrameEnd();

		/** Closes the window
		*/
		void Exit();

		RenderDevice* getRenderDevice() const { return m_device; }
		RenderWindow* getWindow() const { return m_window; }

		const StyleSkin* getSystemUISkin() const { return m_UIskin; }

		Console* getConsole() const { return m_console; }

	protected:
		const GameTime* GetRecordCorrectedTime(const GameTime* time);

		RenderWindow* m_window;
		RenderDevice* m_device = nullptr;

		StyleSkin* m_UIskin = nullptr;
		Console* m_console = nullptr;

		Input::InputPlayer* m_player = nullptr;
		Input::InputRecorder* m_recorder = nullptr;
		GameTime m_correctedFrameTime;

		InputCreationParameters m_inputCreationParam;
	};
}
#endif