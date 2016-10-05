#pragma once

#include "D3D9Common.h"
#include "apoc3d/Core/Plugin.h"

using namespace Apoc3D::IO;

namespace Apoc3D
{
	namespace Graphics
	{
		namespace D3D9RenderSystem
		{
			PLUGINAPI void getData(TextureData& data, D3DTexture2D* tex);
			PLUGINAPI void getData(TextureData& data, D3DTextureCube* tex);
			PLUGINAPI void getData(TextureData& data, D3DTexture3D* tex);

			PLUGINAPI void setData(const TextureData& data, D3DTextureCube* tex);
			PLUGINAPI void setData(const TextureData& data, D3DTexture2D* tex);
			PLUGINAPI void setData(const TextureData& data, D3DTexture3D* tex);

		}
	}
}