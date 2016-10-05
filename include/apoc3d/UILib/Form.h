#pragma once
#ifndef APOC3D_FORM_H
#define APOC3D_FORM_H

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

#include "Control.h"

using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace UI
	{
		enum BorderStyle
		{
			FBS_None,
			FBS_Fixed,
			FBS_Sizable,
			FBS_Pane
		};

		class APAPI Border
		{
		public:
			Border(BorderStyle style, const StyleSkin* skin);
			void Draw(Sprite* sprite, const Point& pt, const Point& size, float shadowAlpha);
			

		private:
			Apoc3D::Math::Rectangle m_dstRect[9];
			Apoc3D::Math::Rectangle m_srcRect[9];

			const StyleSkin* m_skin;
			BorderStyle m_style;

			void UpdateRects(const Point& position, const Point& size);
			void DrawUpper(Sprite* sprite);
			void DrawMiddle(Sprite* sprite);
			void DrawLower(Sprite* sprite);
			void DrawShadow(Sprite* sprite, const Point& pos, float alpha);

		};

		class APAPI Form : public ControlContainer
		{
			RTTI_DERIVED(Form, ControlContainer);
		public:
			enum WindowState
			{
				FWS_Normal,
				FWS_Minimized,
				FWS_Maximized
			};

		public:

			Form(const StyleSkin* skin, RenderDevice* device, BorderStyle border = FBS_Sizable, const String& title = L"");
			virtual ~Form();

			void ShowModal();
			virtual void Show();
			virtual void Hide();
			virtual void Close();
			virtual void Focus();
			virtual void Unfocus();
			virtual void Minimize();
			virtual void Maximize();
			virtual void Restore();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			int32 GetTitleBarHeight() const;

			RenderDevice* getRenderDevice() const { return m_device; }

			void setMinimumSize(const Point& size) { m_minimumSize = size; }
			void setMinimumSize(int32 w, int32 h) { m_minimumSize = Point(w,h); }

			void setMaximumSize(const Point& size) { m_maximumSize = size; }
			void setMaximumSize(int32 w, int32 h) { m_maximumSize = Point(w,h); }

			bool isResized() const { return m_isResizeing; }
			bool isDragged() const { return m_isDragging; }
			bool isMinimizing() const { return m_isMinimizing; }

			bool getHasMinimizeButton() const { return m_hasMinimizeButton; }
			void setHasMinimizeButton(bool val) { m_hasMinimizeButton = val; }

			bool getHasMaximizeButton() const { return m_hasMaximizeButton; }
			void setHasMaximizeButton(bool val) { m_hasMaximizeButton = val; }

			const String& getTitle() const { return m_title; }
			void setTitle(const String& txt) { m_title = txt; }

			void setSize(const Point& sz) { m_size = sz; }
			void setSize(int32 w, int32 h) { m_size = Point(w, h); }
			void setWidth(int32 w) { m_size.X = w; }
			void setHeight(int32 h) { m_size.Y = h; }

			WindowState getState() const { return m_state; }
			BorderStyle getBorderStyle() const { return m_borderStyle; }

			UIEventHandler eventResized;
			UIEventHandler eventClosed;


			/**
			 *  Background from will never cover other's on top
			 */
			bool IsBackgroundForm = false;

		private:
			void Initialize(const StyleSkin* skin);
			void InitializeButtons();

			void DrawTitle(Sprite* sprite);
			void DrawButtons(Sprite* sprite);

			void UpdateFocus();
			void UpdateStateAnimation();
			void CheckDragging();
			void CheckClick(const GameTime* time);
			void CheckResize();
			void ToggleWindowState();

			void SetElementsBasicStates();

			Apoc3D::Math::Rectangle GetDragArea() const;

			void btClose_Release(Button* sender);
			void btMinimize_Release(Button* sender);
			void btMaximize_Release(Button* sender);
			void btRestore_Release(Button* sender);

			RenderDevice* m_device = nullptr;
			const StyleSkin* m_skin = nullptr;

			Point m_titleOffset;
			Point m_minimumSize;
			Point m_minimizedSize;
			Point m_maximumSize;

			bool m_isMinimized = false;
			bool m_isMaximized = false;

			Point m_previousPosition;
			Point m_previousSize;
			/**
			 *  The position to display form title when minimized
			 */
			Point m_minimizedPos;
			Point m_maximizedPos;

			String m_title;


			Border* m_border = nullptr;
			float m_borderAlpha = 1;

			Button* m_btClose = nullptr;
			Button* m_btMinimize = nullptr;
			Button* m_btMaximize = nullptr;
			Button* m_btRestore = nullptr;

			bool m_hasMinimizeButton = true;
			bool m_hasMaximizeButton = true;

			
			bool m_isDragging = false;
			bool m_isResizeing = false;

			/** 
			 *  Tells if the form is on the minimizing animation process.
			 */
			bool m_isMinimizing = false;
			/**
			 *  Tells if current cursor position is in the form's resize area, where can be dragged to resize the form
			 */
			bool m_isInReiszeArea = false;

			/** Specify the cursor position difference since last frame when dragging or resizing the form.
			*/
			Point m_posOffset;
			Point m_oldSize;
			
			MultiClickChecker m_clickChecker;

			BorderStyle m_borderStyle;
			WindowState m_state = FWS_Normal;

			
		};

	}
}
#endif