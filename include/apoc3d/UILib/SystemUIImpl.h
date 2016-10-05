#pragma once
#ifndef APOC3D_UI_UIROOTIMPL_H
#define APOC3D_UI_UIROOTIMPL_H

#include "UICommon.h"

using namespace Apoc3D::Core;

namespace Apoc3D
{
	namespace UI
	{
		class SystemUIImpl
		{
		public:
			SystemUIImpl();
			~SystemUIImpl();

			void Initialize(RenderDevice* device);
			void Finalize();

			void Add(ControlContainer* cc);
			void Remove(ControlContainer* cc);
			void RemoveForm(const String& name);
			void RemoveContainer(const String& name);
			void ScheduleBringFront(ControlContainer* cc);

			void Draw();
			void Update(const GameTime* time);

			Point ClampFormMovementOffset(Form* frm, const Point& vec);


			/** Return the area to display UI in the viewport. In screen coordinates. */
			Apoc3D::Math::Rectangle GetUIArea(RenderDevice* device);

			List<Form*>& getForms() { return m_forms; }

			bool GetMinimizedPosition(RenderDevice* dev, Form* form, Point& pos);
			Point GetMaximizedSize(RenderDevice* dev, Form* form);
			Apoc3D::Math::Rectangle GetMaximizedRect(RenderDevice* dev, Form* form);

			Texture* getWhitePixel() const { return m_white1x1; }

			Form* InteractingForm = nullptr;
			Form* TopMostForm = nullptr;
			Form* m_modalForm = nullptr;

			MenuBar* m_mainMenu = nullptr;
			
			/** Specifies the area to display UI in the viewport. In unified coordinates. */
			RectangleF UIArea = RectangleF(0, 0, 1, 1);

			RectangleF MaximizedArea = RectangleF(0, 0, 1, 1);

		private:
			void Form_SizeChanged(Form* frm);

			List<Form*> m_forms;
			List<ControlContainer*> m_containers;

			List<ControlContainer*> m_bringFrontSchedules;

			SubMenu* m_contextMenu = nullptr;
			Sprite* m_sprite = nullptr;
			int m_modalAnim = 0;

			Texture* m_white1x1 = nullptr;
		};


	}
}

#endif
