#pragma once
/*
-----------------------------------------------------------------------------
This source file is part of Apoc3D Engine

Copyright (c) 2009+ Tao Xin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  if not, write to the Free Software Foundation, 
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.txt.

-----------------------------------------------------------------------------
*/
#ifndef APOC3D_D3D9_ENUMERATION_H
#define APOC3D_D3D9_ENUMERATION_H

#include "D3D9Common.h"
#include "RawSettings.h"

#include "apoc3D/Collections/List.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Graphics/RenderSystem/DeviceContext.h"
#include "apoc3d/Math/Point.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Math;

namespace Apoc3D
{
	namespace Graphics
	{ 
		namespace D3D9RenderSystem
		{
			struct D3DFORMATEqualityComparer
			{
				static bool Equals(const D3DFORMAT& x, const D3DFORMAT& y) { return x == y; }
				static int64 GetHashCode(const D3DFORMAT& obj) { return obj; }
			};
			typedef HashSet<D3DFORMAT, D3DFORMATEqualityComparer> D3DFormatHashSet;

			struct EnumerationMinimumSettings
			{
				bool IsSet = false;
				D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
				int32 Width = 0;
				int32 Height = 0;
				D3DFORMAT ColorFormat = D3DFMT_UNKNOWN;
				D3DFORMAT DepthFormat = D3DFMT_UNKNOWN;
				D3DMULTISAMPLE_TYPE MultisampleType = D3DMULTISAMPLE_NONE;
				int32 RefreshRate = 0;

				void Set(const RenderParameters& params);
			};

			class DeviceInfo;
			class SettingsCombo;

			class AdapterInfo
			{
			public:
				int32 AdapterIndex;
				String GraphicsCardName;
				String DisplayName;
				List<String> MonitorNames; // a list of monitors as output

				List<D3DDISPLAYMODE> DisplayModes;
				List<DeviceInfo*> Devices;

				AdapterInfo(IDirect3D9* d3d9, int32 index, const EnumerationMinimumSettings& minSettings);
				~AdapterInfo();

				AdapterInfo(const AdapterInfo& another) = delete;

			private:
				void GetMonitorNames();
			};

			class DeviceInfo
			{
			public:
				D3DDEVTYPE DeviceType;

				D3DCAPS9 Capabilities;

				List<SettingsCombo*> SettingCombos;

				DeviceInfo(IDirect3D9* d3d9, D3DDEVTYPE devType, AdapterInfo* parentAdp,
					const D3DFormatHashSet& adapterFormats, const EnumerationMinimumSettings& minSettings);
				~DeviceInfo();

				DeviceInfo(const DeviceInfo& another) = delete;
			};

			class SettingsCombo
			{
			public:
				int32 AdapterIndex;

				D3DDEVTYPE DeviceType;
				D3DFORMAT AdapterFormat;
				D3DFORMAT BackBufferFormat;

				bool Windowed = true;

				List<D3DFORMAT> DepthStencilFormats;

				List<D3DMULTISAMPLE_TYPE> MultisampleTypes;
				List<int32> MultisampleQualities;
				List<uint32> PresentIntervals;

				AdapterInfo* ParentAdapterInfo = nullptr;
				DeviceInfo* ParentDeviceInfo = nullptr;

				SettingsCombo(IDirect3D9* d3d9, int32 adpIdx, D3DDEVTYPE devType, D3DFORMAT adpFmt, D3DFORMAT bbFmt, bool wnd,
					AdapterInfo* parentAdp, DeviceInfo* parentDev);

				SettingsCombo(const SettingsCombo& another) = default;

				float RankSettingsCombo(const RawSettings& targetOptimal, const D3DDISPLAYMODE& desktopMode) const;
				Point FindBestResolution(int32 optimalWidth, int32 optimalHeight) const;

			};
		


			class Enumeration
			{
			public:
				static const List<AdapterInfo*>& getAdapters() { return m_adapters; }
				
				static void Enumerate(IDirect3D9* d3d9);
				static void FindValidSettings(IDirect3D9* d3d9, const RenderParameters& settings, RawSettings& result);
				static void FindValidSettings(IDirect3D9* d3d9, const RawSettings& settings, RawSettings& result);

				static bool hasEnumerated() { return m_hasEnumerated; }
				
				static EnumerationMinimumSettings MinimumSettings;
			private:
				Enumeration(){ }
				~Enumeration() { }

				static List<AdapterInfo*> m_adapters;
				static bool m_hasEnumerated;

				static void BuildValidSettings(const SettingsCombo* combo, const RawSettings& input, RawSettings& result);
				static void ConvertToRawSettings(IDirect3D9* d3d9, const RenderParameters& settings, RawSettings& optimal);
			};
		}
		
	};
};

#endif