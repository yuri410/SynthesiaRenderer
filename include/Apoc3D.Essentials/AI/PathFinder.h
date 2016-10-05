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
#ifndef PATHFINDER_H
#define PATHFINDER_H


#include "../EssentialCommon.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Collections/Queue.h"
#include "apoc3d/Math/Point.h"

using namespace Apoc3D;
using namespace Apoc3D::Core;
using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;

namespace Apoc3DEx
{
	namespace AI
	{
		class AStarNode;
		class PathFinderField;
		class PathFinder;
		class PathFinderResult;
	
		class APEXAPI PathFinderManager
		{
		public:
			PathFinderManager(PathFinderField* fld);
			~PathFinderManager();

			PathFinder* CreatePathFinder();
			PathFinderField* getFieldTable() const { return m_terrain; }
		private:
			AStarNode* m_units;
			PathFinderField* m_terrain;

			friend class PathFinder;
		};

		class APEXAPI PathFinder
		{
		public:
			PathFinder(PathFinderManager* mgr);
			PathFinder(PathFinderField* terrain, AStarNode* units);

			void Reset();
			void Continue();
			PathFinderResult* FindPath(int sx, int sy, int tx, int ty);

			void AddExpansionDirection(int32 dx, int32 dy, float cost);
			void ResetExpansionDirections() { m_pathExpansionEnum.Clear(); }
			void Set8DirectionTable();
			void Set4DirectionTable();

			void AddNeighborCostInclusion(int32 dx, int32 dy);
			void ResetNeighborCostInclusion() { m_neighborCosts.Clear(); }

			int MaxStep;

			float TurnCost;
			bool ConsiderFieldDifferencialWeightCost;
			bool ConsiderFieldWeightCost;

			bool UseManhattanDistance;
		private:
			struct ExpansionDirection
			{
				int32 dx, dy;
				float cost;
			};
			struct NeighorCostInclusion
			{
				int32 dx, dy;
				//float weight;
			};

			inline AStarNode* getNode(int32 x, int32 y);
			inline static int32 AStarNodeComparer(AStarNode* const& a, AStarNode* const& b);

			AStarNode* m_units;
			PathFinderField* m_terrain;

			/** bfs search queue
			*/
			PriorityQueue<AStarNode*, &AStarNodeComparer> m_queue;
			

			/** used to fast check if a node is in the queue
			*/
			HashMap<int, AStarNode*> m_inQueueTable;
			
			/** used to fast check if a node is already passed by the algorithm
			*/
			HashMap<int, AStarNode*> m_passedTable;

			int m_width;
			int m_height;

			List<Point> m_result;


			List<ExpansionDirection> m_pathExpansionEnum;
			List<NeighorCostInclusion> m_neighborCosts;
		};

		class APEXAPI PathFinderField
		{
		public:
			PathFinderField(int32 w, int32 h);
			~PathFinderField();
			
			int32 getWidth() { return m_width; }
			int32 getHeight() { return m_height; }

			bool isPassable(int32 x, int32 y) const { return m_fieldPassable[y * m_width + x]; }
			void setPassable(int32 x, int32 y, bool passable) { m_fieldPassable[y * m_width + x] = passable; }

			float getFieldWeight(int32 x, int32 y) const { return m_fieldWeight[y * m_width + x]; }
			void setFieldWeight(int32 x, int32 y, float wgt) { m_fieldWeight[y * m_width + x] = wgt; }

			float getDifferencialFieldWeight(int32 x, int32 y) const { return m_fieldDifferencialWeight[y * m_width + x]; }
			void setDifferencialFieldWeight(int32 x, int32 y, float wgt) { m_fieldDifferencialWeight[y * m_width + x] = wgt; }

			float calculateDifferencialWeight(int32 cx, int32 cy, int32 nx, int32 ny) const
			{
				float d = getDifferencialFieldWeight(nx,ny)-getDifferencialFieldWeight(cx, cy);
				if (d<0)d = 0;
				return d;
			}
		private:
			int m_width;
			int m_height;

			bool* m_fieldPassable;
			float* m_fieldWeight;
			float* m_fieldDifferencialWeight;
			
		};

		class APEXAPI PathFinderResult
		{
		public:

			PathFinderResult(const List<Point>& path, bool rcpf)
				: m_path(path), m_requiresContinuePathFinding(rcpf)
			{
			}

			const Point& operator [](int idx) const
			{
				return m_path[idx];
			}
			int getNodeCount() const { return m_path.getCount(); }

			bool RequiresContinePathFinding() const { return m_requiresContinuePathFinding; }
		private:
			bool m_requiresContinuePathFinding;
			List<Point> m_path;
		};

		class APEXAPI AStarNode
		{
		public:
			int X;
			int Y;

			float f;
			float g;
			float h;

			int depth;

			AStarNode* parent;

			AStarNode()
				: X(0), Y(0), f(0), g(0), h(0), depth(0), parent(0) 
			{ }

			AStarNode(int x, int y)
				: X(x), Y(y), f(0), g(0), h(0), depth(0), parent(0)
			{ }

			int GetHashCode() const { return (X<<16) | Y; }
		};

		AStarNode* PathFinder::getNode(int32 x, int32 y) { return &m_units[y * m_width + x]; }
		int32 PathFinder::AStarNodeComparer(AStarNode* const& a, AStarNode* const& b) { return OrderComparer<float>(a->f, b->f); }

	}
}
#endif