#pragma once
#ifndef APOC3D_UTILS_GRAPH_H
#define APOC3D_UTILS_GRAPH_H

#include "apoc3d/Common.h"
#include "apoc3d/Math/Vector.h"
#include "apoc3d/Math/Rectangle.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/LinkedList.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Core/BackgroundSequencialWorker.h"

#include "apoc3d/Meta/FunctorReference.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Core;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Utility
	{
		class GraphNode;
		class QuadTreeNode;

		struct GraphNodeInfo
		{
			int32 ID;
			Vector2 Position;
		};
		struct GraphNodeDefinition
		{
			int32 ID;
			bool HasPosition = false;
			Vector2 Position;
			float HitTestRadius;
			List<int32> Neighbors;
		};

		/** Graph layout calculation.
		*
		*  ==Method TECH_Quad==
		*  Visualization:
		*   Graph layout calculation is based on the
		*   force directed algorithm, and optimized by using space decomposition from a quad tree.
		*   To calculating the N-body repulsion forces with less time,
		*   the algorithm evaluate the force from distant repulsion forces based on
		*   the higher-level nodes in the quad tree.
		*  Updating the quad tree:
		*   Once a graph node is moved, and when such translation is crossing leaf nodes(in the quad tree),
		*   the node will be attached to a new leaf node and tag both node dirty ones, whose parent will
		*   be updated on properties like the number of graph node the sub tree contains, the center of mass.
		*  Physics Model:
		*   Planned: spring and gravitational force.
		*
		*  ==TECH_BruteForce==
		*   The lazy way. Gravitational force is calculated against other nodes one by one
		*
		*  ==TECH_Fuzzy==
		*   Gravitational force is calculated against other neighbor nodes and the whole graph's center of mass
		*
		*/
		class GraphLayout
		{
		public:
			/** Enumeration containing the methods supported to solve the layout */
			enum Technique
			{
				TECH_NoRepulse,
				TECH_BruteForce,
				/** The default way */
				TECH_Quad,
				/** The repulsive is estimate as emitted from a point at geometric center with the total mass */
				TECH_Fuzzy

			};
			GraphLayout(int32 areaEdgeLength = 2048, int32 workerThreads = 4);
			~GraphLayout();

			void Load(List<GraphNodeDefinition>& graphInfo, bool forceRandom);
			void Save(List<GraphNodeDefinition>& graphInfo);

			/** Disposes the current open document */
			void Reset();

			void ActivateLayout() { m_isProcessingLayout = true; }
			void DectivateLayout() { m_isProcessingLayout = false; }

			void MoveNode(int32 id, float dx, float dy);
			void AddNode(int32 id, float x, float y, float hitTestRadius);
			void DeleteNode(int32 id);
			
			/** Find the nodes intersected with the given point in view port, using the quad tree */
			void IntersectNodes(int mx, int my);
			
			const List<GraphNode*>& getInternalNodes() const { return m_nodes; }
			
			bool getIsProcessingLayout() const { return m_isProcessingLayout; }

			float getAdaptiveTimeScale() const { return m_adaptiveTimeScale; }
			float getCurrentEnergy() const { return m_currentKEnergy; }

			float getFrameProgress() const { return m_frameProgress; }

			Technique getTechnique() const { return m_technique; }
			void setTechnique(Technique tech);

			List<GraphNodeInfo> getVisibleNodes();
			List<GraphNodeInfo> getIntersectingNodes();

			void getGraphArea(PointF& minPos, PointF& maxPos) const
			{
				minPos = { -m_areaEdgeLength * 0.5f, -m_areaEdgeLength * 0.5f };
				maxPos = -minPos;
			}

			Apoc3D::Math::RectangleF Viewport;

		private:
			struct PhysicsTask
			{
				enum 
				{
					Pre,
					Mid,
					Post
				} TaskType;
				
				GraphNode* Subject = nullptr;
				float DT = 0;
				Technique SelectedTechnique;

				PhysicsTask() { }
				PhysicsTask(int32 tskType, GraphNode* obj, float dt, Technique tec)
					: TaskType((decltype(TaskType))tskType), Subject(obj), DT(dt), SelectedTechnique(tec) { }
			};

			struct PhysicsWorker : public BackgroundSequencialWorker<PhysicsTask>
			{
				PhysicsWorker(int32 idx);
				virtual void BackgroundMainProcess(PhysicsTask& item) override;
			};

			enum struct CommandType
			{
				Reset,
				Intersect,
				Add,
				Remove,
				Move
			};

			struct LayoutCommand
			{
				CommandType Cmd;

				int32 ID = 0;
				float X = 0;
				float Y = 0;
				float R = 0;
			};


			/** Converts a world coord to the quad tree's block coord system,
			*  a pair of integer representing the grid in a plane divided all
			*  the quad tree's leaf node, i.e. the flatten leaf nodes to a 2D array.
			*/
			void ConvertCoord(const Vector2& pos, int& qx, int& qy);

			void PrepareAndWaitForIO();
			void CompleteIO();


			static void LayoutThreadEntry(void*);
			void LayoutThreadMain();

			void UpdateLayout(float dt);

			/** If a GraphNode has crossed 2 quad tree node, detach it
			* from the old one(if there is), and then attach it GraphNode to the new one.
			*/
			void PutGraphNode(GraphNode* gn);

			void InitialPlacement();


			void FindVisisbleNodes();


			List<GraphNode*> m_nodes;

			QuadTreeNode* m_quadTree;
			
			QuadTreeNode** m_leafNodes;					/** the flatten leaf nodes to a 2D array. */
			
			HashMap<int32, GraphNode*> m_idMapping;

			int32 m_areaEdgeLength;
			int32 m_areaEdgeLeafNodeCount;
			int32 m_leafNodeEdgeLength;

			bool m_isProcessingLayout;					/** State representing whether the graph is currently being processed for layout. */
			bool m_isShuttingDown = false;
			bool m_isDoingIO = false;
			bool m_isReadyForIO = false;

			float m_frameProgress = 0;

			int m_quadTreeUpdateInterval;				/** The number of frame between times updating the quad tree */

			int m_adaptiveLayoutIterationPerFrame;		/** number of iterations acceptable not to make the application quite laggy */

			float m_adaptiveTimeScale;					/** The time step multiplier, calculated based on the maximum kinetic energy among all the nodes. */

			float m_currentKEnergy;						/** The current-time total kinetic energy */

			Vector2 m_centerOfMass = Vector2::Zero;		/** Center of mass of the entire graph, only calculated and used for TECH_Fuzzy */



			tthread::mutex m_layoutCommandLock;
			Technique m_technique = TECH_Quad;
			Queue<LayoutCommand> m_commandQueue;
			
			tthread::thread* m_layoutThread;
			List<PhysicsWorker*> m_physicsWorkers;

			tthread::mutex m_intersectingNodesLock;
			List<GraphNodeInfo>* m_intersectingNodes = new List<GraphNodeInfo>();
			List<GraphNodeInfo>* m_intersectingNodes2 = new List<GraphNodeInfo>();


			tthread::mutex m_visibleNodesLock;
			List<GraphNodeInfo>* m_currentVisisbleNodes = new List<GraphNodeInfo>();
			List<GraphNodeInfo>* m_currentVisisbleNodes2 = new List<GraphNodeInfo>();

		};

		/** Represents a node in the graph. */
		class GraphNode
		{
		public:
			GraphNode(GraphLayout* graph, QuadTreeNode* tree, int32 id, float hitTestRadius);

			void BeginPhysicsStep();
			void EndPhysicsStep();

			/** Do the physics work for the graph visualization algorithm */
			void PhysicsStep(float dt, GraphLayout::Technique selectedTech);

			/** Adds a node to this one's neighbor */
			void Link(GraphNode* n)
			{
				assert(n != this);
				if (!m_neighbour.Contains(n))
					m_neighbour.Add(n);
			}

			/** Removes a node from this one's neighbor list */
			void Unlink(GraphNode* n)
			{
				assert(n != this);
				m_neighbour.Remove(n);
			}
			
			void ApplyImpulse(const Vector2& imp)
			{
				// apply directly on the velocity since the mass is 1
				m_velocity += imp;
			}


			int32 getID() const { return m_nodeID; }
			
			bool getIsPositionDirty() const { return m_isPositionDirty; }
			void setIsPositionDirty(bool v) { m_isPositionDirty = v; }

			void setDockingNode(QuadTreeNode* n) { m_dockingNode = n; }
			QuadTreeNode* getDockingNode() const { return m_dockingNode; }

			const Vector2& getPosition() const { return m_position; }
			void setPosition(const Vector2& pos) { m_position = pos; m_info.Position = pos; m_isPositionDirty = true; }
			const List<GraphNode*>& getNeighbors() const { return m_neighbour; }
			//const List<GraphNode*>& getTargets() const { return m_targets; }
			float getMass() const;
			float getKineticEnergy() const { return m_kineticEnergy; }

			float getHitTestRadius() const { return m_hitTestRadius; }

			const GraphNodeInfo& getInfo() const { return m_info; }
		private:
			/** Calculate the repulsive force by walking the quad tree */
			void RecursiveRepulsiveForce(QuadTreeNode* node, Vector2& force);

			Vector2 m_velocity = Vector2::Zero;
			Vector2 m_position = Vector2::Zero;
			float m_kineticEnergy = 0;

			Vector2 m_newVelocity = Vector2::Zero;
			Vector2 m_newPosition = Vector2::Zero;
			float m_newKineticEnergy = 0;


			bool m_isPositionDirty = false;
			bool m_isInPhysicsStep = false;

			GraphNodeInfo m_info;
			int32 m_nodeID = 0;
			float m_hitTestRadius = 0;

			List<GraphNode*> m_neighbour;

			GraphLayout* m_graph = nullptr;
			QuadTreeNode* m_tree = nullptr;
			
			QuadTreeNode* m_dockingNode = nullptr;				/** The leaf node the GraphNode is attached */

		};


		class QuadTreeNode
		{
		public:
			enum NodeCorner
			{
				QTN_TopLeft = 0,
				QTN_TopRight = 1,
				QTN_BottomLeft = 2,
				QTN_BottomRight = 3
			};

			/** Creates a quad tree root, and recursively build the entire structure.
			* @param handler Called back once the leaf node are created, providing the leaf node as a parameter
			* @param maxDepth Specifies the depth of the tree.
			* @param parent The parent node of the node being created. Use 0 if the node is a root node.
			* @param rect The 2D region the quad tree covers.
			*/
			QuadTreeNode(QuadTreeNode* parent, const Apoc3D::Math::RectangleF& rect, int maxDepth, FunctorReference<void(QuadTreeNode*)> leafNodeCreated);
			~QuadTreeNode();

			QuadTreeNode* getNode(NodeCorner corner) const { return m_subNodes[(int)corner]; }

			/** Clears the graph nodes attached on the nodes in sub tree */
			void Clear();

			/** [leaf node only]
			*  Attaches a GraphNode to this leaf quad tree node.
			*  Attached objects are GraphNodes within the area of
			*  this leaf node.
			*/
			void Attach(GraphNode* node)
			{
				assert(m_isLeafNode);
				m_attachedGraphNodes.PushBack(node);
				MarkDirty();
			}
			/** [leaf node only]
			*  Detaches a GraphNode to this leaf quad tree node.
			*/
			void Detach(GraphNode* node)
			{
				assert(m_isLeafNode);

				m_attachedGraphNodes.Remove(node);
				MarkDirty();
			}

			/** Update on properties like the number of graph node the sub tree contains, the center of mass,
			*  and total mass.
			*/
			void Update(float dt);


			/** Find the node intersected with the given point */
			void IntersectNodes(const Vector2& pt, List<GraphNodeInfo>& nodes);

			/** Fill all sub nodes' attachment if a node is intersecting the given area. */
			void FillIntersectingNodesAttachment(List<GraphNodeInfo>& list, const Apoc3D::Math::RectangleF& area);

			const RectangleF& getArea() const { return m_area; }
			const LinkedList<GraphNode*>& getAttachedNodes() { return m_attachedGraphNodes; }
			const Vector2& getCenterOfMass() const { return m_centerOfMass; }
			const float getMass() const { return m_equviliantMass; }

			void MakeAllDirty();

		private:
			void MarkDirty()
			{
				assert(m_isLeafNode);
				PopDirty();
			}

			/** Mark the node as well all the way from its parent node to the root node dirty */
			void PopDirty()
			{
				m_isDirty = true;
				if (m_parent)
					m_parent->PopDirty();
			}
			void MakeAllDirty(QuadTreeNode* node);

			QuadTreeNode* m_parent;
			QuadTreeNode* m_subNodes[4];

			LinkedList<GraphNode*> m_attachedGraphNodes;
			bool m_isLeafNode;

			/** Dirty means that the node's m_totalAttachedInSubtree, m_equviliantMass
			*  and m_centerOfMass need to be updated and recalculated based on the
			*  attached GraphNodes
			*/
			bool m_isDirty;

			RectangleF m_area;

			float m_equviliantMass;
			Vector2 m_centerOfMass;
			int m_totalAttachedInSubtree;


		};

	}
}

#endif