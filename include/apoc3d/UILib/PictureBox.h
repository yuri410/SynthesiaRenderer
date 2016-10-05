#pragma once
#ifndef APOC3D_PICTUREBOX_H
#define APOC3D_PICTUREBOX_H

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

using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace UI
	{
		typedef EventDelegate<Sprite*, Apoc3D::Math::Rectangle*> PictureDrawEventHandler;

		class APAPI PictureBox : public Control
		{
			RTTI_DERIVED(PictureBox, Control);
		public:
			typedef EventDelegate<PictureBox*, const GameTime*> InteractiveUpdateEventHandler;

			PictureBox(const Point& position, int border);
			PictureBox(const Point& position, int border, Texture* texture);
			PictureBox(const StyleSkin* skin, const Point& position, int border);
			PictureBox(const StyleSkin* skin, const Point& position, int border, Texture* texture);

			virtual ~PictureBox();

			virtual void Update(const GameTime* time);
			virtual void Draw(Sprite* sprite);

			void setSize(const Point& sz) { m_size = sz; }
			void setSize(int32 w, int32 h) { m_size = Point(w, h); }
			void setWidth(int32 w) { m_size.X = w; }
			void setHeight(int32 h) { m_size.Y = h; }

			//Texture* getImage() const { return m_texture; }
			//void setImage(Texture* tex) { m_texture = tex; }

			//Texture* m_texture = nullptr;
			//Apoc3D::Math::Rectangle m_srcRect;

			UIGraphicSimple Graphic;

			PictureDrawEventHandler eventPictureDraw;

			UIEventHandler eventMouseHover;
			UIEventHandler eventMouseOut;
			UIEventHandler eventPress;
			UIEventHandler eventRelease;

			InteractiveUpdateEventHandler eventInteractiveUpdate;

		private:
			void OnMouseHover();
			void OnMouseOut();
			void OnPress();
			void OnRelease();

			bool m_mouseDown = false;
			bool m_mouseHover = false;
			int m_border = 0;
			
		};
	}
}

#endif