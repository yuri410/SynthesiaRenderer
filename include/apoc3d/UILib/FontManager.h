#pragma once
#ifndef APOC3D_FONTMANAGER_H
#define APOC3D_FONTMANAGER_H

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

#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Math/Rectangle.h"
#include "apoc3d/Math/RandomUtils.h"

namespace Apoc3D
{
	namespace UI
	{
		using namespace Apoc3D::Collections;
		using namespace Apoc3D::Core;
		using namespace Apoc3D::Graphics;
		using namespace Apoc3D::Graphics::RenderSystem;
		using namespace Apoc3D::VFS;
		using namespace Apoc3D::Math;
		using namespace Apoc3D::IO;

		/**
		 *  This class contains and manages the glyphs of a font and renders texts in realtime.
		 *  
		 * remarks:
		 *  The font data is built by the pipeline and can be loaded by this class.
		 *  When rendering the font, it pack
		 *  required glyph bitmaps into a font texture, and calculate the texture coordinates
		 *  used for locating them.
		 *
		 *  When the Font class can not hold the number of glyph bitmaps at once, caching is used.
		 *  Initially the first glyphs in the font will be loaded and packed into texture. 
		 *  When drawing a string, if any required glyph is not yet loaded, the ones seldom used 
		 *  will be unloaded to make room for the requested one.
		 *  This circumstances happens for big fonts or languages like Chinese, Korean, which has
		 *  a large number of characters.
		 */
		class APAPI Font
		{
			friend class FontManager;
		public:
			enum CustomGlyphAlignment
			{
				CGA_Center,
				CGA_Top,
				CGA_Bottom
			};

			Font(RenderDevice* device, const ResourceLocation& fl);
			~Font();

			bool ChangeCharacterSetting(int32 ch, short left, short top, float adcanceX);
			bool LookupCharacterSetting(int32 ch, short& left, short& top, float& adcanceX);

			void RegisterCustomGlyph(int32 charCode, Texture* graphic, const Apoc3D::Math::Rectangle& srcRect, short left, short top, float advanceX);
			void RegisterCustomGlyph(int32 charCode, Texture* graphic, const Apoc3D::Math::Rectangle& srcRect);

			void RegisterCustomGlyphAligned(int32 charCode, Texture* graphic, const Apoc3D::Math::Rectangle& srcRect, int32 padLeft, int32 padRight, CustomGlyphAlignment vertAlignment, int32 vaValue);
			void UnregisterCustomGlyph(int32 utf16code);
			void ClearCustomGlyph();

			void DrawStringDissolving(Sprite* sprite, const String& text, float x, float y, uint color, float length, int dissolvingCount, const Point& dissolvePatchSize, float maxDissolvingScale);
			void DrawStringDissolving(Sprite* sprite, const String& text, const Point& pos, uint color, float length, int dissolvingCount, const Point& dissolvePatchSize, float maxDissolvingScale);

			void DrawString(Sprite* sprite, const String& text, float x, float y, int width, uint color);
			void DrawString(Sprite* sprite, const String& text, int x, int y, int width, uint color);

			void DrawStringEx(Sprite* sprite, const String& text, float x, float y, uint color, int length = -1, float lineSpace = 0, char16_t suffix = 0, float hozShrink = 0);
			void DrawStringEx(Sprite* sprite, const String& text, int x, int y, uint color, int length = -1, int lineSpace = 0, char16_t suffix = 0, float hozShrink = 0);

			void DrawString(Sprite* sprite, const String& text, const Point& pt, uint color, float hozShrink = 0);
			void DrawString(Sprite* sprite, const String& text, const PointF& pt, uint color, float hozShrink = 0);

			void DrawStringGradient(Sprite* sprite, const String& text, int32 x, int32 y, uint startColor, uint endColor);



			String LineBreakString(const String& text, int32 width, bool byWord, int32& lineCount);
			Point MeasureString(const String& text);
			Point MeasureString(const String& text, int32 start, int32 end);

			int32 FitSingleLineString(const String& str, int32 width);

