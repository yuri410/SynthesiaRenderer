#pragma once
#ifndef APOC3D_BUTTON_H
#define APOC3D_BUTTON_H

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
#include "apoc3d/Math/ColorValue.h"

using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace UI
	{
		class APAPI Button : public Control
		{
			RTTI_DERIVED(Button, Control);
		public:
			typedef EventDelegate<Button*> ButtonEvent;

			Button(const ButtonVisualSettings& settings, const Point& position, const String& text);
			Button(const ButtonVisualSettings& settings, const Point& position, int width, const String& text);
			Button(const ButtonVisualSettings& settings, const Point& position, int width, int height, const String& text);
			Button(const ButtonVisualSettings& settings, const Point& position, const Point& size, const String& text);

			Button(const StyleSkin* skin, const Point& position, const String& text);
			Button(const StyleSkin* skin, const Point& position, int width, const String& text);
			Button(const StyleSkin* skin, const Point& position, int width, int height, const String& text);
			Button(const StyleSkin* skin, const Point& position, const Point& size, const String& text);
			virtual ~Button();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			void Resetup(const ButtonVisualSettings& settings);
			void RunAutosize();

			void SetFont(Font* fontRef);
			void SetText(const String& text);

			void SetSize(int32 w, int32 h) { SetSize(Point(w, h)); }
			void SetSize(const Point& sz) { AutoSizedX = AutoSizedY = false; m_size = sz; }
			void SetSizeX(int32 sz) { AutoSizedX = false; m_size.X = sz; }
			void SetSizeY(int32 sz) { AutoSizedY = false; m_size.Y = sz; }

			int32 EvaluateAutoWidth() const;
			Point EvaluateAutoSize() const;

			const String& getText() const { return m_text; }
			bool isMouseHover() const { return m_mouseOver; }
			
			UIGraphic DisabledGraphic;
			UIGraphic NormalGraphic;
			UIGraphic MouseHoverGraphic;
			UIGraphic MouseDownGraphic;
			
			UIGraphicSimple OverlayIcon;
			UIGraphicSimple DisabledOverlayIcon;

			float Rotation = 0;

			bool IsSwitchMode = false;
			bool IsSwitchedOn = false;

			bool AutoSizedX = false;
			bool AutoSizedY = false;
			bool ForceMouseDownVisual = false;
			
			bool RaiseGlobalEvent = true;

			bool OmitsText = false;

			TextRenderSettings TextSettings;

			ControlBounds Margin;

			ButtonEvent eventMouseHover;
			ButtonEvent eventMouseOut;
			ButtonEvent eventPress;
			ButtonEvent eventRelease;

			static ButtonEvent eventAnyPress;
			static ButtonEvent eventAnyRelease;
		protected:

			bool m_mouseOver = false;
			bool m_mouseDown = false;

		private:
			void DrawDefaultButton(Sprite* spriteBatch);
			void DrawTextureButton(Sprite* spriteBatch);
			void DrawButtonCore(Sprite* spriteBatch, const Apoc3D::Math::Rectangle& dstRect);

			void UpdateEvents();
			void CopySkinGraphic(const StyleSkin* skin);
			
			void Initialize(const StyleSkin* skin);
			void Initialize(const ButtonVisualSettings& settings);

			void OnMouseHover();
			void OnMouseOut();
			void OnPress();
			void OnRelease();
			
			Point m_textPos;
			Point m_textSize;


			String m_text;

			//bool m_hasTextOffsetYOverride = false;
			//int32 m_textOffsetYOverride = 0;
		};

		class APAPI ButtonRow : public Control
		{
			RTTI_DERIVED(ButtonRow, Control);
		public:
			typedef EventDelegate<int> SelectedChangedHandler;
			typedef EventDelegate<ButtonRow*> ButtonEvent;

			ButtonRow(const StyleSkin* skin, const Point& position, int32 width, const List<String>& titles);
			ButtonRow(const StyleSkin* skin, const Point& position, int32 width, int32 colCount, const List<String>& titles);
			~ButtonRow();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			int getSelectedIndex() const { return m_selectedIndex; }

			TextRenderSettings TextSettings;
			UIGraphic DisabledGraphic;
			UIGraphic NormalGraphic;
			UIGraphic MouseHoverGraphic;
			UIGraphic MouseDownGraphic;
			ControlBounds ButtonMargin;

			ColorValue SeparationLineColor = CV_Black;

			SelectedChangedHandler eventSelectedChanging;
			
			ButtonEvent eventPress;
			ButtonEvent eventRelease;

			static ButtonEvent eventAnyPress;
			static ButtonEvent eventAnyRelease;

		private:
			enum VerticalBorderStyle
			{
				VBS_Both,
				VBS_Top,
				VBS_Bottom,
				VBS_None
			};
			enum Region9Flags
			{
				R9_None = 0,
				R9_TopLeft = 1 << 0,
				R9_TopCenter = 1 << 1,
				R9_TopRight = 1 << 2,
				R9_MiddleLeft = 1 << 3,
				R9_MiddleCenter = 1 << 4,
				R9_MiddleRight = 1 << 5,
				R9_BottomLeft = 1 << 6,
				R9_BottomCenter = 1 << 7,
				R9_BottomRight = 1 << 8,

				R9Mix_TopBar = R9_TopLeft | R9_TopCenter | R9_TopRight,
				R9Mix_MiddleBar = R9_MiddleLeft | R9_MiddleCenter | R9_MiddleRight,
				R9Mix_BottomBar = R9_BottomLeft | R9_BottomCenter | R9_BottomRight,
				R9Mix_All = R9Mix_TopBar | R9Mix_MiddleBar | R9Mix_BottomBar
			};
			void Setup(const StyleSkin* skin);
			void Init(int32 width, const List<String>& titles);

			void UpdatePositions();

			void DrawButton(Sprite* sprite, Texture* tex, int32 idx, int32 colType, VerticalBorderStyle rowType, const Apoc3D::Math::Rectangle* srcRect);

			void DrawRegion9Subbox(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect, ColorValue cv, Texture* texture, const Apoc3D::Math::Rectangle* srcRects, uint32 subRegionFlags);

			void OnPress();
			void OnRelease();


			int32 m_selectedIndex = 0;
			int32 m_hoverIndex = -1;

			bool m_mouseDown = false;

			int32 m_countPerRow;
			int32 m_numRows;
			int32 m_count;
			int32 m_rowHeight;
			String* m_titles;
			Apoc3D::Math::Rectangle* m_buttonDstRect;
		};

		template <int32 N>
		struct ButtonGroupTexturedVisualSettings
		{
			Texture* Graphic;

			Apoc3D::Math::Rectangle NormalRegion;
			Apoc3D::Math::Rectangle HoverRegions[N];
			Apoc3D::Math::Rectangle DownRegions[N];
			ControlBounds Paddings[N];
		};

		class APAPI ButtonGroupTextured
		{
		public:
			typedef EventDelegate<ButtonGroupTextured*, int32> ButtonEvent;

			template <int32 N>
			ButtonGroupTextured(const Point& position, const ButtonGroupTexturedVisualSettings<N>& settings)
				: Position(position), m_graphic(settings.Graphic), m_buttonCount(N)
			{
				m_graphicsSrcRect = settings.NormalRegion;

				m_isMouseHover = new bool[N];
				m_isMouseDown = new bool[N];
				memset(m_isMouseHover, 0, sizeof(bool) * N);
				memset(m_isMouseDown, 0, sizeof(bool) * N);

				m_graphicsSrcRectHover = new Apoc3D::Math::Rectangle[N];
				m_graphicsSrcRectDown = new Apoc3D::Math::Rectangle[N];
				m_hotAreaPaddings = new ControlBounds[N];
				for (int32 i = 0; i < N; i++)
				{
					m_hotAreaPaddings[i] = settings.Paddings[i];
					m_graphicsSrcRectDown[i] = settings.DownRegions[i];
					m_graphicsSrcRectHover[i] = settings.HoverRegions[i];
				}
			}

			ButtonGroupTextured(const Point& position, Texture* tex, const Apoc3D::Math::Rectangle& normalRegion,
				const List<Apoc3D::Math::Rectangle>& hoverRegs, const List<Apoc3D::Math::Rectangle>& downRegs, const List<ControlBounds>& paddings);

			~ButtonGroupTextured();

			void Draw(Sprite* sprite);
			void Update(const GameTime* time);

			bool isMouseHover() const { return m_hasMouseHoverArea; }
			const Apoc3D::Math::Rectangle& getMouseHoverArea() const { assert(m_hasMouseHoverArea); return m_mouseHoverArea; }

			int32 ForceMouseDownLookIndex = -1;

			bool Enabled = true;
			bool Visible = true;

			bool IsInteractive = true;

			Point Position;

			ButtonEvent eventPress;
			ButtonEvent eventRelease;
			
			static ButtonEvent eventAnyPress;
			static ButtonEvent eventAnyRelease;

		private:
			Texture* m_graphic;

			int32 m_buttonCount;

			Apoc3D::Math::Rectangle m_graphicsSrcRect;

			Apoc3D::Math::Rectangle* m_graphicsSrcRectHover;
			Apoc3D::Math::Rectangle* m_graphicsSrcRectDown;

			ControlBounds* m_hotAreaPaddings;

			bool* m_isMouseHover;
			bool* m_isMouseDown;

			bool m_hasMouseHoverArea = false;
			Apoc3D::Math::Rectangle m_mouseHoverArea;
		};

		class APAPI ButtonGroup : public Control
		{
			RTTI_DERIVED(ButtonGroup, Control);
		public:
			ButtonGroup(const StyleSkin* skin, const List<Button*>& buttons);
			ButtonGroup(const StyleSkin* skin, const List<Button*>& buttons, int selected);
			~ButtonGroup();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			int getSelectedIndex() const { return m_selectedIndex; }

			Button* getSelectedButton() const { return m_button[m_selectedIndex]; }

			const String& getSelectedText() const { return m_button[m_selectedIndex]->getText(); }

			UIEventHandler eventSelectionChanged;

		private:
			void Initialize();
			void Button_OnRelease(Button* button);

			List<Button*> m_button;
			int m_selectedIndex;
			
		};

		class APAPI RadioButton : public Control
		{
			RTTI_DERIVED(RadioButton, Control);
		public:
			RadioButton(const StyleSkin* skin, const Point& position, const String& text, bool checked);
			~RadioButton() { }

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			void Toggle();

			bool isChecked() const { return m_checked; }
			bool canUncheck() const { return m_canUncheck; }

			String Text;
		private:
			Point m_textOffset;

			bool m_mouseDown = false;
			bool m_mouseOver = false;
			bool m_checked = false;
			bool m_canUncheck = true;

			void UpdateEvents();
		};

		//class APAPI RadioButtonGroup : public Control
		//{

		//};
	}
}

#endif