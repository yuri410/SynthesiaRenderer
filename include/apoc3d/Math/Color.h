#pragma once
#ifndef APOC3D_COLOR_H
#define APOC3D_COLOR_H

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

#include "Vector.h"
#include "ColorValue.h"

namespace Apoc3D
{
	namespace Math
	{
		/** A four component color represented using 4 floats from 0 to 1 */
		class Color4
		{
		public:
			float Red = 0;				/** the color's red component. */
			float Green = 0;			/** the color's green component. */
			float Blue = 0;				/** the color's blue component. */
			float Alpha = 0;			/** the color's alpha component. */

			Color4() { }

			explicit Color4(const Vector3& color)
				: Red(color.X), Green(color.Y), Blue(color.Z), Alpha(1) { }
			
			explicit Color4(ColorValue argb)
			{
				Alpha = static_cast<float>(((argb >> 24) & 0xff) / 255.0f);
				Red = static_cast<float>((float)((argb >> 16) & 0xff) / 255.0f);
				Green = static_cast<float>(((argb >> 8) & 0xff) / 255.0f);
				Blue = static_cast<float>((argb & 0xff) / 255.0f);
			}
			
			Color4(int red, int green, int blue, int alpha)
			{
				Alpha = static_cast<float>(alpha / 255.0f);
				Red = static_cast<float>(red / 255.0f);
				Green = static_cast<float>(green / 255.0f);
				Blue = static_cast<float>(blue / 255.0f);
			}

			Color4(int red, int green, int blue)
				: Alpha(1.0f)
			{
				Red = static_cast<float>(red / 255.0f);
				Green = static_cast<float>(green / 255.0f);
				Blue = static_cast<float>(blue / 255.0f);
			}

			Color4(float red, float green, float blue)
				: Alpha(1.0f), Red(red), Green(green), Blue(blue)
			{ }

			Color4(float red, float green, float blue, float alpha)
				: Alpha(alpha), Red(red), Green(green), Blue(blue)
			{ }

			/** Converts the color into a packed integer. */
			ColorValue ToArgb() const
			{
				uint a = static_cast<uint>(Alpha * 255.0f + 0.5f);// Math::UnifiedToByteRange(Alpha);
				uint r = static_cast<uint>(Red * 255.0f + 0.5f);// Math::UnifiedToByteRange(Red);
				uint g = static_cast<uint>(Green * 255.0f + 0.5f);// Math::UnifiedToByteRange(Green);
				uint b = static_cast<uint>(Blue * 255.0f + 0.5f);// Math::UnifiedToByteRange(Blue);
				uint result = b;
				result += g << 8;
				result += r << 16;
				result += a << 24;
				return result;
			}

			/** Converts the color into a vector. */
			Vector3 ToVector3() const
			{
				return Vector3(Red, Green, Blue);
			}
			Vector4 ToVector4() const
			{
				return Vector4(Red, Green, Blue, Alpha);
			}

			void ClampInPlace();

			bool operator==(const Color4 &other) const
			{
				return other.Red == Red && other.Green == Green && other.Blue == Blue && other.Alpha == Alpha; 
			}
			bool operator!=(const Color4 &other) const { return !(*this == other); }

			/** Adds two colors. */
			static void Add(Color4& result, const Color4 &color1, const Color4 &color2)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				result = Color4(SIMDVecAdd(color1.vector, color2.vector));
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				result = Color4(color1.Red + color2.Red, 
					color1.Green + color2.Green, 
					color1.Blue + color2.Blue, 
					color1.Alpha + color2.Alpha);
			#endif
			}

