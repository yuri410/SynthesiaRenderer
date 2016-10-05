#pragma once
#ifndef APOC3D_EFFECTPARAMETERS_H
#define APOC3D_EFFECTPARAMETERS_H

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
#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Math/Color.h"
#include "apoc3d/Math/Vector.h"
#include "apoc3d/Math/Matrix.h"
#include "apoc3d/Graphics/RenderSystem/Shader.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Graphics::RenderSystem;
using namespace Apoc3D::Math;
using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace EffectSystem
		{
			/**
			 *   Defines usage of a effect parameters.
			 *
			 *   When the engine auto bind a parameter, it first checks the parameter's usage to
			 *   find the corresponding data, then assign it to the param. 
			 *
			 *   EffectParamUsage has 3 special usages as follows:
			 *   1. EPUSAGE_Unknow.
			 *   2. EPUSAGE_InstanceBlob
			 *   3. EPUSAGE_CustomMaterialParam
			 *   See the fields for detailed description.
			 */
			enum EffectParamUsage
			{
				/**
				 *  The unknown usage means the binding for typical usage is unavailable, 
				 *  the engine will do nothing.
				 *
				 *  This is useful in occasions like the effect is a post effect without materials, the value of custom
				 *  effect can be assigned by scene rendering scripts.
				 */
				EPUSAGE_Unknown=0,
				
				EPUSAGE_MtrlC4_Ambient,				/** mc4_ambient */
				EPUSAGE_MtrlC4_Diffuse,				/** mc4_diffuse */
				EPUSAGE_MtrlC4_Emissive,			/** mc4_emissive */
				EPUSAGE_MtrlC4_Specular,			/** mc4_specular */
				EPUSAGE_MtrlC_Power,				/** mc_power */
				
				
				EPUSAGE_Trans_WorldViewProj = 100,		/** tr_worldviewproj */
				EPUSAGE_Trans_WorldViewOriProj,			/** tr_worldvieworiproj */
				EPUSAGE_Trans_World,					/** tr_world */
				EPUSAGE_Trans_WorldView,				/** tr_worldview */
				EPUSAGE_Trans_View,						/** tr_view */
				EPUSAGE_Trans_ViewProj,					/** tr_viewproj */
				EPUSAGE_Trans_Projection,				/** tr_proj */
				EPUSAGE_Trans_InvProj,					/** tr_invproj */
				EPUSAGE_Trans_InvView,					/** tr_invview */
				EPUSAGE_Trans_InvViewProj,				/** tr_invviewproj */

				/**
				 * tr_instanceworld
				 *  This parameter is supposed to be a 4x4 matrix array in the shader with a length of
				 *  InstancingData::MaxOneTimeInstances
				 */
				EPUSAGE_Trans_InstanceWorlds,
				

				EPUSAGE_M4X3_BoneTrans,			/** m4x3_bonestransform */
				EPUSAGE_M4X4_BoneTrans,			/** m4x4_bonestransform */

				EPUSAGE_V3_CameraX,				/** v3_camerax */
				EPUSAGE_V3_CameraY,				/** v3_cameray */
				EPUSAGE_V3_CameraZ,				/** v3_cameraz */
				EPUSAGE_V3_CameraPos,			/** v3_camerapos */


				EPUSAGE_Tex0 = 200,				/** tex_0 */
				EPUSAGE_Tex1,
				EPUSAGE_Tex2,
				EPUSAGE_Tex3,
				EPUSAGE_Tex4,
				EPUSAGE_Tex5,
				EPUSAGE_Tex6,
				EPUSAGE_Tex7,
				EPUSAGE_Tex8,
				EPUSAGE_Tex9,
				EPUSAGE_Tex10,
				EPUSAGE_Tex11,
				EPUSAGE_Tex12,
				EPUSAGE_Tex13,
				EPUSAGE_Tex14,
				EPUSAGE_Tex15,
				EPUSAGE_Tex16,
				EPUSAGE_DefaultTexture,				/** tex_default */

				EPUSAGE_LV3_LightDir = 500,			/** lv3_lightDir */
				EPUSAGE_LV3_LightPos,				/** lv3_lightPos */
				EPUSAGE_LC4_Ambient,				/** lc4_ambient */
				EPUSAGE_LC4_Diffuse,				/** lc4_diffuse */
				EPUSAGE_LC4_Specular,				/** lc4_specular */

				EPUSAGE_PV3_ViewPos = 1000,			/** pv3_viewPos */
				EPUSAGE_SV2_ViewportSize,			/** sv2_viewportSize */
				EPUSAGE_SV2_InvViewportSize,		/**  sv2_invviewportSize */

				/**
				 *  s_unifiedTime. A time from 0 to 5pi will be assigned to the parameter
				 */
				EPUSAGE_S_UnifiedTime,
				EPUSAGE_S_FarPlane,				/** s_farPlane */
				EPUSAGE_S_NearPlane,			/** s_nearPlane  */

				/**
				 *  The engine will get the value for the parameter from UserData pointer 
				 *  which is expected pointing to a InstanceInfoBlob struct.
				 *  The value will be obtained from the list inside the struct.
				*/
				EPUSAGE_InstanceBlob,

				/** 
				 *  The engine will look up the entry(MaterialCustomParameter)
				 *	with the same name from material's CustomParamTable in render operation.
				 *  Custom material param need to be specified in a String format in EffectParameter 
				 *  as CustomMaterialParamName. 
				 * 
				 *  Internally, parameters using automatic(typical usages) will be manipulated 
				 *  faster than this.
				*/
				EPUSAGE_CustomMaterialParam
			};

			/**
			 *  Includes all global scene render resources such as the current camera, lighting that could 
			 *  be used in effects.
			 */
			namespace RendererEffectParams
			{
				extern Camera* CurrentCamera;
				extern Vector3 LightDirection;
				extern Vector3 LightPosition;
				extern Color4 LightAmbient;
				extern Color4 LightDiffuse;
				extern Color4 LightSpecular;

				inline void Reset()
				{
					CurrentCamera = nullptr;
				}
			};


			enum CustomEffectParameterType
			{
				CEPT_Float = 0,
				CEPT_Vector2,
				CEPT_Vector4,
				CEPT_Matrix,
				CEPT_Boolean,
				CEPT_Integer,
				CEPT_Ref_Vector2,
				CEPT_Ref_Vector3,
				CEPT_Ref_Vector4,
				CEPT_Ref_Matrix,
				CEPT_Ref_Texture,
				CEPT_Ref_TextureHandle,
			};


			inline bool CustomEffectParameterType_IsReference(CustomEffectParameterType t)
			{
				return t == CEPT_Ref_Texture || t == CEPT_Ref_TextureHandle ||
					t == CEPT_Ref_Vector2 || t == CEPT_Ref_Vector3 || t == CEPT_Ref_Vector4 || t == CEPT_Ref_Matrix;
			}


			/** 
			 *  Defines a parameter in an effect containing effect param mapping info.
			 * 
			 *  CustomMaterialParamName and InstanceBlobIndex will be used based on the Usage of the EffectParameter. 
			 *  See EPUSAGE_CustomMaterialParam for details.
			 *
			 *  When EPUSAGE_InstanceBlob or EPUSAGE_CustomMaterialParam is unused in effect, 
			 *  no more attention from application developer is needed on setting the effect parameter.
			 *
			 *  The parameter's usage is configured by the effect parameter listing file along with the shader code,
			 *  when building resources using APBuild or APDesigner.
			 */
			class APAPI EffectParameter
			{	
			public:
				String Name;
				EffectParamUsage Usage = EPUSAGE_Unknown;

				String CustomMaterialParamName;
				int32 InstanceBlobIndex = -1;

				ShaderType ProgramType = ShaderType::Vertex;

				int32 RegisterIndex = -1;
				
				int32 SamplerIndex = -1;
				ShaderSamplerState SamplerState;

				String SamplerStateOverridenGroupName;
				String DefaultTextureName;

				EffectParameter() { }
				EffectParameter(const String& name);
				~EffectParameter();

				void Read(BinaryReader* br);
				void Write(BinaryWriter* bw);

				static EffectParamUsage ParseParamUsage(const String& val);
				static bool SupportsParamUsage(const String& val);

				static String ToString(EffectParamUsage usage);
				static void FillParameterUsageNames(List<String>& results);

			private:

			};




			struct InstanceInfoBlobValue
			{
				/** The data type of the value. */
				CustomEffectParameterType Type;
				uint Value[16];
				void* RefValue = nullptr;

				InstanceInfoBlobValue()
				{ }

				InstanceInfoBlobValue(float val) : Type(CEPT_Float) { AsSingle() = val; }
				InstanceInfoBlobValue(const Vector2& val) : Type(CEPT_Vector2) { AsVector2() = val; }
				InstanceInfoBlobValue(const Vector4& val) : Type(CEPT_Vector4) { AsVector4() = val; }
				InstanceInfoBlobValue(const Matrix& val) : Type(CEPT_Matrix) { AsMatrix() = val; }

				InstanceInfoBlobValue(bool val) : Type(CEPT_Boolean) { AsBoolean() = val; }
				InstanceInfoBlobValue(int val) : Type(CEPT_Integer) { AsInteger() = val; }

				InstanceInfoBlobValue(const Vector2* val) : Type(CEPT_Ref_Vector2) { AsVector2Ref() = val; }
				InstanceInfoBlobValue(const Vector3* val) : Type(CEPT_Ref_Vector3) { AsVector3Ref() = val; }
				InstanceInfoBlobValue(const Vector4* val) : Type(CEPT_Ref_Vector4) { AsVector4Ref() = val; }

				InstanceInfoBlobValue(Texture* val) : Type(CEPT_Ref_Texture) { RefValue = val; }
				InstanceInfoBlobValue(ResourceHandle<Texture>* val) : Type(CEPT_Ref_TextureHandle) { RefValue = val; }

				//InstanceInfoBlobValue(CustomEffectParameterType type) : Type(type) { }

				const float& AsSingle() const  { assert(Type == CEPT_Float); return reinterpret_cast<const float&>(Value); }
				const Vector2& AsVector2() const { assert(Type == CEPT_Vector2); return reinterpret_cast<const Vector2&>(Value); }
				const Vector4& AsVector4() const { assert(Type == CEPT_Vector4); return reinterpret_cast<const Vector4&>(Value); }
				const Matrix& AsMatrix() const  { assert(Type == CEPT_Matrix); return reinterpret_cast<const Matrix&>(Value); }
				const bool& AsBoolean() const { assert(Type == CEPT_Boolean); return reinterpret_cast<const bool&>(Value); }
				const int& AsInteger() const { assert(Type == CEPT_Integer); return reinterpret_cast<const int&>(Value); }


				float& AsSingle() { assert(Type == CEPT_Float); return reinterpret_cast<float&>(Value); }
				Vector2& AsVector2() { assert(Type == CEPT_Vector2); return reinterpret_cast<Vector2&>(Value); }
				Vector4& AsVector4() { assert(Type == CEPT_Vector4); return reinterpret_cast<Vector4&>(Value); }
				Matrix& AsMatrix() { assert(Type == CEPT_Matrix); return reinterpret_cast<Matrix&>(Value); }
				bool& AsBoolean() { assert(Type == CEPT_Boolean); return reinterpret_cast<bool&>(Value); }
				int& AsInteger() { assert(Type == CEPT_Integer); return reinterpret_cast<int&>(Value); }

				const Vector2 *& AsVector2Ref() { assert(Type == CEPT_Ref_Vector2); return const_cast<const Vector2*&>( reinterpret_cast<Vector2 *&>(RefValue) ); }
				const Vector3 *& AsVector3Ref() { assert(Type == CEPT_Ref_Vector3); return const_cast<const Vector3*&>(reinterpret_cast<Vector3 *&>(RefValue)); }
				const Vector4 *& AsVector4Ref() { assert(Type == CEPT_Ref_Vector4); return const_cast<const Vector4*&>(reinterpret_cast<Vector4 *&>(RefValue)); }
				const Matrix *& AsMatrixRef() { assert(Type == CEPT_Ref_Matrix); return const_cast<const Matrix*&>(reinterpret_cast<Matrix *&>(RefValue)); }


				//InstanceInfoBlobValue& operator=(float v) { AsSingle() = v; return *this; }
				//InstanceInfoBlobValue& operator=(const Vector2& v) { AsVector2() = v; return *this; }
				//InstanceInfoBlobValue& operator=(const Vector4& v) { AsVector4() = v; return *this; }
				//InstanceInfoBlobValue& operator=(const Matrix& v) { AsMatrix() = v; return *this; }
				//InstanceInfoBlobValue& operator=(bool v) { AsBoolean() = v; return *this; }
				//InstanceInfoBlobValue& operator=(int v) { AsInteger() = v; return *this; }

				//InstanceInfoBlobValue& operator=(const Vector2* v) { AsVector2Ref() = v; return *this; }
				//InstanceInfoBlobValue& operator=(const Vector3* v) { AsVector3Ref() = v; return *this; }
				//InstanceInfoBlobValue& operator=(const Vector4* v) { AsVector4Ref() = v; return *this; }

				//InstanceInfoBlobValue& operator=(Texture* v) { assert(Type == CEPT_Ref_Texture); RefValue = v; return *this; }
				//InstanceInfoBlobValue& operator=(ResourceHandle<Texture>* v) { assert(Type == CEPT_Ref_TextureHandle);  RefValue = v; return *this; }


				void Configure(float defVal) { RefValue = nullptr; Type = CEPT_Float; AsSingle() = defVal; }
				void Configure(const Vector2& defVal) { RefValue = nullptr; Type = CEPT_Vector2; AsVector2() = defVal; }
				void Configure(const Vector4& defVal) { RefValue = nullptr; Type = CEPT_Vector4; AsVector4() = defVal; }
				void Configure(const Matrix& defVal) { RefValue = nullptr; Type = CEPT_Matrix; AsMatrix() = defVal; }
				void Configure(bool defVal) { RefValue = nullptr; Type = CEPT_Boolean; AsBoolean() = defVal; }
				void Configure(int defVal) { RefValue = nullptr; Type = CEPT_Integer; AsInteger() = defVal; }

				void ConfigureRef(const Vector2* defVal) { Type = CEPT_Ref_Vector2; RefValue = (void*)defVal; }
				void ConfigureRef(const Vector3* defVal) { Type = CEPT_Ref_Vector3; RefValue = (void*)defVal; }
				void ConfigureRef(const Vector4* defVal) { Type = CEPT_Ref_Vector4; RefValue = (void*)defVal; }
				void ConfigureRef(Texture* defVal) { Type = CEPT_Ref_Texture; RefValue = defVal; }
				void ConfigureRef(ResourceHandle<Texture>* defVal) { Type = CEPT_Ref_TextureHandle; RefValue = defVal; }
			};

			typedef List<InstanceInfoBlobValue> InstanceInfoBlob;



			class InstanceInfoBlob2 final
			{
			public:
				struct ValueProxy
				{
					ValueProxy() { }
					ValueProxy(void* addr, CustomEffectParameterType type)
						: m_address(addr), m_type(type) { }

					ValueProxy& operator=(float v) { AsSingle() = v; return *this; }
					ValueProxy& operator=(const Vector2& v) { AsVector2() = v; return *this; }
					ValueProxy& operator=(const Vector4& v) { AsVector4() = v; return *this; }
					ValueProxy& operator=(const Matrix& v) { AsMatrix() = v; return *this; }
					ValueProxy& operator=(bool v) { AsBoolean() = v; return *this; }
					ValueProxy& operator=(int v) { AsInteger() = v; return *this; }

					ValueProxy& operator=(const Vector2* v) { AsVector2Ref() = v; return *this; }
					ValueProxy& operator=(const Vector3* v) { AsVector3Ref() = v; return *this; }
					ValueProxy& operator=(const Vector4* v) { AsVector4Ref() = v; return *this; }

					ValueProxy& operator=(Texture* v) { AsTextureRef() = v; return *this; }
					ValueProxy& operator=(ResourceHandle<Texture>* v) { AsTextureHandleRef() = v; return *this; }
					ValueProxy& operator=(const InstanceInfoBlobValue& o);

					CustomEffectParameterType getType() const { return m_type; }
				private:

					float& AsSingle() { assert(m_type == CEPT_Float); return *reinterpret_cast<float*>(m_address); }
					Vector2& AsVector2() { assert(m_type == CEPT_Vector2); return *reinterpret_cast<Vector2*>(m_address); }
					Vector4& AsVector4() { assert(m_type == CEPT_Vector4); return *reinterpret_cast<Vector4*>(m_address); }
					Matrix& AsMatrix() { assert(m_type == CEPT_Matrix); return *reinterpret_cast<Matrix*>(m_address); }
					bool& AsBoolean() { assert(m_type == CEPT_Boolean); return *reinterpret_cast<bool*>(m_address); }
					int& AsInteger() { assert(m_type == CEPT_Integer); return *reinterpret_cast<int*>(m_address); }

					const Vector2 *& AsVector2Ref() { assert(m_type == CEPT_Ref_Vector2); return const_cast<const Vector2*&>(reinterpret_cast<Vector2 *&>(m_address)); }
					const Vector3 *& AsVector3Ref() { assert(m_type == CEPT_Ref_Vector3); return const_cast<const Vector3*&>(reinterpret_cast<Vector3 *&>(m_address)); }
					const Vector4 *& AsVector4Ref() { assert(m_type == CEPT_Ref_Vector4); return const_cast<const Vector4*&>(reinterpret_cast<Vector4 *&>(m_address)); }
					const Matrix *& AsMatrixRef() { assert(m_type == CEPT_Ref_Matrix); return const_cast<const Matrix*&>(reinterpret_cast<Matrix *&>(m_address)); }

					Texture *& AsTextureRef() { assert(m_type == CEPT_Ref_Texture); return reinterpret_cast<Texture *&>(m_address); }
					ResourceHandle<Texture> *& AsTextureHandleRef() { assert(m_type == CEPT_Ref_TextureHandle); return reinterpret_cast<ResourceHandle<Texture> *&>(m_address); }

					CustomEffectParameterType m_type;

					void* m_address;
				};

				InstanceInfoBlob2(std::initializer_list<InstanceInfoBlobValue> layout);

				~InstanceInfoBlob2();

				InstanceInfoBlob2(const InstanceInfoBlob2& o);
				InstanceInfoBlob2(InstanceInfoBlob2&& o);

				InstanceInfoBlob2& operator=(const InstanceInfoBlob2& o);
				InstanceInfoBlob2& operator=(InstanceInfoBlob2&& o);


				ValueProxy operator[](int32 idx) const 
				{
					assert(idx >= 0 && idx < m_count);
					return idx < m_locallyStoredCount ? m_localItemInfo[idx] : m_allocatedItemInfo[idx - m_locallyStoredCount];
				}

				
			private:
				static const int32 LocalSizeLimit = 128;
				static const int32 LocalCountLimit = 16;

				int32 m_count;
				int32 m_locallyStoredCount;

				char m_localStorage[LocalSizeLimit];
				ValueProxy m_localItemInfo[LocalCountLimit];

				char* m_allocatedStorage = nullptr;
				ValueProxy* m_allocatedItemInfo = nullptr;

			};

		};
	};
};
#endif
