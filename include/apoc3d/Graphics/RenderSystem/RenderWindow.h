#pragma once
#ifndef APOC3D_RENDERWINDOW_H
#define APOC3D_RENDERWINDOW_H

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

#include "DeviceContext.h"

#include "apoc3d/Collections/Queue.h"

using namespace Apoc3D::Math;
using namespace Apoc3D::Core;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			/** Implements a simple FPS counter */
			class APAPI FPSCounter
			{
			public:
				FPSCounter() { }

				void Step(const GameTime* time);
				void Step(float dt);

				float getFPS() const { return m_fps; }

			private:
				Queue<float> m_frameTimes;
				float m_fps = 0;
				float m_currentWindowTimePos = 0;
			};

			/** Represents a view of rendered graphics. */
			class APAPI RenderView
			{
				RTTI_BASE;
			public:
				virtual ~RenderView(){}

				virtual void Present(const GameTime* time);
				/**
				 *  Change the device settings. 
				 *  This may cause the device to be reseted/recreated depending on how big the change is.
				 */
				virtual void ChangeRenderParameters(const RenderParameters& params) { m_presentParams = params; }

				void* UserData = nullptr;

				RenderDevice* getRenderDevice() const { return m_renderDevice; }
				const RenderParameters& getRenderParams() const { return m_presentParams; }
				DeviceContext* getDeviceContext() const { return m_deviceContext; }

				float getFPS() const { return m_fpsCounter.getFPS(); }

			protected:
				RenderDevice* m_renderDevice;

				RenderView(DeviceContext* dc, RenderDevice* rd, const RenderParameters &pm, RenderTarget* rt)
					: m_presentParams(pm), m_renderDevice(rd), m_renderTarget(rt), m_deviceContext(dc)
				{
				}
				RenderView(DeviceContext* dc, RenderDevice* rd, const RenderParameters &pm)
					: m_presentParams(pm), m_renderDevice(rd), m_deviceContext(dc)
				{

				}

			private:
				RenderTarget* m_renderTarget = nullptr;
				RenderParameters m_presentParams;

				FPSCounter m_fpsCounter;
				DeviceContext* m_deviceContext;

			};

			enum struct TimeStepMode
			{
				Raw,
				Constrained,
				FixedStep
			};

			/** Represents a window with rendered graphics displayed in its client area. */
			class APAPI RenderWindow : public RenderView
			{
				RTTI_DERIVED(RenderWindow, RenderView);
			public:
				virtual ~RenderWindow();

				/** Quit the rendering app */
				virtual void Exit() { m_isExiting = true; }
				/**
				 *  Enter the main loop. 
				 *  This also include the initialization and finalization in the event handler.
				 */
				virtual void Run() = 0;

				virtual void Minimize() = 0;
				virtual void Restore() = 0;
				virtual void Maximize() = 0;

				virtual Size getClientSize() = 0;
				virtual String getTitle() = 0;
				virtual void setTitle(const String& name) = 0;

				virtual void SetupTimeStepMode(TimeStepMode type, float refFrameTime = 1.0f / 60.0f) = 0;
				virtual TimeStepMode GetCurrentTimeStepMode() = 0;

				virtual void SetVisible(bool v) = 0;


				void setInactiveSleepTime(int32 ms) { m_inactiveSleepTime = ms; }
				int32 getInactiveSleepTime() const { return m_inactiveSleepTime; }

				/** Represents if the window is activated. */
				virtual bool getIsActive() const = 0;

				bool getVisisble() const { return m_visisble; }

				bool getIsExiting() const { return m_isExiting; }

				void setEventHandler(RenderWindowHandler* handler) { m_evtHandler = handler; }

			protected:

				RenderWindow(DeviceContext* dc, RenderDevice* rd, const RenderParameters &pm, RenderTarget* rt)
					: RenderView(dc, rd, pm, rt) { }

				RenderWindow(DeviceContext* dc, RenderDevice* rd, const RenderParameters &pm)
					: RenderView(dc, rd, pm) { }
				
				void OnFrameStart();
				void OnFrameEnd();

				void OnInitialize();
				void OnFinalize();
				void OnLoad();
				void OnUnload();
				void OnUpdate(const GameTime* time);
				void OnUpdateConstrainedVarTimeStep(const GameTime* time);
				void OnDraw(const GameTime* time);

				RenderWindowHandler* m_evtHandler = nullptr;
				bool m_isExiting = false;

				bool m_visisble = true;

				int32 m_inactiveSleepTime = 20;

			};

		}
	}
}

#endif