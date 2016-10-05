#pragma once
#ifndef APOC3D_CLASSICSTYLESKIN_H
#define APOC3D_CLASSICSTYLESKIN_H

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

#include "UICommon.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Math/Rectangle.h"
#include "apoc3d/Math/ColorValue.h"

using namespace Apoc3D::Math;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::VFS;

namespace Apoc3D
{
	namespace UI
	{
		class APAPI StyleSkin
		{
		public:
			StyleSkin(RenderDevice* device, const FileLocateRule& rule);
			~StyleSkin();

			Font* ContentTextFont = nullptr;
			Font* TitleTextFont = nullptr;
			ColorValue TextColor;
			ColorValue TextColorDisabled;
			ColorValue TextSelectionColor;
			ColorValue TextSelectionColorDisabled;

			ColorValue ControlFaceColor;
			ColorValue ControlDarkShadeColor;
			ColorValue ControlLightShadeColor;

			ColorValue MIDBackgroundColor;
			ColorValue BorderColor;


			Font* ButtonFont = nullptr;
			ControlBounds ButtonPadding;
			ControlBounds ButtonMargin;
			Apoc3D::Math::Rectangle ButtonNormalRegions[9];
			Apoc3D::Math::Rectangle ButtonHoverRegions[9];
			Apoc3D::Math::Rectangle ButtonDownRegions[9];
			Apoc3D::Math::Rectangle ButtonDisabledRegions[9];
			ColorValue ButtonNormalColor = CV_White;
			ColorValue ButtonHoverColor = CV_White;
			ColorValue ButtonDownColor = CV_White;
			ColorValue ButtonDisabledColor = CV_White;

			Font* TextBoxFont = nullptr;
			ControlBounds TextBoxPadding;
			ControlBounds TextBoxMargin;
			Apoc3D::Math::Rectangle TextBoxRegions[3];
			ColorValue TextBoxColor = CV_White;
			

			ControlBounds TextBoxExMargin;
			Apoc3D::Math::Rectangle TextBoxExRegions[9];
			ColorValue TextBoxExColor = CV_White;

			Font* CheckBoxFont = nullptr;
			ControlBounds CheckBoxMargin;
			Apoc3D::Math::Rectangle CheckBoxDisabledRegion;
			Apoc3D::Math::Rectangle CheckBoxNormalRegion;
			Apoc3D::Math::Rectangle CheckBoxHoverRegion;
			Apoc3D::Math::Rectangle CheckBoxDownRegion;
			Apoc3D::Math::Rectangle CheckBoxCheckedRegion;
			Apoc3D::Math::Rectangle CheckBoxDisabledCheckedRegion;
			int32 CheckBoxTextSpacing;
			ColorValue CheckBoxDisabledColor = CV_White;
			ColorValue CheckBoxNormalColor = CV_White;
			ColorValue CheckBoxHoverColor = CV_White;
			ColorValue CheckBoxDownColor = CV_White;
			ColorValue CheckBoxCheckedColor = CV_White;
			ColorValue CheckBoxDisableCheckedColor = CV_White;
			

			Font* RadioButtonFont = nullptr;
			ControlBounds RadioButtonMargin;
			Apoc3D::Math::Rectangle RadioButtonDisabledRegion;
			Apoc3D::Math::Rectangle RadioButtonNormalRegion;
			Apoc3D::Math::Rectangle RadioButtonHoverRegion;
			Apoc3D::Math::Rectangle RadioButtonDownRegion;
			Apoc3D::Math::Rectangle RadioButtonCheckedRegion;
			Apoc3D::Math::Rectangle RadioButtonDisabledCheckedRegion;
			int32 RadioButtonTextSpacing;
			ColorValue RadioButtonDisabledColor = CV_White;
			ColorValue RadioButtonNormalColor = CV_White;
			ColorValue RadioButtonHoverColor = CV_White;
			ColorValue RadioButtonDownColor = CV_White;
			ColorValue RadioButtonCheckedColor = CV_White;
			ColorValue RadioButtonDisabledCheckedColor = CV_White;


			ControlBounds DropdownButtonMargin;
			Point DropdownButtonOffset;
			Apoc3D::Math::Rectangle DropdownButtonNormalRegion;
			Apoc3D::Math::Rectangle DropdownButtonHoverRegion;
			Apoc3D::Math::Rectangle DropdownButtonDownRegion;
			ColorValue DropdownButtonNormalColor = CV_White;
			ColorValue DropdownButtonHoverColor = CV_White;
			ColorValue DropdownButtonDownColor = CV_White;

		
			Apoc3D::Math::Rectangle VScrollBarCursor[3];
			Apoc3D::Math::Rectangle VScrollBarBG;
			Apoc3D::Math::Rectangle VScrollBarUp;
			Apoc3D::Math::Rectangle VScrollBarDown;

