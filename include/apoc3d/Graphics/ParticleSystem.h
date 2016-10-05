#pragma once
#ifndef APOC3D_PARTICLESYSTEM_H
#define APOC3D_PARTICLESYSTEM_H

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

#include "ParticleSettings.h"
#include "Material.h"
#include "GeometryData.h"
#include "RenderSystem/VertexElement.h"
#include "Renderable.h"
#include "RenderOperationBuffer.h"

using namespace Apoc3D::Core;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{
		/** Custom vertex structure for drawing point sprite particles. */
		struct APAPI ParticleVertex
		{
			Vector3 Position;			/** Stores the starting position of the particle. */
			Vector3 Velocity;			/** Stores the starting velocity of the particle. */
			uint32 Random;		/** Four random values, used to make each particle look slightly different. */
			
			float Time;		/** The time (in seconds) at which this particle was created. */

			/** Describe the layout of this vertex structure. */
			static const VertexElement VtxElements[];
		};

		/**
		 *  in charge of displaying particles.
		 */
		class APAPI ParticleSystem : public Renderable
		{
		public:
			ParticleSystem(RenderDevice* device, Material* mtrl);
			virtual ~ParticleSystem();

			void Setup(FunctorReference<void(ParticleSettings&)> settingsFunc);

			void Reset()
			{
				m_firstActiveParticle = 0;
				m_firstNewParticle = 0;
				m_firstFreeParticle = 0;
				m_firstRetiredParticle = 0;
				m_currentTime = 0;
				m_drawCounter = 0;
			}
			void Reset(Material* mtrl)
			{
				Reset();
				changeMaterial(mtrl);
			}

			float getCurrentTime() const { return m_currentTime; }
			const ParticleSettings* getSettings() const { return &m_settings; }
			ParticleSettings* getSettings() { return &m_settings; }

			virtual RenderOperationBuffer* GetRenderOperation(int lod);

			/* Updates the particle system. */
			virtual void Update(float dt);

			/* Adds a new particle to the system. */
			bool AddParticle(const Vector3 &position, const Vector3& _velocity);
			
		protected:
			void changeMaterial(Material* mtrl) { m_mtrl = mtrl; }

			/* Derived particle system classes should override this method
			   and use it to initialize their tweakable settings.
			*/
			virtual void InitializeSettings(ParticleSettings &settings) const { }

			virtual void Load();
		private:

			RenderDevice* m_device;

			/** Settings class controls the appearance and animation of this particle system. */
			ParticleSettings m_settings;
			Material* m_mtrl;

			/** An array of particles, treated as a circular queue. */
			ParticleVertex* m_particles = nullptr;
			int32 m_particleCount = 0;

			/**
			 *  A vertex buffer holding our particles. This contains the same data as
			 *  the particles array, but copied across to where the GPU can access it.
			 */
			VertexBuffer* m_vertexBuffer = nullptr;

			/** Vertex declaration describes the format of our ParticleVertex structure. */
			VertexDeclaration* m_vertexDeclaration = nullptr;

			GeometryData m_geoData;
			GeometryData m_geoDataAlt;
			RenderOperationBuffer m_opBuffer;

			/* The particles array and vertex buffer are treated as a circular queue.
			   Initially, the entire contents of the array are free, because no particles
			   are in use. When a new particle is created, this is allocated from the
			   beginning of the array. If more than one particle is created, these will
			   always be stored in a consecutive block of array elements. Because all
			   particles last for the same amount of time, old particles will always be
			   removed in order from the start of this active particle region, so the
			   active and free regions will never be intermingled. Because the queue is
			   circular, there can be times when the active particle region wraps from the
			   end of the array back to the start. The queue uses modulo arithmetic to
			   handle these cases. For instance with a four entry queue we could have:
			  
			        0
			        1 - first active particle
			        2 
			        3 - first free particle
			  
			   In this case, particles 1 and 2 are active, while 3 and 4 are free.
			   Using modulo arithmetic we could also have:
			  
			        0
			        1 - first free particle
			        2 
			        3 - first active particle
			  
			   Here, 3 and 0 are active, while 1 and 2 are free.
			  
			   But wait! The full story is even more complex.
			  
			   When we create a new particle, we add them to our managed particles array.
			   We also need to copy this new data into the GPU vertex buffer, but we don't
			   want to do that straight away, because setting new data into a vertex buffer
			   can be an expensive operation. If we are going to be adding several particles
			   in a single frame, it is faster to initially just store them in our managed
			   array, and then later upload them all to the GPU in one single call. So our
			   queue also needs a region for storing new particles that have been added to
			   the managed array but not yet uploaded to the vertex buffer.
			  
			   Another issue occurs when old particles are retired. The CPU and GPU run
			   asynchronously, so the GPU will often still be busy drawing the previous
			   frame while the CPU is working on the next frame. This can cause a
			   synchronization problem if an old particle is retired, and then immediately
			   overwritten by a new one, because the CPU might try to change the contents
			   of the vertex buffer while the GPU is still busy drawing the old data from
			   it. Normally the graphics driver will take care of this by waiting until
			   the GPU has finished drawing inside the VertexBuffer.SetData call, but we
			   don't want to waste time waiting around every time we try to add a new
			   particle! To avoid this delay, we can specify the SetDataOptions.NoOverwrite
			   flag when we write to the vertex buffer. This basically means "I promise I
			   will never try to overwrite any data that the GPU might still be using, so
			   you can just go ahead and update the buffer straight away". To keep this
			   promise, we must avoid reusing vertices immediately after they are drawn.
			  
			   So in total, our queue contains four different regions:
			  
			   Vertices between firstActiveParticle and firstNewParticle are actively
			   being drawn, and exist in both the managed particles array and the GPU
			   vertex buffer.
			  
			   Vertices between firstNewParticle and firstFreeParticle are newly created,
			   and exist only in the managed particles array. These need to be uploaded
			   to the GPU at the start of the next draw call.
			  
			   Vertices between firstFreeParticle and firstRetiredParticle are free and
			   waiting to be allocated.
			  
			   Vertices between firstRetiredParticle and firstActiveParticle are no longer
			   being drawn, but were drawn recently enough that the GPU could still be
			   using them. These need to be kept around for a few more frames before they
			   can be reallocated.
			*/
			int32 m_firstActiveParticle = 0;
			int32 m_firstNewParticle = 0;
			int32 m_firstFreeParticle = 0;
			int32 m_firstRetiredParticle = 0;

			/** Store the current time, in seconds. */
			float m_currentTime = 0;

			/**
			 *  Count how many times Draw has been called. This is used to know
			 *  when it is safe to retire old particles back into the free list.
			 */
			int32 m_drawCounter = 0;


			/**
			 *  uploading new particles from our managed
			 *  array to the GPU vertex buffer.
			 */
			void AddNewParticlesToVertexBuffer();

			/** checking when active particles have reached the end of
			 *  their life. It moves old particles from the active area of the queue
			 *  to the retired section. 
			 */
			void RetireActiveParticles();
			/** checking when retired particles have been kept around long
			 *  enough that we can be sure the GPU is no longer using them. It moves
			 *  old particles from the retired area of the queue to the free section.
			 */
			void FreeRetiredParticles();

		};


	}

}
#endif