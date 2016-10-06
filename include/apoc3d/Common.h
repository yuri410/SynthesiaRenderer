#pragma once
#ifndef APOC3D_COMMON_H
#define APOC3D_COMMON_H

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


#define APOC3D_DEFAULT 0
#define APOC3D_SSE 1
#define APOC3D_MATH_IMPL APOC3D_DEFAULT

#define APOC3D_PLATFORM_WINDOWS 0
#define APOC3D_PLATFORM_MAC 1
#define APOC3D_PLATFORM_LINUX 2

#define APOC3D_PLATFORM_NAME L"unknown"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define APOC3D_PLATFORM APOC3D_PLATFORM_WINDOWS
#	undef APOC3D_PLATFORM_NAME
#	define APOC3D_PLATFORM_NAME L"windows"
#elif defined( __APPLE_CC__)
#   define APOC3D_PLATFORM APOC3D_PLATFORM_MAC
#	undef APOC3D_PLATFORM_NAME
#	define APOC3D_PLATFORM_NAME L"mac"
#else
#   define APOC3D_PLATFORM APOC3D_PLATFORM_LINUX
#	undef APOC3D_PLATFORM_NAME
#	define APOC3D_PLATFORM_NAME L"linux"
#endif

#include "ApocExportSettings.h"
#include "ApocMacros.h"

#ifndef CASSERT_INCLUDED
#define CASSERT_INCLUDED
#include <cassert>
#endif

#include <string>
#include <cstdint>
#include <type_traits>
#include <initializer_list>

#include "Library/FastDelegate.h"
#include "Meta/FunctorReference.h"

typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef uintptr_t uintptr;

typedef uint8_t byte;
typedef uint16 ushort;
typedef uint32 uint;
typedef uint64 ulong;

#define MaxUInt16 std::numeric_limits<uint16>::max()
#define MinUInt16 std::numeric_limits<uint16>::min()

#define MaxUInt32 std::numeric_limits<uint32>::max()
#define MinUInt32 std::numeric_limits<uint32>::min()

#define MaxUInt64 std::numeric_limits<uint64>::max()
#define MinUInt64 std::numeric_limits<uint64>::min()


#define MaxInt16 std::numeric_limits<int16>::max()
#define MinInt16 std::numeric_limits<int16>::min()

#define MaxInt32 std::numeric_limits<int32>::max()
#define MinInt32 std::numeric_limits<int32>::min()

#define MaxInt64 std::numeric_limits<int64>::max()
#define MinInt64 std::numeric_limits<int64>::min()


// Forward Declarations

namespace tthread
{
	class thread;
	class mutex;
}

namespace Apoc3D
{
	typedef std::wstring String;
	typedef std::u32string String32;

	//template <typename T, int32 N>
	//int32 countof(T(&)[N]) { return N; }
	
	template <typename T, int N> const char(&__countof(T(&)[N]))[N];
#define countof(arr) static_cast<int32>(sizeof(__countof(arr)))
#define COUNTOF(arr) countof(arr)

	template <typename T, int32 N>
	void ZeroArray(T(&arr)[N]) { static_assert(std::is_trivially_default_constructible<T>::value, "Type not suitable"); memset(arr, 0, sizeof(T)*N); }

	//template <typename X, typename Y, int32 N1, int32 N2>
	//void CopyArray(X(&dst)[N1], const Y(&src)[N2]) 
	//{
	//	static_assert(std::is_trivially_copyable<X>::value && std::is_trivially_copyable<Y>::value, "Type not suitable"); 
	//	static_assert(N1 * sizeof(X) == N2 * sizeof(Y), "Array size mismatch");
	//	memcpy(dst, src, sizeof(X)*N1); 
	//}

	//template <typename X, typename Y, int32 N>
	//void FillArray(X(&dst)[N], const Y* dataPtr)
	//{
	//	static_assert(std::is_trivially_copyable<X>::value, "Type not suitable");
	//	memcpy(dst, dataPtr, sizeof(X)*N);
	//}

