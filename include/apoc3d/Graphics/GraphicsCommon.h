#pragma once
#ifndef APOC3D_GR_ENUMS_H
#define APOC3D_GR_ENUMS_H

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
#include "apoc3d/Utility/TypeConverter.h"

namespace Apoc3D
{
	namespace Graphics
	{
		namespace RenderSystem
		{
			const int32 MaxScenePass = 64;
			const int32 DefaultMaterialPriority = 10;

			enum TextureType
			{
				TT_Texture1D = 0,
				TT_Texture2D,
				TT_Texture3D,
				TT_CubeTexture
			};


			enum LockMode
			{
				LOCK_None = 0,
				LOCK_Discard = 1,
				LOCK_ReadOnly = 2,
				LOCK_NoOverwrite = 4,
			};

			enum BufferUsageFlags
			{
				// BU_None,
				// BU_Constant,
				// BU_Uploadable,    // optimized for CPU upload
				// BU_Downloadable   // allows CPU read

				BU_Static = 1,
				BU_Dynamic = 2,
				BU_WriteOnly = 4,
				BU_PointSpriteVertex = 8
			};

			enum IndexBufferType    
			{
				IBT_Bit16 = 0,
				IBT_Bit32 = 1
			};

			enum CullMode
			{
				CULL_None = 0,				/** Do not cull back faces. */
				CULL_Clockwise = 1,			/** Cull back faces with clockwise vertices. */
				CULL_CounterClockwise = 2,	/** Cull back faces with counterclockwise vertices. */
				CULL_Count = 3
			};
			enum FillMode
			{
				FILL_Point = 0,			/** Draw a point at each vertex. */
				FILL_WireFrame = 1,		/** Draw lines connecting the vertices that define a primitive face. */
				FILL_Solid = 2,			/** Draw solid faces for each primitive. */
				FILL_Count = 3
			};
			
			enum PrimitiveType
			{
				PT_PointList = 0,		/** Render the vertices as individual points. */
				PT_LineList = 1,		/** Render the vertices as a series of individual lines. */
				PT_LineStrip = 2,		/** Render the vertices as a continuous line. */
				PT_TriangleList = 3,	/** Render the vertices as a series of individual triangles. */
				PT_TriangleStrip = 4,	/** Render the vertices as a continous set of triangles in a zigzag type fashion. */
				PT_TriangleFan = 5,		/** Render the vertices as a set of trinagles in a fan like formation. */
				PT_Count = 6
			};
			
			
			enum ClearFlags
			{
				CLEAR_ColorBuffer = 1,
				CLEAR_DepthBuffer = 2,
				CLEAR_Stencil = 4,
				CLEAR_ColorAndDepth = CLEAR_ColorBuffer | CLEAR_DepthBuffer,
				CLEAR_All = CLEAR_ColorBuffer | CLEAR_DepthBuffer | CLEAR_Stencil
			};

			/** Defines stencil buffer operations. */
			enum struct StencilOperation
			{
				Keep = 0,		/** Does not update the stencil-buffer entry. This is the default value. */
				Zero = 1,		/** Sets the stencil-buffer entry to 0. */

				Replace = 2,				/** Replaces the stencil-buffer entry with a reference value. */
				IncrementSaturation = 3,	/** Increments the stencil-buffer entry, clamping to the maximum value. */
				DecrementSaturation = 4,	/** Decrements the stencil-buffer entry, clamping to 0. */

				Invert = 5,		/** Inverts the bits in the stencil-buffer entry. */

				/**
				 *  Increments the stencil-buffer entry, wrapping to 0 if the new value exceeds
				 *  the maximum value.
				 */
				Increment = 6,
				
				/**
				 *  Decrements the stencil-buffer entry, wrapping to the maximum value if the
				 *  new value is less than 0.
				 */
				Decrement = 7,
				Count = 8
			};

			/** Defines constants that describe supported texture-addressing modes. */
			enum struct TextureAddressMode
			{
				/** Tile the texture at every integer junction. For example, for u values between
					0 and 3, the texture is repeated three times; no mirroring is performed.
				*/
				Wrap = 0,

				/** Similar to Wrap, except that the texture is flipped at every integer junction.
					For u values between 0 and 1, for example, the texture is addressed normally;
					between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the
					texture is normal again, and so on.
				*/
				Mirror = 1,

				/** Texture coordinates outside the range [0.0, 1.0] are set to the texture color
					at 0.0 or 1.0, respectively.
				*/
				Clamp = 2,        
				
				/** Texture coordinates outside the range [0.0, 1.0] are set to the border color.
				*/
				Border = 3,

				/** Similar to Mirror and Clamp. Takes the absolute value of the texture coordinate
					(thus, mirroring around 0), and then clamps to the maximum value. The most
					common usage is for volume textures, where support for the full MirrorOnce
					texture-addressing mode is not necessary, but the data is symmetrical around
					the one axis.
				*/
				MirrorOnce = 4,
				Count = 5
			};

