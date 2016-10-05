#pragma once
#ifndef APOC3D_SHADER_H
#define APOC3D_SHADER_H

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

#include "apoc3d/Math/Vector.h"
#include "apoc3d/Graphics/GraphicsCommon.h"

using namespace Apoc3D::Config;
using namespace Apoc3D::Math;
using namespace Apoc3D::Graphics;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			/** Contains the parameters required for texture samplers. */
			struct APAPI ShaderSamplerState 
			{
				TextureAddressMode AddressU = TextureAddressMode::Wrap;
				TextureAddressMode AddressV = TextureAddressMode::Wrap;
				TextureAddressMode AddressW = TextureAddressMode::Wrap;

				uint BorderColor = 0;
				TextureFilter MagFilter = TFLT_Point;
				TextureFilter MinFilter = TFLT_Point;
				TextureFilter MipFilter = TFLT_None;
				int32 MaxAnisotropy = 1;
				int32 MaxMipLevel = 0;
				int32 MipMapLODBias = 0;

				ShaderSamplerState() { }
				void Parse(const ConfigurationSection* sect);
				void Save(ConfigurationSection* sect);
			};

			class APAPI Shader
			{
			public:
				RenderDevice* getRenderDevice() { return m_renderDevice; }

				virtual int GetParamIndex(const String& paramName) = 0;
				virtual int GetSamplerIndex(const String& paramName) = 0;
				virtual bool TryGetParamIndex(const String& paramName, int& result) = 0;
				virtual bool TryGetSamplerIndex(const String& paramName, int& result) = 0;

				virtual void SetVector2(int reg, const Vector2& value) = 0;
				virtual void SetVector3(int reg, const Vector3& value) = 0;
				virtual void SetVector4(int reg, const Vector4& value) = 0;
				virtual void SetValue(int reg, const Quaternion& value) = 0;
				virtual void SetValue(int reg, const Matrix& value) = 0;
				virtual void SetValue(int reg, const Color4& value) = 0;
				virtual void SetValue(int reg, const Plane& value) = 0;

				virtual void SetVector2(int reg, const Vector2* value, int count) = 0;
				virtual void SetVector3(int reg, const Vector3* value, int count) = 0;
				virtual void SetVector4(int reg, const Vector4* value, int count) = 0;
				virtual void SetValue(int reg, const Quaternion* value, int count) = 0;
				virtual void SetValue(int reg, const Matrix* value, int count) = 0;
				virtual void SetValue(int reg, const Color4* value, int count) = 0;
				virtual void SetValue(int reg, const Plane* value, int count) = 0;
				virtual void SetMatrix4x3(int reg, const Matrix* value, int count) = 0;

				virtual void SetValue(int reg, bool value) = 0;
				virtual void SetValue(int reg, float value) = 0;
				virtual void SetValue(int reg, int value) = 0;
				virtual void SetValue(int reg, const bool* value, int count) = 0;
				virtual void SetValue(int reg, const float* value, int count) = 0;
				virtual void SetValue(int reg, const int* value, int count) = 0;

				virtual void SetTexture(int samIndex, Texture* tex) = 0;
				virtual void SetSamplerState(int samIndex, const ShaderSamplerState &state) = 0;


				virtual void SetVector2(const String& paramName, const Vector2& value) = 0;
				virtual void SetVector3(const String& paramName, const Vector3& value) = 0;
				virtual void SetVector4(const String& paramName, const Vector4& value) = 0;
				virtual void SetValue(const String& paramName, const Quaternion& value) = 0;
				virtual void SetValue(const String& paramName, const Matrix& value) = 0;
				virtual void SetValue(const String& paramName, const Color4& value) = 0;
				virtual void SetValue(const String& paramName, const Plane& value) = 0;



				virtual void SetVector2(const String& paramName, const Vector2* value, int count) = 0;
				virtual void SetVector3(const String& paramName, const Vector3* value, int count) = 0;
				virtual void SetVector4(const String& paramName, const Vector4* value, int count) = 0;
				virtual void SetValue(const String& paramName, const Quaternion* value, int count) = 0;
				virtual void SetValue(const String& paramName, const Matrix* value, int count) = 0;
				virtual void SetValue(const String& paramName, const Plane* value, int count) = 0;
				virtual void SetValue(const String& paramName, const Color4* value, int count) = 0;

				virtual void SetValue(const String& paramName, bool value) = 0;
				virtual void SetValue(const String& paramName, float value) = 0;
				virtual void SetValue(const String& paramName, int value) = 0;
				virtual void SetValue(const String& paramName, const bool* value, int count) = 0;
				virtual void SetValue(const String& paramName, const float* value, int count) = 0;
				virtual void SetValue(const String& paramName, const int* value, int count) = 0;

				virtual void SetTexture(const String& paramName, Texture* tex) = 0;
				virtual void SetSamplerState(const String& paramName, const ShaderSamplerState& state) = 0;

				virtual ~Shader() {}

				virtual ShaderType getType() const = 0;
			protected:
				Shader(RenderDevice* rd);

			private:
				RenderDevice* m_renderDevice;

			};

			
		}
	}
}
#endif