	template <typename X, typename Y, int32 N1, int32 N2>
	typename std::enable_if<std::is_trivially_copyable<X>::value && std::is_trivially_copyable<Y>::value, void>::type
		CopyArray(X(&dst)[N1], const Y(&src)[N2])
	{
		static_assert(std::is_trivially_copyable<X>::value && std::is_trivially_copyable<Y>::value, "Type not suitable");
		static_assert(N1 * sizeof(X) == N2 * sizeof(Y), "Array size mismatch");
		memcpy(dst, src, sizeof(X)*N1);
	}

	template <typename X, int32 N>
	typename std::enable_if<!std::is_trivially_copyable<X>::value, void>::type 
		CopyArray(X(&dst)[N], const X(&src)[N])
	{
		static_assert(!std::is_trivially_copyable<X>::value, "Type not suitable");
		for (int32 i = 0; i < N; i++)
			dst[i] = src[i];
	}

	template <typename X, typename Y, int32 N>
	typename std::enable_if<std::is_trivially_copyable<X>::value, void>::type 
		FillArray(X(&dst)[N], const Y* dataPtr)
	{
		static_assert(std::is_trivially_copyable<X>::value, "Type not suitable");
		memcpy(dst, dataPtr, sizeof(X)*N);
	}

	template <typename X, int32 N>
	typename std::enable_if<!std::is_trivially_copyable<X>::value, void>::type
		FillArray(X(&dst)[N], const X* dataPtr)
	{
		static_assert(!std::is_trivially_copyable<X>::value, "Type not suitable");
		for (int32 i = 0; i < N; i++)
			dst[i] = dataPtr[i];
	}

	void DebugBreak();

	template <typename A, typename B>
	struct IsSimilar : std::false_type { };

	template <typename A>
	struct IsSimilar<A, A> : std::true_type{};

	template <typename A>
	struct IsSimilar<A, A&> : std::true_type{};
	
	template <typename A>
	struct IsSimilar<A, const A&> : std::true_type{};

	template <typename A>
	struct IsSimilar<A&, A> : std::true_type{};

	template <typename A>
	struct IsSimilar<const A&, A> : std::true_type{};


	class Project;
	class ProjectItemData;
	class ProjectCustomItem;
	class ProjectAssetItemData;
	class ProjectFolder;
	class ProjectResTexture;
	class ProjectResModel;
	class ProjectResEffect;
	class ProjectResFont;
	class ProjectItem;


	namespace Collections
	{
		template<typename T> class List;
		template<typename T> class List2D;
		template<typename T> class Queue;
		template<typename T> class Stack;

	}
	namespace Core
	{		
		class Resource;

		template<class T> 
		class ResourceHandle;

		class ResourceManager;
		struct GameTime;
		
		template<typename T>
		class BackgroundSequencialWorker;

		class Plugin;
		class PluginManager;

		class IParsable;

		class LogSet;

		struct CommandDescription;

		namespace Streaming
		{
			struct ResourceOperation;

			class AsyncProcessor;
			class GenerationTable;
		};
	};
	namespace Config
	{
		class Configuration;
		class ConfigurationSection;
		class ConfigurationFormat;

		class ConfigurationManager;
	};
	namespace Math
	{	
		class Color4;

		class Vector2;
		class Vector3;
		class Vector4;

		class Matrix;
		class Quaternion;		

		class Plane;

		class Ray;
		class RaySegment;
		
		class Point;
		class PointF;
		class Size;

		class Rectangle;
		class RectangleF;
		class Box;
		class OctreeBox;

		class BoundingSphere;		
		class BoundingBox;
		class Frustum;

		class Random;
		class Randomizer;

		class PerlinNoise;

		class GaussBlurFilter;

		class Viewport;
	};
	namespace IO
	{
		class BinaryReader;
		class BinaryWriter;

		class TaggedDataReader;
		class TaggedDataWriter;

		class Stream;
		class FileStream;
		class FileOutStream;
		class MemoryStream;
		class MemoryOutStream;
		class VirtualStream;
		class BufferedStreamReader;

		class TextureLevelData;
		class TextureData;

