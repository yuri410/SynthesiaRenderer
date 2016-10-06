#pragma once
#ifndef APOC3D_CAMERA_H
#define APOC3D_CAMERA_H

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

#include "apoc3d/Math/Frustum.h"
#include "apoc3d/Math/Ray.h"

using namespace Apoc3D::Math;
using namespace Apoc3D::Core;

namespace Apoc3D
{
	namespace Graphics
	{
		/** Represents a view into a 3D scene. */
		class APAPI Camera
		{
			RTTI_BASE;
		public:

			Camera();
			Camera(const Matrix& view, const Matrix& proj);
			~Camera() { }

			void CalculateMatrices();

			void GetCornerRays(RaySegment* topLeft, RaySegment* topRight, RaySegment* bottomLeft, RaySegment* bottomRight);
			void GetEdgeRays(RaySegment* left, RaySegment* right, RaySegment* top, RaySegment* bottom);

			const Frustum& getFrustum() { return m_frustum; }

			const Matrix& getViewMatrix() const { return m_view; }		/** Gets the view transform matrix */
			const Matrix& getProjMatrix() const { return m_proj; }		/** Gets the projection matrix */

			const Matrix& getInvViewMatrix() const { return m_invView; }

			const Matrix& getViewProjMatrix() const { return m_viewProj; }

			Vector3 getUp() const { return Vector3(m_invView.M21, m_invView.M22, m_invView.M23); }			/** Gets the up vector of the camera view */
			Vector3 getRight() const { return Vector3(m_invView.M11, m_view.M12, m_invView.M13); }			/** Gets the right vector of the camera view */
			Vector3 getForward() const { return Vector3(m_invView.M31, m_invView.M32, m_invView.M33); }		/** Gets the forward vector of the camera view */

			void setViewMatrix(const Matrix& value) { m_view = value; }		/** Sets the view transform matrix */
			void setProjMatrix(const Matrix& value) { m_proj = value; }		/** Sets the projection transform matrix */

		protected:

			Matrix m_view;
			Matrix m_invView;
			Matrix m_proj;
			Matrix m_viewProj;

			Frustum m_frustum;
		};

		class APAPI FreeCamera : public Camera
		{
		public:
			FreeCamera(float aspectRatio, bool rightHand=false);
			~FreeCamera();

			/** Gets the position of the view point */
			const Vector3& getPosition() const { return m_position; }
			
			void MoveForward();
			void MoveBackward();
			void MoveLeft();
			void MoveRight();
			void MoveUp();
			void MoveDown();
			void Move(const Vector3 &dir);
			
			void Turn(float dx, float dy);

			void Update(const GameTime* time);
			void UpdateTransform();

			void setPosition(const Vector3& p)
			{
				m_position = p;
			}

			float getNear() const { return m_near; }					/** The near plane of the frustum */
			float getFar() const { return m_far; }						/** The far plane of the frustum */

			float getAspectRatio() const { return m_aspectRatio; }		/** The aspect ratio of the projection */
			float getFieldOfView() const { return m_fieldOfView; }		/** The y direction fov angle in radians */

			void setNear(float val) { m_near = val; }
			void setFar(float val) { m_far = val; }

			void setAspectRatio(float val) { m_aspectRatio = val; }
			void setFieldOfView(float val) { m_fieldOfView = val; }


			float MaxVelocity = 5;
			float Deacceleration = 5;
			float Acceleration = 5;
		protected:
			Vector3 m_position = Vector3::Zero;
			Vector3 m_velocity = Vector3::Zero;
			Vector3 m_velChange = Vector3::Zero;

		private:
			bool m_rightHand = false;

			float m_aspectRatio;
			
			float m_fieldOfView;
			float m_near;
			float m_far;

			float m_rotX = 0;
			float m_rotY = 0;

		};

		/** Implements a 3rd person chase camera with inertia */
		class APAPI ChaseCamera : public Camera
		{
		public:
			ChaseCamera(float fov = ToRadian(45));
			~ChaseCamera();

			/** Resets the camera to its desired position */
			void Reset();

			/** Update the camera's motion simulation and the view */
			virtual void Update(const GameTime* time);

			void ForceUpdateMatrix() { UpdateMatrices(); }

			/**
			 *  A unit vector showing the up direction of the camera
			 *  (0,1,0) will have no rolling
			 */
			const Vector3& getChaseUp() const { return m_up; }
			const Vector3& getChaseDirection() const { return m_chaseDirection; }	/** A unit vector representing the view direction to the chase target */
			const Vector3& getChasePosition() const { return m_chasePosition; }		/** The position of the chase target */

			const Vector3& getPosition() const { return m_position; }				/** Gets the camera's current eye position in world space */
			const Vector3& getVelocity() const { return m_velocity; }				/** Gets the camera's current linear velocity in world space */
			/**
			 *  Gets the offset from the target to the camera for the camera to move to.
			 *  This is relative to the chase direction.
			 */
			const Vector3& getDesiredOffset() const { return m_desiredPositionOfs; }
			
			const Vector3& getLookAtOffset() const { return m_lootAtOfs; }				/**  Gets the offset from the target to the look at position, in world space */
			const Vector3& getDesiredPosition() const { return m_desiredPosition; }		/**  Get the current camera's position desired to be moving to. */
			const Vector3& getLookAt() const { return m_lootAt; }						/**  Gets the current camera's look at position in world space */

			float getStiffness() const { return m_stiffness; }			/** How rigid the movement is. */
			float getDamping() const { return m_damping; }				/** The friction applied to movement using the damping model: f = vel*damp */
			float getMass() const { return m_mass; }					/** The mass of the camera */
			

			void setStiffness(float val) { m_stiffness = val; }
			void setDamping(float val) { m_damping = val; }
			void setMass(float val) { m_mass = val; }
			
			void setChasePosition(const Vector3& val) { m_chasePosition = val; }
			void setChaseDirection(const Vector3& val) { m_chaseDirection = val; }
			void setVelocity(const Vector3& val) { m_velocity = val; }
			void setChaseUp(const Vector3& val) { m_up = val; }
			void setPosition(const Vector3& val) { m_position = val; }
			void setDesiredOffset(const Vector3& val) { m_desiredPositionOfs = val; }
			void setLookAtOffset(const Vector3& val) { m_lootAtOfs = val; }


			float getNear() const { return m_near; }					/** The near plane of the frustum */
			float getFar() const { return m_far; }						/** The far plane of the frustum */

			float getAspectRatio() const { return m_aspectRatio; }		/** The aspect ratio of the projection */
			float getFieldOfView() const { return m_fieldOfView; }		/** The y direction fov angle in radians */

			void setNear(float val) { m_near = val; }
			void setFar(float val) { m_far = val; }

			void setAspectRatio(float val) { m_aspectRatio = val; }
			void setFieldOfView(float val) { m_fieldOfView = val; }

		private:
			void UpdateWorldPositions();
			void UpdateMatrices();

			// Perspective properties
			float m_aspectRatio;
			float m_fieldOfView;
			float m_near;
			float m_far;

			// Current camera properties
			Vector3 m_position;
			Vector3 m_velocity;

			// chased object prop
			Vector3 m_up;
			Vector3 m_chasePosition;
			Vector3 m_chaseDirection;

			// Desired camera positioning
			Vector3 m_desiredPositionOfs;
			Vector3 m_desiredPosition;
			Vector3 m_lootAtOfs;
			Vector3 m_lootAt;

			// Camera physics
			float m_stiffness;
			float m_damping;
			float m_mass;


		};
	};
};
#endif