			/**
			 *  Defines how a texture will be filtered as it is minified for each mipmap level.
			 */
			enum TextureFilter
			{
				/* Mipmapping disabled. The rasterizer uses the magnification filter instead.
				*/
				TFLT_None = 0,

				/*  Point filtering used as a texture magnification or minification filter. The
				*   texel with coordinates nearest to the desired pixel value is used. The texture
				*    filter used between mipmap levels is based on the nearest point; that is,
				*    the rasterizer uses the color from the texel of the nearest mipmap texture.
				*/
				TFLT_Point = 1,

				/*  Bilinear interpolation filtering used as a texture magnification or minification
				*   filter. A weighted average of a 2×2 area of texels surrounding the desired
				*    pixel is used. The texture filter used between mipmap levels is trilinear
				*    mipmap interpolation, in which the rasterizer performs linear interpolation
				*    on pixel color, using the texels of the two nearest mipmap textures.
				*/
				TFLT_Linear = 2,

				/*  Anisotropic texture filtering used as a texture magnification or minification
				*   filter. This type of filtering compensates for distortion caused by the difference
				*    in angle between the texture polygon and the plane of the screen.
				*/
				TFLT_Anisotropic = 3,
				
				TFLT_Count = 4
			};


			/** Defines format of vertex element */
			enum VertexElementFormat
			{	
				VEF_Single = 0,		/** One-component, float expanded to (float, 0, 0, 1) */
				VEF_Vector2 = 1,	/** Two-component, float expanded to (float, float, 0, 1) */
				VEF_Vector3 = 2,	/** Three-component, float expanded to (float, float, float, 1) */
				VEF_Vector4 = 3,	/** Four-component, float expanded to (float, float, float, float) */
				VEF_Color = 4,		/** Four-component, packed byte to (R, G, B, A) */

				VEF_Byte4 = 5,		/** Four-component, byte. */
				VEF_Short2 = 6,		/** Two-component, signed short expanded to (value, value, 0, 1). */
				VEF_Short4 = 7,		/** Four-component, signed short expanded to (value, value, value, value). */
				
				/**
				 *  Four-component byte with each byte normalized by dividing the component with
				 *  255.0f. This type is valid for vertex shader version 2.0 or higher.
				 */
				VEF_NormalizedByte4 = 8,

				/**
				 *  Normalized, two-component, signed short, expanded to (first short/32767.0,
				 *  second short/32767.0, 0, 1). This type is valid for vertex shader version
				 *  2.0 or higher.
				 */
				VEF_NormalizedShort2 = 9,
				/*
				 *  Normalized, four-component, signed short, expanded to (first short/32767.0,
				 *   second short/32767.0, third short/32767.0, fourth short/32767.0). This type
				 *   is valid for vertex shader version 2.0 or higher.
				 */
				VEF_NormalizedShort4 = 10,

				/**
				 *  Three-component, unsigned, 10 10 10 format expanded to (value, value, value, 1).
				 */
				VEF_UInt101010 = 11,
				/**
				 *  Three-component, signed, 10 10 10 format normalized and expanded to (v[0]/511.0,
				 *   v[1]/511.0, v[2]/511.0, 1).
				 */
				VEF_Normalized101010 = 12,
				/** 
				 *  Two-component, 16-bit floating point expanded to (value, value, value, value).
				 *   This type is valid for vertex shader version 2.0 or higher.
				 */
				VEF_HalfVector2 = 13,
				/**
				 *   Four-component, 16-bit floating-point expanded to (value, value, value, value).
				 *   This type is valid for vertex shader version 2.0 or higher.
				 */
				VEF_HalfVector4 = 14,
				
				VEF_Count = 15,
			};


			enum VertexElementUsage
			{
				VEU_Position = 0,
				VEU_BlendWeight = 1,
				VEU_BlendIndices = 2,
				VEU_Normal = 3,
				VEU_PointSize = 4,
				VEU_TextureCoordinate = 5,
				VEU_Tangent = 6,
				VEU_Binormal = 7,
				VEU_TessellateFactor = 8,
				VEU_PositionTransformed = 9,
				VEU_Color = 10,
				VEU_Fog = 11,
				VEU_Depth = 12,
				VEU_Sample = 13,
				VEU_Count = 14
			};
			
			/** Specifies how a texture is to be used in the engine. */
			enum TextureUsage
			{
				TU_Static = BU_Static,
				TU_Dynamic = BU_Dynamic,
				/* Mipmaps will be automatically generated for this texture */
				TU_AutoMipMap = 0x100,
				
				/* Default to static textures */
				TU_Default = TU_Static,
			};

