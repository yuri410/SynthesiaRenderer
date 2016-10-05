#pragma once
#ifndef APOC3D_UI_TEXT_H
#define APOC3D_UI_TEXT_H

#include "Control.h"
#include "KeyboardHelper.h"

namespace Apoc3D
{
	namespace UI
	{
		class APAPI Label : public Control
		{
			RTTI_DERIVED(Label, Control);
		public:
			typedef EventDelegate<Label*> LabelEvent;
			typedef EventDelegate<Label*, uint16> LabelHyperLinkEvent;

			Label(Font* font, const Point& position, const String& text);
			Label(Font* font, const Point& position, const String& text, int width, TextHAlign alignment = TextHAlign::Left);
			Label(Font* font, const Point& position, const String& text, int width, int height, TextHAlign alignment = TextHAlign::Left);
			Label(Font* font, const Point& position, const String& text, const Point& size, TextHAlign alignment = TextHAlign::Left);

			Label(const StyleSkin* skin, const Point& position, const String& text);
			Label(const StyleSkin* skin, const Point& position, const String& text, int width, TextHAlign alignment = TextHAlign::Left);
			Label(const StyleSkin* skin, const Point& position, const String& text, int width, int height, TextHAlign alignment = TextHAlign::Left);
			Label(const StyleSkin* skin, const Point& position, const String& text, const Point& size, TextHAlign alignment = TextHAlign::Left);

			virtual ~Label();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			const String& getText() const { return m_text; }
			void SetText(const String& txt);

			void setSize(const Point& v);
			void setHeight(int v) { m_size.Y = v; }
			void setWidth(int v);

			bool isMouseHover() const { return m_mouseHover; }
			bool isMouseHoverLinks() const { return m_mouseHoverLinks; }

			const Apoc3D::Math::Rectangle& getMouseHoverLinksArea() const { return m_mouseHoverLinkArea; }

			TextRenderSettings TextSettings;

			bool AutosizeX = false;
			bool AutosizeY = false;

			int32 AdditionalLineSpacing = 0;

			LabelEvent eventMouseHover;
			LabelEvent eventMouseOut;
			LabelEvent eventPress;
			LabelEvent eventRelease;

			LabelHyperLinkEvent eventHyperlinkMouseHover;
			LabelHyperLinkEvent eventHyperlinkMouseOut;
			LabelHyperLinkEvent eventHyperlinkPress;
			LabelHyperLinkEvent eventHyperlinkRelease;

		private:
			struct HyperLinkRange
			{
				int32 StartLine = 0;
				int32 StartIndex = 0;
				int32 EndLine = 0;
				int32 EndIndex = 0;

				uint16 ID = 0;

				bool MouseHover = false;
				bool MouseDown = false;

				List<String> LinkTextLines;

				void GetLineMetrics(int32 idx, const String& line, Font* fnt, int32 lineHeight, TextRenderSettings& ts, Point& size, Point& off) const;
			};

			void Initialize(const StyleSkin* skin);
			void Initialize(Font* font);

			void UpdateText();

			bool GotoNextCharacter(int32& lineIdx, int32& chIdx);
			void GotoPreviousCharacter(int32& lineIdx, int32& chIdx);

			void OnMouseHover();
			void OnMouseOut();
			void OnPress();
			void OnRelease();

			int32 getLineHeight();

			String m_text;
			List<String> m_lines;
			List<HyperLinkRange> m_hyperLinks;

			bool m_mouseHover = false;
			bool m_mouseDown = false;
			bool m_mouseHoverLinks = false;

			Apoc3D::Math::Rectangle m_mouseHoverLinkArea;
		};

		class APAPI TextBox : public ScrollableControl
		{
			RTTI_DERIVED(TextBox, ScrollableControl);
		public:
			typedef EventDelegate<TextBox*> TextBoxEvent;

			TextBox(const TextBoxVisualSettings& settings, const Point& position, int width);
			TextBox(const TextBoxVisualSettings& settings, const Point& position, int width, const String& text);
			TextBox(const TextBoxVisualSettings& settings, const Point& position, const Point& size, const String& text);
			TextBox(const TextBoxVisualSettings& settings, const Point& position, int width, int height, const String& text);

			TextBox(const StyleSkin* skin, const Point& position, int width);
			TextBox(const StyleSkin* skin, const Point& position, int width, const String& text);
			TextBox(const StyleSkin* skin, const Point& position, const Point& size, const String& text);
			TextBox(const StyleSkin* skin, const Point& position, int width, int height, const String& text);
			virtual ~TextBox();

			virtual void Update(const GameTime* time) override;
			virtual void Draw(Sprite* sprite) override;

			Apoc3D::Math::Rectangle GetTextArea() const;

			const String& getText() const { return m_textEdit.getText(); }
			void SetText(const String& text);

			void setWidth(int w) { m_size.X = w; }
			void setHeight(int h) { m_size.Y = h; }

			bool isMouseHover() const { return m_mouseHover; }

			bool ReadOnly = false;
			bool HasFocus = false;

			TextRenderSettings TextSettings;
			TextRenderSettings BackgroundTextSettings;

			String BackgroundText;

			UIGraphic NormalGraphic;
			UIGraphic DisabledGraphic;

			ControlBounds Margin;
			ControlBounds ContentPadding;

			TextBoxEvent eventEnterPressed;
			TextBoxEvent eventContentChanged;
			TextBoxEvent eventUpPressedSingleline;
			TextBoxEvent eventDownPressedSingleline;

		private:
			void Initialize(const StyleSkin* skin);
			void Initialize(const TextBoxVisualSettings& settings);
			void PostInitialize();
			
			void InitText(const String& text);

			void UpdateScrollbars(const GameTime* time);

			void CheckFocus();

			void CursorScrolling();
			void _DrawText(Sprite* sprite);
			void DrawCursor(Sprite* sprite);

			void TextEditState_EnterPressed();
			void TextEditState_ContentChanged();
			void TextEditState_UpPressedSingleline();
			void TextEditState_DownPressedSingleline();

			void Keyboard_OnPress(KeyboardKeyCode code, KeyboardEventsArgs e);
			void Keyboard_OnPaste(String value);
			void vScrollbar_OnChangeValue(ScrollBar* ctrl);
			void hScrollbar_OnChangeValue(ScrollBar* ctrl);

			TextEditState m_textEdit;

			Point m_cursorOffset;
			Point m_scrollOffset;

			int m_visibleLines = 0;
			bool m_multiline = false;

			bool m_mouseHover = false;
			bool m_cursorVisible = false;
			bool m_timerStarted = false;
			float m_timer = 0.5f;

			MultiClickChecker m_clickChecker;
		};
	}
}

#endif