			int32 CalculateLineCount(const String& text, int32 width);

			float getLineHeight() const { return m_glyphHeight; }
			int32 getLineHeightInt() const { return m_glyphHeightInt; }

			float getLineGap() const { return m_lineGap; }
			
			float getLineBackgroundHeight() const { return m_glyphHeight + m_lineGap + m_descender; }
			float getTextBackgroundHeight(int32 lineCount) const { return m_descender + (m_glyphHeight + m_lineGap) * lineCount; }

			void getStandardMetrics(float& ascender, float& descender) const { ascender = m_ascender; descender = m_descender; }

			Texture* getInternalTexture() const { return m_fontPack; }

		private:
			static const int32 MaxFreq = 10;

			/**
			 *  Represents a character supported by the font.
			 *  A supported character is basically the glyph bitmap with additional metrics info.
			 *
			 *  The character and glyphs are not 1 to 1 mapped because a glyph may be shared by 
			 *  different same looking characters.
			 */
			struct Character
			{
				char16_t _Character = 0;
				int32 GlyphIndex = 0;
				short Left = 0;
				short Top = 0;
				float AdvanceX = 0;
			};

			/**
			 * Represents a glyph bitmap in the font.
			 */
			struct Glyph
			{
				int32 Index = 0;
				int32 Width = 0;
				int32 Height = 0;
				int64 Offset = 0;

				/**
				 * Is the glyph bitmap stored in the texture pack currently?
				 */
				bool IsMapped = false;
				Apoc3D::Math::Rectangle MappedRect;
				Apoc3D::Math::RectangleF MappedRectF;

				/**
				 *  The number of consecutive grids that the glyph is using.
				 */
				int32 NumberOfGridsUsing = 0;

				/**
				 * The index of the first grid currently used.
				 */
				int32 StartingParentGrid = -1;
			};

			struct CustomGlyph
			{
				char16_t _Character;

				short Left;
				short Top;
				float AdvanceX;

				Apoc3D::Math::RectangleF SrcRect;
				Apoc3D::Math::RectangleF SrcRectF;
				Texture* Graphic;
			};

			struct Grid
			{
				/**
				 * If the grid is used by any glyph currently,
				 * the index of the glyph. Otherwise -1.
				 */
				int32 CurrentGlyph = -1;

				/**
				 * The rect area mapping into the texture pad
				 */
				Apoc3D::Math::RectangleF SrcRect;

				/**
				 * The index of the grid in the grid array
				 */
				int32 GridIndex;
			};

			/**
			 *  Load the glyph bitmap data for a mapped glyph into the m_fontPack texture
			 */
			void LoadGlyphData(BinaryReader* br, Glyph& glyph);

			/**
			 *  Make sure the glyph is loaded and packed in to texture pack
			 */
			void EnsureGlyph(Glyph& glyph);

			/**
			*  Marks a glyph and its grids using by the given glyph or
			*  clears the use when the given glyph pointer is 0
			*/
			void UseGrids(Glyph* g, int32 i, int32 j, int32 amount);

			void FrameStartReset();

			/**
			*  Increase the frequency of use of the grids used by the given glyph
			*/
			void SetUseFreqToMax(const Glyph& g);

			/**
			 * Get the initial font rendering offset coordinate, top left padding
			 */
			PointF GetOrigin(int32 x, int32 y) const;
			PointF GetOrigin(float x, float y) const;

			/**
			 *  Helper function for advancing x coordinate based on the glyph's advanceX
			 */
			void AdvanceXSimple(float& xpos, char16_t ch);


			template <typename UnitType>
			void DrawStringExT(Sprite* sprite, const String& text, UnitType x, UnitType y, uint color, int32 width = 0,
				int32 length = -1, UnitType extLineSpace = 0, char16_t suffix = 0, float hozShrink = 0);

			void DrawCharacter(Sprite* sprite, int32 ch, PointF& pos, uint color, float hozShrink, float extLineSpace, float widthCap, float xOrig, bool pixelAligned);

