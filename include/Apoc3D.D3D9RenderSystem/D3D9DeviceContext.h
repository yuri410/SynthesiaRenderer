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
#ifndef D3D9DDEVICECONTEXT_H
#define D3D9DDEVICECONTEXT_H

#include "D3D9Common.h"
#include "apoc3d/Graphics/RenderSystem/DeviceContext.h"

using namespace Apoc3D::Graphics::RenderSystem;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			class D3D9DeviceContext : public DeviceContext
			{
			public:
				D3D9DeviceContext();
				~D3D9DeviceContext();

				virtual List<RenderDisplayMode> GetSupportedDisplayModes();
				virtual String GetHardwareName();
				
				virtual RenderDevice* getRenderDevice();
				
				IDirect3D9* getD3D() const { return m_d3d9; }

				void NotifyWindowClosed(D3D9RenderWindow* wnd);

			protected:
				virtual RenderView* create(const RenderParameters &pm);
			private:
				String m_hardwareName;
				IDirect3D9* m_d3d9;

				D3D9RenderWindow* m_window;
				D3D9RenderViewSet* m_viewSet;
			};
		}
	}
}

#endif