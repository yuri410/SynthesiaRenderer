#pragma once

#include "SRCommon.h"

namespace SR
{
	void SavePng(RenderTarget* rt, FileOutStream& strm, bool removeAlpha);
	Texture* LoadPngTexture(RenderDevice* device, const ResourceLocation& rl);
}