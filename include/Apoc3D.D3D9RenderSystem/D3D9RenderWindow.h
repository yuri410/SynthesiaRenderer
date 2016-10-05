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
#ifndef D3D9RENDERWINDOW_H
#define D3D9RENDERWINDOW_H

#include "D3D9Common.h"
#include "D3D9RenderDevice.h"

#include "apoc3d/Graphics/RenderSystem/RenderWindow.h"
#include "apoc3d/Graphics/RenderSystem/RenderWindowHandler.h"

using namespace Apoc3D::Graphics;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			class D3D9RenderView : public RenderView
			{
			public:
				D3D9RenderView(D3D9RenderDevice* device, D3D9DeviceContext* dc, D3D9RenderViewSet* viewSet, IDirect3DSwapChain9* chain, const RenderParameters& pm);
				~D3D9RenderView();

				virtual void ChangeRenderParameters(const RenderParameters& params);

				virtual void Present(const GameTime* time);

			private:
				D3D9RenderViewSet* m_viewSet;
				D3D9RenderDevice* m_device;
				IDirect3DSwapChain9* m_swapChain;
				HANDLE m_controlHandle;

			};

			class D3D9RenderWindow final : public RenderWindow
			{
			public:
				D3D9RenderWindow(D3D9RenderDevice* device, D3D9DeviceContext* dc, const RenderParameters& pm);
				~D3D9RenderWindow();

				virtual void ChangeRenderParameters(const RenderParameters& params) override;

				virtual void Exit() override;
				virtual void Run() override;

				virtual void Minimize() override;
				virtual void Restore() override;
				virtual void Maximize() override;

				virtual String getTitle() override;
				virtual void setTitle(const String& name) override;

				virtual Size getClientSize() override;

				virtual bool getIsActive() const { return m_active; }

				virtual void SetVisible(bool v) override;
				virtual void SetupTimeStepMode(TimeStepMode type, float refFrameTime) override;
				virtual TimeStepMode GetCurrentTimeStepMode() override;

				void D3D9_Initialize(bool isDeviceReset);
				void D3D9_Finalize(bool isDeviceReset);

				void D3D9_OnDeviceLost();
				void D3D9_OnDeviceReset();

				void D3D9_LoadContent();
				void D3D9_UnloadContent();


				const String& getHardwareName() const { return m_hardwareName; }
				GameWindow* getWindow() const { return m_gameWindow; }


				CancellableEventHandler eventFrameStart;
				EventHandler eventFrameEnd;

			private:

				/** When derived, this method should be overridden to initialize the graphics device,
				*  with the specific parameters and settings provided.
				*  As GraphicsDeviceManager has creates the device, Game::LoadContent and Game::Initialize
				*  will be called.
				*/
				void D3D9_Create(const RenderParameters& params);

				/** This method will ask the GraphicsDeviceManager to release resources. And
				*  will cause the Game::UnloadContent to be called.
				*/
				void D3D9_Release();

				/** Enters the main loop. */
				void D3D9_MainLoop();

				/** Run one frame, which includes one render and X updates  */
				void D3D9_Tick();

				void D3D9_DrawFrame(const GameTime* time);

				/** This should be overridden to draw a frame */
				void D3D9_Render(const GameTime* time);
				/** This should be overridden to allow the game to run logic such as updating the world,
				*  checking for collisions, gathering input, playing audio and etc.
				*/
				void D3D9_Update(const GameTime* time);
				void D3D9_UpdateConstrainedVarTimeStep(const GameTime* time);

				
				bool D3D9_OnFrameStart();
				void D3D9_OnFrameEnd();

				
				void Window_ApplicationActivated();
				void Window_ApplicationDeactivated();
				void Window_Suspend();
				void Window_Resume();
				void Window_Paint();


				D3D9DeviceContext* m_dc;
				String m_hardwareName;

				GraphicsDeviceManager* m_graphicsDeviceManager;
				GameWindow* m_gameWindow;
				GameClock* m_gameClock;

				TimeStepMode m_timeStepMode = TimeStepMode::FixedStep;

				int32 m_maxSkipFrameCount = 10;
				int32 m_slowRenderFrameHits = 0;

				// fixed time step states
				float m_accumulatedDt_fixedStep = 0;



				float m_referenceElapsedTime = 1.0f / 60.0f;

				
				bool m_active = false;

			};
		}
	}
}

#endif