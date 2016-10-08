#pragma once

#include "apoc3d/Common.h"

#include "apoc3d/Engine.h"

#include "apoc3d/Exception.h"
#include "apoc3d/Library/tinyxml.h"
#include "apoc3d/Library/tinythread.h"
#include "apoc3d/Library/lz4.h"
#include "apoc3d/Library/lz4hc.h"

#include "apoc3d/Meta/EventDelegate.h"
#include "apoc3d/Meta/FunctorReference.h"

#include "apoc3d/Collections/CollectionsCommon.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Collections/LinkedList.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/List2D.h"
#include "apoc3d/Collections/Queue.h"
#include "apoc3d/Collections/Stack.h"

#include "apoc3d/Config/ABCConfigurationFormat.h"
#include "apoc3d/Config/Configuration.h"
#include "apoc3d/Config/ConfigurationManager.h"
#include "apoc3d/Config/ConfigurationSection.h"
#include "apoc3d/Config/XmlConfigurationFormat.h"
#include "apoc3d/Config/IniConfigurationFormat.h"

#include "apoc3d/Core/BackgroundSequencialWorker.h"
#include "apoc3d/Core/CommandInterpreter.h"
#include "apoc3d/Core/CoreCommon.h"
#include "apoc3d/Core/GameTime.h"
#include "apoc3d/Core/Logging.h"
#include "apoc3d/Core/Plugin.h"
#include "apoc3d/Core/PluginManager.h"
#include "apoc3d/Core/Resource.h"
#include "apoc3d/Core/ResourceHandle.h"
#include "apoc3d/Core/ResourceManager.h"

#include "apoc3d/Graphics/BatchModelBuilder.h"
#include "apoc3d/Graphics/Camera.h"
#include "apoc3d/Graphics/GeometryData.h"
#include "apoc3d/Graphics/GraphicsCommon.h"
#include "apoc3d/Graphics/LockData.h"
#include "apoc3d/Graphics/Material.h"
#include "apoc3d/Graphics/MaterialTypes.h"
#include "apoc3d/Graphics/Mesh.h"
#include "apoc3d/Graphics/Model.h"
#include "apoc3d/Graphics/ModelManager.h"
#include "apoc3d/Graphics/ModelTypes.h"
#include "apoc3d/Graphics/ParticleSettings.h"
#include "apoc3d/Graphics/ParticleSystem.h"
#include "apoc3d/Graphics/Patch.h"
#include "apoc3d/Graphics/PixelFormat.h"
#include "apoc3d/Graphics/Renderable.h"
#include "apoc3d/Graphics/RenderOperation.h"
#include "apoc3d/Graphics/RenderOperationBuffer.h"
#include "apoc3d/Graphics/TextureManager.h"
#include "apoc3d/Graphics/VertexFormats.h"

#include "apoc3d/Graphics/Animation/AnimationData.h"
#include "apoc3d/Graphics/Animation/AnimationManager.h"
#include "apoc3d/Graphics/Animation/AnimationPlayers.h"
#include "apoc3d/Graphics/Animation/AnimationTypes.h"

#include "apoc3d/Graphics/EffectSystem/Effect.h"
#include "apoc3d/Graphics/EffectSystem/EffectManager.h"
#include "apoc3d/Graphics/EffectSystem/EffectParameter.h"

#include "apoc3d/Graphics/RenderSystem/HardwareBuffer.h"
#include "apoc3d/Graphics/RenderSystem/DeviceContext.h"
#include "apoc3d/Graphics/RenderSystem/GraphicsAPI.h"
#include "apoc3d/Graphics/RenderSystem/InstancingData.h"
#include "apoc3d/Graphics/RenderSystem/RenderDevice.h"
#include "apoc3d/Graphics/RenderSystem/RenderStateManager.h"
#include "apoc3d/Graphics/RenderSystem/RenderTarget.h"
#include "apoc3d/Graphics/RenderSystem/RenderWindow.h"
#include "apoc3d/Graphics/RenderSystem/RenderWindowHandler.h"
#include "apoc3d/Graphics/RenderSystem/Shader.h"
#include "apoc3d/Graphics/RenderSystem/Sprite.h"
#include "apoc3d/Graphics/RenderSystem/Texture.h"
#include "apoc3d/Graphics/RenderSystem/VertexDeclaration.h"
#include "apoc3d/Graphics/RenderSystem/VertexElement.h"

#include "apoc3d/Input/InputAPI.h"
#include "apoc3d/Input/Keyboard.h"
#include "apoc3d/Input/Mouse.h"

