#pragma once
#ifndef APOC3D_FRUSTUM_H
#define APOC3D_FRUSTUM_H

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


#include "BoundingSphere.h"
#include "Plane.h"

namespace Apoc3D
{
	namespace Math
	{
		/**
		 * Defines the plane indexes of a frustum
		 */
		enum FrustumPlane
		{
			FPL_Right = 0,
			FPL_Left = 1,
			FPL_Bottom = 2,
			FPL_Top = 3,
			FPL_Far = 4,
			FPL_Near = 5
		};

		/**
		 *  Defines a frustum
		 */
		class APAPI Frustum
		{
		public:
			Frustum();
			~Frustum();

			/*
			 *  Check if a bounding sphere is intersecting the frustum
			 */
			bool Intersects(const BoundingSphere& sp) const
			{
				for (int i = 0; i < ClipPlaneCount; i++)
				{
					float d = m_planes[i].Dot3(sp.Center);
					if (d <= -sp.Radius)
					{
						return false;
					}
				}
				return true;
			}

			/**
			 *  Update the frustum with new view and projection matrix.
			 */
			void Update(const Matrix& view, const Matrix& proj);

			/**
			 *  Update the frustum with new view-projection matrix.
			 */
			void Update(const Matrix& viewProj);

			const Plane& getPlane(FrustumPlane p) const { return m_planes[static_cast<int32>(p)]; }
		private:
			const static int ClipPlaneCount = 6;
			Plane m_planes[ClipPlaneCount];

		};
	};
};
#endif