			void DrawDisolvingCharacter(Sprite* sprite, float x, float y,
				int32 seed, const Apoc3D::Math::RectangleF& srcRect, int32 glyphLeft, int32 glyphTop, int32 glyphWidth, int32 glyphHeight, uint32 color,
				const Point& dissolvePatchSize, float progress);


			static bool IgnoreCharDrawing(int32 ch) { return ch == '\r'; }


			static bool ScanMoveControlCode(const String& str, char16_t& cur, int32& i, int32 len, const PointF* orig, PointF* pos);

			static bool ScanColorControlCodes(const String& str, char16_t& cur, int32& i, int32 len, uint* color);

			static void ScanOtherControlCodes(const String& str, char16_t& cur, int32& i, int32 len);

			/**
			 * A texture that packs as much glyphs as possible within its limits size
			 * This is managed with m_grids.
			 */
			Texture* m_fontPack = nullptr;

			/**
			 * The height of the glyph bitmaps. Typically glyphs in a font are considered to have a fixed height.
			 */
			float m_glyphHeight = 0;
			int32 m_glyphHeightInt = 1;

			float m_ascender = 0;
			float m_descender = 0;
			float m_lineGap = 0;

			/**
			 *  Whether or not the font glyph bitmap has luminance channel.
			 */
			bool m_hasLuminance = false;

			/**
			*  Whether or not the font has a overall draw offset for all characters.
			*/
			bool m_hasDrawOffset = false;
			PointF m_drawOffset = Point::Zero;

			int32 m_selectTextureSize;

			int32 m_maxGlyphWidth = 1;
			int32 m_maxGlyphHeight = 1;

			/**
			 *  Number of grids on one edge
			 */
			int32 m_edgeCount = 0;
			/**
			 *  A copy of the resource location initially used for loading is store here
			 *  for dynamically loading glyphs if needed.
			 */
			ResourceLocation* m_resource = nullptr;

			HashMap<int32, Character> m_charTable;
			Glyph* m_glyphList = nullptr;

			HashMap<int32, CustomGlyph> m_customCharacters;


			/** 
			 *  The entire texture area is divided into 2d plane of grids(m_edgeCount*m_edgeCount), 
			 *  which records the use of every small texture region
			 *
			 *  Grids are row major. Fat glyph use will use more than one grid in a row.
			 *  Other mechanism like m_rowGridsFreqClassificationCount depends on rows too.
			 */
			Grid* m_grids = nullptr;
			
			/** 
			 *  A one to one array containing the use frequency of each grid.
			 *  The frequency here indicates that approximately how many time the 
			 *  grid is used since the beginning of the last frame.
			 */
			int32* m_currentFreqTable = nullptr;
			/** 
			 *  A one to one array containing the last-frame frequency of each grid.
			 *  This is statistical as texts are drawn.
			 */
			int32* m_lastFreqTable = nullptr;
			/** 
			 *  An array of m_edgeCount*MaxFreq, 
			 *  indicates how many grids are there with specific freqs in each row.
			 */
			int32* m_rowGridsFreqClassificationCount = nullptr;
			/** 
			 *  The back up copy of last-frame m_rowGridsFreqClassificationCount
			 */
			int32* m_lasttime_rowGridsFreqClassificationCount = nullptr;

			Random m_gridSearchRandomizer;
			bool m_isUsingCaching = false;
			bool m_usedInFrame = false;

			friend class FontManager;
		
		};

		/**
		 *  Manages font resources, creating them from font files.
		 */
		class APAPI FontManager
		{
			SINGLETON_DECL(FontManager);
		public:
			static int32 MaxTextureSize;

			FontManager();
			~FontManager();

			/**
			 *  Should be called at the beginning of each frame to 
			 *  reset every Font's internal frequency statistics
			 */
			void StartFrame();
			Font* LoadFont(RenderDevice* device, const String& name, const ResourceLocation& rl);

			void ReportComplexFonts();

			Font* getFont(const String& fontName);
			bool hasFont(const String& fontName);

		private:
			HashMap<String, Font*> m_fontTable;

		};
	}
}
#endif