		class MeshData;
		class ModelData;
		class MaterialData;
		class EffectProfileData;
		class EffectData;
	};
	namespace Input
	{
		class InputAPIManager;
		class InputAPIFactory;

		class Mouse;
		class Keyboard;

		class InputRecorder;
		class InputPlayer;
	}
	namespace VFS
	{
		class Archive;
		class ArchiveFactory;

		class FileLocateRule;
		class FileSystem;

		class ResourceLocation;
		class FileLocation;
		class MemoryLocation;
		class StreamLocation;
	};

	namespace Scene
	{
		class ScenePass;
		class SceneProcedure;
		class SceneRenderer;

		class SceneManager;
		class SceneNode;
		class SimpleSceneManager;
		class SimpleSceneNode;
		class OctreeSceneManager;
		class OctreeSceneNode;
		class SceneObject;
		class BatchData;
		class DynamicObject;
	};
	namespace Graphics
	{
		enum PixelFormat;

		class Camera;
		class FreeCamera;
		class ChaseCamera;

		class Renderable;
		class GeometryData;
		class RenderOperation;
		class RenderOperationBuffer;
		class Material;
		
		class Mesh;
		class Model;
		class ModelSharedData;

		class Patch;
		class ParticleSettings;
		class ParticleSystem;

		namespace Animation
		{
			class ModelAnimationClip;
			class MaterialAnimationClip;

			class ModelKeyframe;
			class MaterialAnimationKeyframe;

			class AnimationData;

			class ModelAnimationPlayerBase;
			class RootAnimationPlayer;
			class RigidAnimationPlayer;
			class SkinnedAnimationPlayer;
			class MaterialAnimationPlayer;
		}

		namespace EffectSystem
		{
			class Effect;
			class AutomaticEffect;
			class CustomShaderEffect;
			class EffectManager;
			class MicroEffect;
			class MicroEffectCodeParser;
		};
		namespace RenderSystem
		{
			class GraphicsAPIManager;
			class GraphicsAPIFactory;

			struct ShaderSamplerState;
			struct RenderParameters;
			class DeviceContext;
			

			class Capabilities;

			class RenderView;
			class RenderWindow;
			class RenderWindowHandler;

			class ObjectFactory;
			class RenderStateManager;

			class RenderDevice;

			

			class HardwareBuffer;
			class VertexBuffer;
			class IndexBuffer;
			class DepthStencilBuffer;

			class RenderTarget;
			class CubemapRenderTarget;

			class Texture;
			class VertexElement;
			class VertexDeclaration;

			class Shader;

			class StateBlock;


			class Sprite;

			class FPSCounter;


			class DataBox;
			class DataRectangle;


			enum CubeMapFace
			{
				CUBE_PositiveX = 0,		/** Positive x-face of the cube map. */
				CUBE_NegativeX = 1,		/** Negative x-face of the cube map. */
				CUBE_PositiveY = 2,		/** Positive y-face of the cube map. */
				CUBE_NegativeY = 3,		/** Negative y-face of the cube map. */
				CUBE_PositiveZ = 4,		/** Positive z-face of the cube map. */
				CUBE_NegativeZ = 5,		/** Negative z-face of the cube map. */
				CUBE_Count = 6
			};
		}
	};
	namespace Platform
	{
		class Library;
	}

	namespace UI
	{
		class Font;
		class FontManager;

		class ControlContainer;
		class Form;
		class Control;
		class Button;
		class ButtonRow;
		class ButtonGroup;
		class RadioButton;
		class PictureBox;
		class Label;
		class ScrollBar;
		class TreeView;
		class TreeViewNode;
		class ListBox;
		class ListView;
		class TextBox;
		class CheckBox;
		class CheckboxGroup;
		class ComboBox;

		class MenuBar;
		class MenuItem;
		class SubMenu;

		class Console;

		struct ControlBounds;
		class StyleSkin;

		class MessageDialogBox;
		class InputDialogBox;
	}

	namespace Utility
	{
		template <typename T> class TypeDualConverter;
		template <typename T> class TypeToStringConverter;
		template <typename T> class TypeParseConverter;

	}
};

#endif