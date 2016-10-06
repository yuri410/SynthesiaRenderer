#pragma once

#ifndef APOC3D_PROPERTIES_H
#define APOC3D_PROPERTIES_H

#include "apoc3d/Common.h"
#include "apoc3d/Math/MathCommon.h"

using namespace Apoc3D::Config;

namespace Apoc3D
{
	class APAPI ProjectTextureResizingOptions
	{
	public:

		void Parse(const String& text);
		String ToString() const;

		int32 GetResizedWidth(int32 cw) const;
		int32 GetResizedHeight(int32 ch) const;
		int32 GetResizedDepth(int32 cd) const;

		bool IsResizing() const;

		int32 getNewWidth() const { return m_newWidth; }
		int32 getNewHeight() const { return m_newHeight; }
		int32 getNewDepth() const { return m_newDepth; }
	private:

		int m_newWidth = 0;
		int m_newHeight = 0;
		int m_newDepth = 0;

		float m_newWidthRatio = 0;
		float m_newHeightRatio = 0;
		float m_newDepthRatio = 0;

	};
}
#endif