			/** Adds two colors. */
			static Color4 Add(const Color4 &color1, const Color4 &color2)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				return Color4(SIMDVecAdd(color1.vector, color2.vector));
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				return Color4(color1.Red + color2.Red, 
					color1.Green + color2.Green, 
					color1.Blue + color2.Blue, 
					color1.Alpha + color2.Alpha);
			#endif
			}

			
			/** Subtracts two colors. */
			static Color4 Subtract(const Color4 &color1, const Color4 &color2)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				return Color4(SIMDVecSub(color1.vector, color2.vector));
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				return Color4(color1.Red - color2.Red, 
					color1.Green - color2.Green, 
					color1.Blue - color2.Blue, 
					color1.Alpha - color2.Alpha);
			#endif
			}

			/** Subtracts two colors. */
			static void Subtract(Color4& result, const Color4 &color1, const Color4 &color2)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				result = Color4(SIMDVecSub(color1.vector, color2.vector));
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				result = Color4(color1.Red - color2.Red, 
					color1.Green - color2.Green, 
					color1.Blue - color2.Blue, 
					color1.Alpha - color2.Alpha);
			#endif
			}

			/** Modulates two colors. */
			static void Modulate(Color4& result,  const Color4 &color1, const Color4 &color2)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				result = SIMDVecMul(color1.vector, color2.vector);
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				result = Color4(color1.Red * color2.Red, 
					color1.Green * color2.Green, 
					color1.Blue * color2.Blue, 
					color1.Alpha * color2.Alpha);
			#endif
			}

			/** Modulates two colors. */
			static Color4 Modulate(const Color4 &color1, const Color4 &color2)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				return SIMDVecMul(color1.vector, color2.vector);
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				return Color4(color1.Red * color2.Red, 
					color1.Green * color2.Green, 
					color1.Blue * color2.Blue, 
					color1.Alpha * color2.Alpha);
			#endif
			}

			/** Performs a linear interpolation between two colors. */
			static Color4 Lerp(const Color4 &color1, const Color4 &color2, float amount)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				Vector3 b = SIMDVecSub(color2.vector, color1.vector);
				b = SIMDVecMul(b, amount);
				b = SIMDVecAdd(color1.vector, b);
				return b;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				float alpha = color1.Alpha + (amount * (color2.Alpha - color1.Alpha));
				float red = color1.Red + (amount * (color2.Red - color1.Red));
				float green = color1.Green + (amount * (color2.Green - color1.Green));
				float blue = color1.Blue + (amount * (color2.Blue - color1.Blue));
				return Color4(red, green, blue, alpha);
			#endif
			}

			/** Performs a linear interpolation between two colors. */
			static void Lerp(Color4& result, const Color4 &color1, const Color4 &color2, float amount)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				Vector3 b = SIMDVecSub(color2.vector, color1.vector);
				b = SIMDVecMul(b, amount);
				b = SIMDVecAdd(color1.vector, b);
				result = Color4(b);
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				float alpha = color1.Alpha + (amount * (color2.Alpha - color1.Alpha));
				float red = color1.Red + (amount * (color2.Red - color1.Red));
				float green = color1.Green + (amount * (color2.Green - color1.Green));
				float blue = color1.Blue + (amount * (color2.Blue - color1.Blue));
				result = Color4(red, green, blue, alpha);
			#endif
			}

			/** Negates a color. */
			static void Negate(Color4& result, const Color4 &color)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				static const Vector3 one = SIMDVecLoad(1);
				result = Color4(SIMDVecSub(one, color.vector));
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				result = Color4(1.0f - color.Red, 1.0f - color.Green, 1.0f - color.Blue, 1.0f - color.Alpha);
			#endif
			}
			
			/** Scales a color by the specified amount. */
			static void Scale(Color4& result,  const Color4 &color, float scale)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				result = Color4(SIMDVecMul(color.vector, scale));
				result.Alpha = color.Alpha;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				result = Color4(color.Red * scale, color.Green * scale, color.Blue * scale, color.Alpha);
			#endif
			}

			/** Scales a color by the specified amount. */
			static Color4 Scale(const Color4 &color, float scale)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				Color4 result = Color4(SIMDVecMul(color.vector, scale));
				result.Alpha = color.Alpha;
				return result;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				return Color4(color.Red * scale, color.Green * scale, color.Blue * scale, color.Alpha);
			#endif
			}

			/** Adjusts the contrast of a color. */
			static void AdjustContrast(Color4& result, const Color4 &color, float contrast)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				static const Vector3 half = SIMDVecLoad(0.5f);

				Vector3 t = SIMDVecSub(color.vector, half);
				t = SIMDVecMul(t, contrast);
				t = SIMDVecAdd(t, half);

				result = Color4(t);
				result.Alpha = color.Alpha;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				float r = 0.5f + contrast * (color.Red - 0.5f);
				float g = 0.5f + contrast * (color.Green - 0.5f);
				float b = 0.5f + contrast * (color.Blue - 0.5f);

				result = Color4(r, g, b, color.Alpha);
			#endif
			}

			/** Adjusts the contrast of a color. */
			static Color4 AdjustContrast(const Color4 &color, float contrast)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				static const Vector3 half = SIMDVecLoad(0.5f);

				Vector3 t = SIMDVecSub(color.vector, half);
				t = SIMDVecMul(t, contrast);
				t = SIMDVecAdd(t, half);

				Color4 result = Color4(t);
				result.Alpha = color.Alpha;
				return result;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				float r = 0.5f + contrast * (color.Red - 0.5f);
				float g = 0.5f + contrast * (color.Green - 0.5f);
				float b = 0.5f + contrast * (color.Blue - 0.5f);

				return Color4(r, g, b, color.Alpha);
			#endif
			}

			/** Adjusts the saturation of a color. */
			static void AdjustSaturation(Color4& result, const Color4 &color, float saturation)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				static const SSEVecLoader buffer = {0.2125f, 0.7154f, 0.0721f, 0};
				static const Vector3 hue = SIMDVecLoad(buffer);
			
				Vector3 grey = SIMDVec3Dot2(hue, color.vector);

				Vector3 c = SIMDVecSub(color.vector, grey);
				c = SIMDVecMul(c, saturation);
				c = SIMDVecAdd(c, grey);

				result = Color4(c);
				result.Alpha = color.Alpha;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				float grey = color.Red * 0.2125f + color.Green * 0.7154f + color.Blue * 0.0721f;
				float r = grey + saturation * (color.Red - grey);
				float g = grey + saturation * (color.Green - grey);
				float b = grey + saturation * (color.Blue - grey);

				result = Color4(r, g, b, color.Alpha);
			#endif
			}

			/** Adjusts the saturation of a color. */
			static Color4 AdjustSaturation(const Color4 &color, float saturation)
			{
			#if APOC3D_MATH_IMPL == APOC3D_SSE
				static const SSEVecLoader buffer = {0.2125f, 0.7154f, 0.0721f, 0};
				static const Vector3 hue = SIMDVecLoad(buffer);

				Vector3 grey = SIMDVec3Dot2(hue, color.vector);

				Vector3 c = SIMDVecSub(color.vector, grey);
				c = SIMDVecMul(c, saturation);
				c = SIMDVecAdd(c, grey);

				Color4 result = Color4(c);
				result.Alpha = color.Alpha;
				return result;
			#elif APOC3D_MATH_IMPL == APOC3D_DEFAULT
				float grey = color.Red * 0.2125f + color.Green * 0.7154f + color.Blue * 0.0721f;
				float r = grey + saturation * (color.Red - grey);
				float g = grey + saturation * (color.Green - grey);
				float b = grey + saturation * (color.Blue - grey);

				return Color4(r, g, b, color.Alpha);
			#endif
			}


		};

	}
}

#endif
