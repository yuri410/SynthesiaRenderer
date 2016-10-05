#pragma once
#ifndef APOC3D_LIBRARY_H
#define APOC3D_LIBRARY_H

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

using namespace Apoc3D;

namespace Apoc3D
{
	namespace Platform
	{
		/** 
		 *  Represents a dynamic library.
		 */
		class APAPI Library
		{
		private:
			String m_name;

			byte m_handle[8];
		public:
			Library(String name)
				: m_name(name)
			{}

			const String &getName() const { return m_name; }
			/** 
			 * 
			 */
			void* getSymbolAddress(const String &name) const;

			/**
			 *  Dynamically loads the library into memory.
			 *  Exported functions on the library can be called once it is loaded.
			 */
			void Load();
			void Unload();


		};
	}
}


#endif