			Apoc3D::Math::Rectangle HScrollBarCursor[3];
			Apoc3D::Math::Rectangle HScrollBarBG;
			Apoc3D::Math::Rectangle HScrollBarLeft;
			Apoc3D::Math::Rectangle HScrollBarRight;

			Font* ListBoxFont = nullptr;
			ControlBounds ListBoxMargin;
			ControlBounds ListBoxPadding;
			Apoc3D::Math::Rectangle ListBoxBackground[9];

			ProgressBarVisualSettings ProgressBarVS;
			SliderBarVisualSettings HSliderBar;
			SliderBarVisualSettings VSliderBar;

			Font* FormFont = nullptr;
			ControlBounds FormTitlePadding;
			Apoc3D::Math::Rectangle FormTitle[3];
			Apoc3D::Math::Rectangle FormBody[9];
			Apoc3D::Math::Rectangle FormResizer;

			Apoc3D::Math::Rectangle FormCBIconMax;
			Apoc3D::Math::Rectangle FormCBIconMin;
			Apoc3D::Math::Rectangle FormCBIconRestore;
			Apoc3D::Math::Rectangle FormCBIconClose;

			Apoc3D::Math::Rectangle FormCBMaxDisabled;
			Apoc3D::Math::Rectangle FormCBMaxNormal;
			Apoc3D::Math::Rectangle FormCBMaxHover;
			Apoc3D::Math::Rectangle FormCBMaxDown;

			Apoc3D::Math::Rectangle FormCBMinDisabled;
			Apoc3D::Math::Rectangle FormCBMinNormal;
			Apoc3D::Math::Rectangle FormCBMinHover;
			Apoc3D::Math::Rectangle FormCBMinDown;

			Apoc3D::Math::Rectangle FormCBCloseDisabled;
			Apoc3D::Math::Rectangle FormCBCloseNormal;
			Apoc3D::Math::Rectangle FormCBCloseHover;
			Apoc3D::Math::Rectangle FormCBCloseDown;

			Apoc3D::Math::Rectangle FormCBRestoreDisabled;
			Apoc3D::Math::Rectangle FormCBRestoreNormal;
			Apoc3D::Math::Rectangle FormCBRestoreHover;
			Apoc3D::Math::Rectangle FormCBRestoreDown;

			
			

			Apoc3D::Math::Rectangle SubMenuArrow;

			Apoc3D::Math::Rectangle HShade;

			Texture* SkinTexture = nullptr;

		private:
			Font* GetFontName(const String& alias);

			void ParseMargin(Apoc3D::Config::ConfigurationSection* sect, ControlBounds& result);
			void ParsePadding(Apoc3D::Config::ConfigurationSection* sect, ControlBounds& result);
			void Parse9Region(Apoc3D::Config::ConfigurationSection* sect, Apoc3D::Math::Rectangle (&srcRects)[9], Apoc3D::Collections::HashMap<String, const Apoc3D::Math::Rectangle*>& cachedRegions);
			void Parse3Region(Apoc3D::Config::ConfigurationSection* sect, Apoc3D::Math::Rectangle (&srcRects)[3], Apoc3D::Collections::HashMap<String, const Apoc3D::Math::Rectangle*>& cachedRegions);
			void ParseRegion(Apoc3D::Config::ConfigurationSection* sect, Apoc3D::Math::Rectangle& srcRect, Apoc3D::Collections::HashMap<String, const Apoc3D::Math::Rectangle*>& cachedRegions);

			ColorValue ParseColorValue(const String& txt);
			void ParseColorValue(Apoc3D::Config::ConfigurationSection* sect, ColorValue& result);
			void ParseColorValue(Apoc3D::Config::ConfigurationSection* sect, const String& attName, ColorValue& result);
			void ParseOffset(Apoc3D::Config::ConfigurationSection* sect, Point& result);
			void ParsePoint(Apoc3D::Config::ConfigurationSection* sect, const String& attName, Point& result);
			

			void Offset9Region(Apoc3D::Config::ConfigurationSection* sect, Apoc3D::Math::Rectangle (&srcRects)[9]);
			void Offset3Region(Apoc3D::Config::ConfigurationSection* sect, Apoc3D::Math::Rectangle (&srcRects)[3]);
			void OffsetRegion(Apoc3D::Config::ConfigurationSection* sect, Apoc3D::Math::Rectangle& srcRect);
		};
	}
}

#endif