			enum struct CompareFunction
			{
				Never = 1,		/** Always fail */
				Less = 2,		/** Pass if the determined pixel is less than current pixel. */
				Equal = 3,		/** Pass if the determined pixel is equal to current pixel. */
				LessEqual = 4,	/** Pass if the determined pixel is equal or less than current pixel. */

				Greater = 5,			/** Pass if the determined pixel is greater than current pixel. */
				NotEqual = 6,		/** Pass if the determined pixel is not equal to current pixel. */

				GreaterEqual = 7,	/** Pass if the determined pixel is equal or greater than current pixel. */

				Always = 8,			/** Always pass */
				Count = 9
			};
			enum struct BlendFunction
			{				
				/** The result is the destination added to the source.Result = (Source Color
					* Source Blend) + (Destination Color * Destination Blend)
				*/
				Add = 0,

				/** The result is the destination subtracted from the source.Result = (Source
					Color * Source Blend) − (Destination Color * Destination Blend)
				*/
				Subtract = 1,

				/** The result is the source subtracted from the destination.Result = (Destination
					Color * Destination Blend) −(Source Color * Source Blend)
				*/
				ReverseSubtract = 2,

				/** The result is the minimum of the source and destination.Result = min( (Source
					 Color * Source Blend), (Destination Color * Destination Blend) )
				*/
				Min = 3,

				/** The result is the maximum of the source and destination.Result = max( (Source
					Color * Source Blend), (Destination Color * Destination Blend) )
				*/
				Max = 4,
				Count = 5
			};

			enum struct Blend
			{
				Zero = 0,				/** Each component of the color is multiplied by 0 */
				One = 1,				/** Each component of the color is multiplied by 1 */
				SourceColor = 2,		/** Each component color is multiplied by the source color. */

				/** Each component of the color is multiplied by the inverse of the source color.
					This can be represented as (1 − Rs, 1 − Gs, 1 − Bs, 1 − As) where R, G, B,
					and A respectively stand for the red, green, blue, and alpha destination
					values.
				*/
				InverseSourceColor = 3,
				
				/** Each component of the color is multiplied by the alpha value of the source.
					This can be represented as (As, As, As, As), where As is the alpha source
					value.
				*/
				SourceAlpha = 4,

				/** Each component of the color is multiplied by the inverse of the alpha value
					of the source. This can be represented as (1 − As, 1 − As, 1 − As, 1 − As),
					where As is the alpha destination value.
				*/
				InverseSourceAlpha = 5,

				
				/** Each component of the color is multiplied by the alpha value of the destination.
					This can be represented as (Ad, Ad, Ad, Ad), where Ad is the destination
					alpha value.
				*/
				DestinationAlpha = 6,
				
				/** Each component of the color is multiplied by the inverse of the alpha value
					of the destination. This can be represented as (1 − Ad, 1 − Ad, 1 − Ad, 1
					− Ad), where Ad is the alpha destination value.
				*/
				InverseDestinationAlpha = 7,

				/** Each component color is multiplied by the destination color. This can be
					represented as (Rd, Gd, Bd, Ad), where R, G, B, and A respectively stand
					for red, green, blue, and alpha destination values.
				*/
				DestinationColor = 8,

				/** Each component of the color is multiplied by the inverse of the destination
					color. This can be represented as (1 − Rd, 1 − Gd, 1 − Bd, 1 − Ad), where
					Rd, Gd, Bd, and Ad respectively stand for the red, green, blue, and alpha
					destination values.
				*/
				InverseDestinationColor = 9,

				/** Each component of the color is multiplied by either the alpha of the source
					color, or the inverse of the alpha of the source color, whichever is greater.
					This can be represented as (f, f, f, 1), where f = min(A, 1 − Ad).
				*/
				SourceAlphaSaturation = 10,

				/**  Each component of the color is multiplied by BlendFactor. */
				BlendFactor = 11,
				Count = 12
			};
			enum struct ShaderType
			{
				Vertex = 1 << 0,
				Pixel = 1 << 1,
				Geometry = 1 << 2,
				All = Pixel | Vertex | Geometry
			};

			APAPI extern const Utility::TypeDualConverter<Blend> BlendConverter;
			APAPI extern const Utility::TypeDualConverter<BlendFunction> BlendFunctionConverter;
			APAPI extern const Utility::TypeDualConverter<CullMode> CullModeConverter;
			APAPI extern const Utility::TypeDualConverter<TextureAddressMode> TextureAddressModeConverter;
			APAPI extern const Utility::TypeDualConverter<TextureFilter> TextureFilterConverter;
			APAPI extern const Utility::TypeDualConverter<ShaderType> ShaderTypeConverter;
			APAPI extern const Utility::TypeDualConverter<VertexElementUsage> VertexElementUsageConverter;

		}
	}
}
#endif