#include "apoc3d/IOLib/TextData.h"
#include "apoc3d/IOLib/BinaryReader.h"
#include "apoc3d/IOLib/BinaryWriter.h"
#include "apoc3d/IOLib/EffectData.h"
#include "apoc3d/IOLib/IOUtils.h"
#include "apoc3d/IOLib/MaterialData.h"
#include "apoc3d/IOLib/ModelData.h"
#include "apoc3d/IOLib/PatchData.h"
#include "apoc3d/IOLib/Streams.h"
#include "apoc3d/IOLib/TaggedData.h"
#include "apoc3d/IOLib/TextureData.h"

#include "apoc3d/Math/BoundingBox.h"
#include "apoc3d/Math/BoundingSphere.h"
#include "apoc3d/Math/Box.h"
#include "apoc3d/Math/Color.h"
#include "apoc3d/Math/ColorValue.h"
#include "apoc3d/Math/DoubleMath.h"
#include "apoc3d/Math/Frustum.h"
#include "apoc3d/Math/GaussBlurFilter.h"
#include "apoc3d/Math/Math.h"
#include "apoc3d/Math/Matrix.h"
#include "apoc3d/Math/OctreeBox.h"
#include "apoc3d/Math/PerlinNoise.h"
#include "apoc3d/Math/Plane.h"
#include "apoc3d/Math/Point.h"
#include "apoc3d/Math/Quaternion.h"
#include "apoc3d/Math/RandomUtils.h"
#include "apoc3d/Math/Ray.h"
#include "apoc3d/Math/Rectangle.h"
#include "apoc3d/Math/sse_functions.h"
#include "apoc3d/Math/Vector.h"
#include "apoc3d/Math/Viewport.h"

#include "apoc3d/Platform/API.h"
#include "apoc3d/Platform/Library.h"
#include "apoc3d/Platform/Thread.h"

#include "apoc3d/Scene/OctreeSceneManager.h"
#include "apoc3d/Scene/SceneManager.h"
#include "apoc3d/Scene/SceneNode.h"
#include "apoc3d/Scene/SceneObject.h"
#include "apoc3d/Scene/ScenePass.h"
#include "apoc3d/Scene/ScenePassTypes.h"
#include "apoc3d/Scene/SceneProcedure.h"
#include "apoc3d/Scene/SceneRenderer.h"
#include "apoc3d/Scene/SceneRenderScriptParser.h"
#include "apoc3d/Scene/SimpleSceneManager.h"

#include "apoc3d/UILib/Button.h"
#include "apoc3d/UILib/CheckBox.h"
#include "apoc3d/UILib/ComboBox.h"
#include "apoc3d/UILib/Console.h"
#include "apoc3d/UILib/Control.h"
#include "apoc3d/UILib/FontManager.h"
#include "apoc3d/UILib/Form.h"
#include "apoc3d/UILib/KeyboardHelper.h"
#include "apoc3d/UILib/Text.h"
#include "apoc3d/UILib/List.h"
#include "apoc3d/UILib/Menu.h"
#include "apoc3d/UILib/PictureBox.h"
#include "apoc3d/UILib/Bar.h"
#include "apoc3d/UILib/StyleSkin.h"
#include "apoc3d/UILib/Dialogs.h"
#include "apoc3d/UILib/UICommon.h"

#include "apoc3d/Utility/StringTable.h"
#include "apoc3d/Utility/StringUtils.h"
#include "apoc3d/Utility/TypeConverter.h"
#include "apoc3d/Utility/Graph.h"

#include "apoc3d/Vfs/Archive.h"
#include "apoc3d/Vfs/File.h"
#include "apoc3d/Vfs/FileLocateRule.h"
#include "apoc3d/Vfs/FileSystem.h"
#include "apoc3d/Vfs/PathUtils.h"
#include "apoc3d/Vfs/ResourceLocation.h"
#include "apoc3d/Vfs/VFSCommon.h"

#include "Apoc3D.Essentials/EssentialCommon.h"
#include "Apoc3D.Essentials/Game.h"
#include "Apoc3D.Essentials/AI/PathFinder.h"

#ifndef APOC3D_DYNLIB
#include "Apoc3D.D3D9RenderSystem/Plugin.h"
#include "Apoc3D.WindowsInput/Plugin.h"
#endif

using namespace Apoc3D;
using namespace Apoc3D::Collections;
using namespace Apoc3D::Config;
using namespace Apoc3D::Core;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Input;
using namespace Apoc3D::IO;
using namespace Apoc3D::Math;
using namespace Apoc3D::Platform;
using namespace Apoc3D::Scene;
using namespace Apoc3D::UI;
using namespace Apoc3D::Utility;
using namespace Apoc3D::VFS;

using namespace Apoc3DEx::AI;