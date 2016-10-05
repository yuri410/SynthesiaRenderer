#pragma once
#ifndef APOC3D_UICOMMON_H
#define APOC3D_UICOMMON_H

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
#include "apoc3d/Math/Rectangle.h"
#include "apoc3d/Math/ColorValue.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Meta/EventDelegate.h"

using namespace Apoc3D::Math;
using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace UI
	{
		typedef EventDelegate<Control*> UIEventHandler;
		typedef EventDelegate<MenuItem*> MenuItemEventHandler;

		enum struct TextHAlign
		{
			Left,
			Center,
			Right
		};
		enum struct TextVAlign
		{
			Top,
			Middle,
			Bottom
		};

		struct APAPI UIGraphic
		{
			Texture* Graphic = nullptr;
			ColorValue ModColor = CV_White;
			Collections::FixedList<Apoc3D::Math::Rectangle, 9> SourceRects;

			UIGraphic() { }

			UIGraphic(Texture* tex);
			UIGraphic(Texture* tex, ColorValue color);

			template <typename = void>
			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle& srcRect) : UIGraphic(tex, srcRect, CV_White) { }
			template <typename = void>
			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle& srcRect, ColorValue color)
				: Graphic(tex), ModColor(color)
			{
				SourceRects.Add(srcRect);
			}

			template <typename = void>
			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle(&srcRect)[3]) : UIGraphic(tex, srcRect, CV_White) { }
			template <typename = void>
			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle(&srcRect)[3], ColorValue color) 
				: Graphic(tex), ModColor(color)
			{
				SourceRects.AddArray(srcRect);
			}

			template <typename = void>
			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle(&srcRect)[9]) : UIGraphic(tex, srcRect, CV_White) { }
			template <typename = void>
			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle(&srcRect)[9], ColorValue color)
				: Graphic(tex), ModColor(color)
			{
				SourceRects.AddArray(srcRect);
			}

			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle& srcRect, int32 left, int32 right, int32 top, int32 bottom);
			UIGraphic(Texture* tex, const Apoc3D::Math::Rectangle& srcRect, int32 left, int32 right, int32 top, int32 bottom, ColorValue color);

			UIGraphic(Texture* tex, std::initializer_list<Apoc3D::Math::Rectangle> srcRect);
			UIGraphic(Texture* tex, std::initializer_list<Apoc3D::Math::Rectangle> srcRect, ColorValue modColor);

			void Draw(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect, bool vertical) const;
			void Draw(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect) const;
			void DrawVert(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect) const;

			bool isSet() const { return Graphic != nullptr; }

			bool isSimple() const { return SourceRects.getCount() == 1; }
			bool is3patch() const { return SourceRects.getCount() == 3; }
			bool is9patch() const { return SourceRects.getCount() == 9; }

			int32 getWidth() const;
			int32 getHeight() const;
			Point getSize() const;
		};

		struct APAPI UIGraphicSimple 
		{
			Texture* Graphic = nullptr;
			ColorValue ModColor = CV_White;
			Collections::FixedList<Apoc3D::Math::Rectangle, 1> SourceRects;

			UIGraphicSimple() { }

			UIGraphicSimple(Texture* tex);
			UIGraphicSimple(Texture* tex, ColorValue modColor);

			UIGraphicSimple(Texture* tex, const Apoc3D::Math::Rectangle& srcRect);
			UIGraphicSimple(Texture* tex, const Apoc3D::Math::Rectangle& srcRect, ColorValue modColor);

			void Draw(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect) const;
			void DrawCentered(Sprite* sprite, const Point& pos, const Point& parentSize) const;
			void DrawCentered(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect) const;

			bool isSet() const { return Graphic != nullptr; }

			int32 getWidth() const;
			int32 getHeight() const;
			Point getSize() const;
		};

		struct APAPI ControlBounds
		{
			enum SideIndex
			{
				SI_Left,
				SI_Top,
				SI_Right,
				SI_Bottom
			};

			int32 Left = 0;
			int32 Right = 0;
			int32 Top = 0;
			int32 Bottom = 0;

			ControlBounds() { }
			ControlBounds(int32 left, int32 right, int32 top, int32 bottom) : Left(left), Right(right), Top(top), Bottom(bottom) { }
			ControlBounds(const Apoc3D::Math::Rectangle& graphicalArea, const Apoc3D::Math::Rectangle& hotArea);

			int32 getHorizontalSum() const { return Left + Right; }
			int32 getVerticalSum() const { return Top + Bottom; }

			Apoc3D::Math::Rectangle InflateRect(const Apoc3D::Math::Rectangle& rect) const;
			Apoc3D::Math::Rectangle ShrinkRect(const Apoc3D::Math::Rectangle& rect) const;

			int32 operator[](SideIndex idx) const;

			void SetFromLeftTopRightBottom(int32 padding[4]);
			void SetLeftRight(int32 l, int32 r);
			void SetTopBottom(int32 t, int32 b);
			void SetZero();
		};

		struct APAPI TextRenderSettings
		{
			bool HasTextShadow = false;

			Point TextShadowOffset = Point(2, 1);
			ColorValue TextShadowColor = CV_White;
			ColorValue TextColor = CV_Black;
			ColorValue TextSelectionColor = CV_LightBlue;

			ColorValue TextShadowColorDisabled = CV_RepackAlpha(CV_White, 0x7f);
			ColorValue TextColorDisabled = CV_RepackAlpha(CV_Black, 0x7f);
			ColorValue TextSelectionColorDisabled = CV_RepackAlpha(CV_LightBlue, 0x7f);


			TextHAlign HorizontalAlignment = TextHAlign::Center;
			TextVAlign VerticalAlignment = TextVAlign::Middle;

			ControlBounds TextPadding;


			void Draw(Sprite* sprite, Font* font, const String& text, const Point& pos, const Point& size, bool enabled) const;
			void Draw(Sprite* sprite, Font* font, const String& text, const Apoc3D::Math::Rectangle& area, bool enabled) const;

			void DrawNoAlignment(Sprite* sprite, Font* font, const String& text, const Point& pos, bool enabled) const;
			void DrawHyperLinkText(Sprite* sprite, Font* font, const String& text, const Point& pos, bool enabled, bool selected, float underlineWidth) const;

			void DrawBG(Sprite* sprite, Font* font, const String& text, int32 selStart, int32 selEnd, const Point& pos, const Point& size, bool enabled) const;
			void DrawBG(Sprite* sprite, Font* font, const String& text, int32 selStart, int32 selEnd, const Apoc3D::Math::Rectangle& area, bool enabled) const;

			void DrawTextLine(Sprite* sprite, const Point& start, const Point& end, float width, bool enabled, bool selected) const;

			Point GetTextOffset(const Point& textSize, const Point& areaSize) const;
			Point GetTextOffset(Font* font, const String& text, const Point& size) const;
		};

		template <typename T>
		class OptionalSetting
		{
		public:
			OptionalSetting() : m_data() { }
			OptionalSetting(const T& data) : m_data(data), m_isSet(true) { }
			~OptionalSetting() { }

			OptionalSetting& operator=(const T& other)
			{
				m_data = other;
				m_isSet = true;
				return *this;
			}

			void Clear() { m_isSet = false; }

			bool isSet() const { return m_isSet; }
			operator T() { return m_data; }
			operator T() const { return m_data; }

			T& getContent() { return m_data; }
			const T& getContent() const { return m_data; }

			bool operator ==(const OptionalSetting& o) const { return m_isSet == o.m_isSet && m_data == o.m_data; }
			bool operator !=(const OptionalSetting& o) const { return !this->operator=(o); }
		private:
			T m_data;
			bool m_isSet = false;
		};

		struct ButtonVisualSettings
		{
			typedef Apoc3D::Math::Rectangle ARect;

			Font* FontRef = nullptr;

			OptionalSetting<ColorValue> TextColor;
			OptionalSetting<ColorValue> TextColorDisabled;

			UIGraphic NormalGraphic;

			OptionalSetting<UIGraphic> DisabledGraphic;
			OptionalSetting<UIGraphic> MouseHoverGraphic;
			OptionalSetting<UIGraphic> MouseDownGraphic;

			OptionalSetting<UIGraphicSimple> OverlayIcon;
			OptionalSetting<UIGraphicSimple> DisabledOverlayIcon;

			OptionalSetting<ControlBounds> ContentPadding;
			OptionalSetting<ControlBounds> Margin;

			ButtonVisualSettings() { }

			void Setup(Texture* tex, const ARect& normal, const ARect& hover, const ARect& down);
			void Setup(Texture* tex, const ARect& normal, const ARect& hover, const ARect& down, const ControlBounds& margin);
			void Setup(Texture* tex, const ARect& normal, const ARect& hover, const ARect& down, const ARect& disabled, const ControlBounds& margin);
		};

		struct ScrollBarVisualSettings
		{
			UIGraphic BackgroundGraphic;
			UIGraphic HandleGraphic;
			OptionalSetting<UIGraphic> DisabledBackgroundGraphic;
			
			ButtonVisualSettings DecrButton;
			ButtonVisualSettings IncrButton;

			OptionalSetting<ControlBounds> BorderPadding;
			OptionalSetting<ControlBounds> HandlePadding;
		};

		struct TextBoxVisualSettings
		{
			Font* FontRef = nullptr;

			ColorValue TextColor = CV_Black;
			OptionalSetting<ColorValue> TextColorDisabled;

			ColorValue TextSelectionColor = CV_LightBlue;
			OptionalSetting<ColorValue> TextSelectionColorDisabled;

			UIGraphic BackgroundGraphic;
			OptionalSetting<UIGraphic> DisabledGraphic;

			OptionalSetting<bool> AlwaysShowVS;
			OptionalSetting<bool> AlwaysShowHS;

			OptionalSetting<ControlBounds> Margin;
			OptionalSetting<ControlBounds> ContentPadding;

			OptionalSetting<ScrollBarVisualSettings> HScrollBar;
			OptionalSetting<ScrollBarVisualSettings> VScrollBar;
		};

		struct ListBoxVisualSettings
		{
			Font* FontRef = nullptr;

			ColorValue TextColor = CV_Black;
			OptionalSetting<ColorValue> TextColorDisabled;

			UIGraphic BackgroundGraphic;

			ScrollBarVisualSettings HScrollBar;
			ScrollBarVisualSettings VScrollBar;

			OptionalSetting<ControlBounds> Margin;
			OptionalSetting<ControlBounds> Padding;

			OptionalSetting<int32> ItemHeight;
		};

		struct ProgressBarVisualSettings
		{
			Font* FontRef = nullptr;

			Texture* Graphic = nullptr;
			Apoc3D::Math::Rectangle BackgroundRegions[3];
			Apoc3D::Math::Rectangle BarRegions[3];
			ControlBounds Margin;

			ColorValue BackgroundColor = CV_White;
			ColorValue BarColor = CV_White;

			bool HasTextShadow = false;

			ColorValue TextColor = CV_Black;
			ColorValue TextColorDisabled = CV_Black;
			ColorValue TextShadowColor = CV_Black;
			ColorValue TextShadowColorDisabled = CV_Black;

			Point TextShadowOffset;

			int32 BarStartPad = 0;
			int32 BarEndPad = 0;
		};

		struct SliderBarVisualSettings
		{
			Texture* Graphic = nullptr;
			Apoc3D::Math::Rectangle BackgroundRegions[3];
			Apoc3D::Math::Rectangle BarRegions[3];
			ControlBounds Margin;

			ColorValue BackgroundColor = CV_White;
			ColorValue BarColor = CV_White;

			int32 BarStartPad = 0;
			int32 BarEndPad = 0;

			Point HandleOffset;
			UIGraphicSimple HandleNormalGraphic;
			UIGraphicSimple HandleHoverGraphic;
			UIGraphicSimple HandleDownGraphic;
			UIGraphicSimple HandleDisabledGraphic;

			UIGraphicSimple LargeTickGraphic;

			ControlBounds HandleMargin;
		};

		struct ScissorTestScope
		{
		public:
			ScissorTestScope(const Apoc3D::Math::Rectangle& region, Sprite* sprite);
			~ScissorTestScope();

			bool isEmpty() const { return m_isEmpty; }
		private:
			Sprite* m_sprite;
			Apoc3D::Math::Rectangle m_oldScissorRect;
			bool m_oldScissorTest;
			bool m_isEmpty = false;
		};

		/**
		 *  The interfaces for System Forms and Panels. And it is also responsible
		 *  for drawing and updating them.
		 *  Any system forms should be added through this interface.
		 */
		namespace SystemUI
		{
			APAPI void Initialize(RenderDevice* device);
			APAPI void Finalize();

			APAPI void Add(ControlContainer* cc);
			APAPI void Remove(ControlContainer* cc);
			APAPI void RemoveForm(const String& name);
			APAPI void RemoveContainer(const String& name);
			APAPI void ScheduleBringFront(ControlContainer* cc);
				 
			APAPI void Draw();
			APAPI void Update(const Core::GameTime* time);

			APAPI Point ClampFormMovementOffset(Form* frm, const Point& vec);
			APAPI Texture* GetWhitePixel();

			/** Return the area to display UI in the viewport. In screen coordinates. */
			APAPI Apoc3D::Math::Rectangle GetUIArea(RenderDevice* device);

			APAPI bool GetMinimizedPosition(RenderDevice* dev, Form* form, Point& pos);
			APAPI Point GetMaximizedSize(RenderDevice* dev, Form* form);
			APAPI Apoc3D::Math::Rectangle GetMaximizedRect(RenderDevice* dev, Form* form);
			
			APAPI const List<Form*>& getForms();

			/** Specifies the area to display UI in the viewport. In unified coordinates. */
			APAPI extern RectangleF& UIArea;
			APAPI extern RectangleF& MaximizedArea;
			
			/**
			 *  InteractingForm is the one that is currently being mouse hovered.
			 *  Used to mark the first form being touched.
			 */
			APAPI extern Form*& InteractingForm;
			APAPI extern Form*& ModalForm;
			APAPI extern Form*& TopMostForm;

			APAPI extern MenuBar*& MainMenu;

			APAPI extern float DoubleClickInterval;
		}

		namespace GUIUtils
		{
			struct ScrollBarPositioning
			{
				Point Position;
				int32 Width = 0;

				int32 Length = 0;

				ScrollBarPositioning(int32 width) : Width(width) { }
			};

			APAPI void CalculateScrollBarPositions(const Apoc3D::Math::Rectangle& area, ScrollBarPositioning* vs, ScrollBarPositioning* hs);
		}

		APAPI void guiGenerateRegion9Rects(const Apoc3D::Math::Rectangle& dstRect, Apoc3D::Math::Rectangle (&srcRects)[9], Apoc3D::Math::Rectangle (&destRects)[9]);
		APAPI void guiGenerateRegion3Rects(const Point& pos, int32 width, Apoc3D::Math::Rectangle (&srcRects)[3], Apoc3D::Math::Rectangle (&destRects)[3]);
		APAPI void guiGenerateRegion3VertRects(const Point& pos, int32 width, Apoc3D::Math::Rectangle (&srcRects)[3], Apoc3D::Math::Rectangle (&destRects)[3]);

		
		APAPI void guiDrawRegion9(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect, ColorValue cv,
			Texture* texture, const Apoc3D::Math::Rectangle (&srcRects)[9], Apoc3D::Math::Rectangle* centerDstRegion = nullptr);

		APAPI void guiDrawRegion3(Sprite* sprite, const Point& pt, int width, ColorValue cv,
			Texture* texture, const Apoc3D::Math::Rectangle (&srcRects)[3], Apoc3D::Math::Rectangle* centerDstRegion = nullptr);

		APAPI void guiDrawRegion3Vert(Sprite* sprite, const Point& pos, int height, ColorValue cv,
			Texture* texture, const Apoc3D::Math::Rectangle (&srcRects)[3], Apoc3D::Math::Rectangle* centerDstRegion = nullptr);

		// Draw horizontal region 3 with a clipping width
		APAPI void guiDrawRegion3Clipped(Sprite* sprite, const Point& pos, int32 maxWidth, int32 clipWidth,
			ColorValue cv, Texture* texture, const Apoc3D::Math::Rectangle (&srcRects)[3]);

		// Assumed to include the border regions
		// No clips do fewer calculations, but can not draw stuff that are smaller than the start/end regions
		APAPI bool guiDrawRegion9Noclip(Sprite* sprite, const Apoc3D::Math::Rectangle& dstRect, ColorValue cv,
			Texture* texture, const Apoc3D::Math::Rectangle (&srcRects)[9], Apoc3D::Math::Rectangle* centerDstRegion = nullptr);

		APAPI bool guiDrawRegion3Noclip(Sprite* sprite, const Point& pt, int width, ColorValue cv,
			Texture* texture, const Apoc3D::Math::Rectangle (&srcRects)[3], Apoc3D::Math::Rectangle* centerDstRegion = nullptr);

		APAPI bool guiDrawRegion3VertNoclip(Sprite* sprite, const Point& pos, int height, ColorValue cv,
			Texture* texture, const Apoc3D::Math::Rectangle (&srcRects)[3], Apoc3D::Math::Rectangle* centerDstRegion = nullptr);

		APAPI void guiOmitLineText(Font* fnt, int32 widthlimit, String& line);

		APAPI void guiDrawProgressBar(Sprite* sprite, const Point& position, int32 width, float value, 
			Texture* texture, const Apoc3D::Math::Rectangle (&bgSrcRects)[3], const Apoc3D::Math::Rectangle (&barSrcRects)[3],
			const ControlBounds& margin, int32 barStartPad, int32 barEndPad);


		namespace ControlCodes
		{
			// UTF-16 BMP(0) private range: U+E000..U+F8FF
			// U+F8C0..U+F8CF for UI text control codes
			// U+F8E0..U+F8EF for font system control codes
			const uint16 Font_Color = 0xF8E0;
			const uint16 Font_Font = 0xF8E1;
			const uint16 Font_Move = 0xF8E2;		// move drawing position

			const uint16 Label_HyperLinkStart = 0xF8C0;
			const uint16 Label_HyperLinkEnd = 0xF8C1;

			inline bool isControlCode(int32 code) { return code >= 0xF8C0 && code < 0xF8F0; }

			String MakeColorControl(uint32 argb);
			String MakeMoveControl(const Point& position, bool passConditionCheck = false, bool relative = false);

			String MakeHyperLink(const String& linkText, uint16 linkID);
		}
	}
}

#endif