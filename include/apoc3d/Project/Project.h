#pragma once
#ifndef APOC3D_PROJECT_H
#define APOC3D_PROJECT_H

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

#include "apoc3d/Collections/List.h"
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Config/ConfigurationSection.h"
#include "apoc3d/Graphics/PixelFormat.h"
#include "apoc3d/Graphics/RenderSystem/VertexElement.h"
#include "apoc3d/Project/Properties.h"

using namespace Apoc3D::Collections;
using namespace Apoc3D::Config;
using namespace Apoc3D::Graphics;
using namespace Apoc3D::Utility;

namespace Apoc3D
{
	/**
	 *  Presents and provides load and save operation for project configuration file.
	 *  Includes project directory structure, and all parameters used to compile assets to
	 *  final formats.
	*/


	class ProjectItem;

	enum struct ProjectItemType
	{
		Custom,
		Folder,
		Material,
		MaterialSet,
		Texture,
		Model,
		TransformAnimation,
		MaterialAnimation,
		Effect,
		EffectList,
		CustomEffect,
		ShaderNetwork,
		Font,
		FontGlyphDist,
		UILayout,
		Copy,
		ItemPreset
	};
	enum struct TextureFilterType
	{
		Nearest,
		Box,
		BSpline
	};
	enum struct TextureBuildMethod
	{
		D3D,
		Devil,
		BuiltIn
	};
	enum struct TextureCompressionType
	{
		None,
		RLE,
		LZ4
	};
	enum struct MeshBuildMethod
	{
		ASS,
		/** The method for importing FBX files */
		FBX,
		D3D
	};
	
	//struct ProjectItemBuildStamp 
	//{
	//	int32 SettingsHash = 0;
	//	int64 BuildTimeHash = 0;

	//	bool operator==(const ProjectItemBuildStamp& h) const { return SettingsHash == h.SettingsHash && BuildTimeHash == h.BuildTimeHash; }
	//	bool operator!=(const ProjectItemBuildStamp& h) const { return !operator==(h); }
	//};

	/** Represent the build configuration for a specific type of assert in the project. */
	class APAPI ProjectItemData
	{
		RTTI_BASE;
	public:
		virtual ProjectItemType getType() const = 0;
		
		/** Indicates if the item's required further editing on the built file after building */
		virtual bool RequiresPostEdit() const { return false; }
		virtual bool AlwaysBuild() const { return false; }

		virtual void Parse(const ConfigurationSection* sect) = 0;

		ProjectItem* getParentItem() const { return m_parentItem; }

		/** Save the project item as an section in the project file.
		 *  @param savingBuild If true, build information in the section will be generated.
		 */
		virtual void Save(ConfigurationSection* sect, bool savingBuild) = 0;
		
		virtual List<String> GetAllInputFiles() = 0;
		virtual List<String> GetAllOutputFiles() = 0;

		virtual ~ProjectItemData() { }

		String GetAbsoluteSourcePathBase(bool ingoreRelativeBase) const;
		String GetAbsoluteDestinationPathBase(bool ingoreRelativeBase) const;

		String GetAbsoluteSourcePath(const String& path) const;
		String GetAbsoluteDestinationPath(const String& path) const;

	protected:
		Project* m_project;
		ProjectItem* m_parentItem;

		ProjectItemData(Project* prj, ProjectItem* item)
			: m_project(prj), m_parentItem(item) { }

		String WrapSourcePath(const String& path, bool isAbsolute) const;
		String WrapDestinationPath(const String& path, bool isAbsolute) const;

		//bool IsSourceFileNewer(const String& srcFile, time_t t) const;
		//bool IsSettingsNewerThan(const String& destinationFile) const;
		//bool IsSettingsNewerThan(const String& destinationFile, time_t& dstFileTime) const;

		//bool IsOutdatedSimple(const String& srcFile, const String& destinationFile) const;

		List<String> MakeOutputFileList(const String& destinationFile);
		List<String> MakeOutputFileList(const String& f1, const String& f2);
		List<String> MakeOutputFileList(const String& f1, const String& f2, const String& f3);
		List<String> MakeOutputFileList(const List<String>& files);

		List<String> MakeInputFileList(const String& srcFile);
		List<String> MakeInputFileList(const String& f1, const String& f2);
		List<String> MakeInputFileList(const String& f1, const String& f2, const String& f3);
		List<String> MakeInputFileList(const List<String>& files);

		//bool IsDestFileNotBuilt(const String& destinationFile);
	};

	/** An item preset is a parameter dictionary that other items can inherit from **/
	class APAPI ProjectItemPreset : public ProjectItemData
	{
		RTTI_DERIVED(ProjectItemPreset, ProjectItemData);
	public:
		ProjectItemPreset(Project* prj, ProjectItem* item)
			: ProjectItemData(prj, item) { }
		~ProjectItemPreset();

		ConfigurationSection* SectionCopy = nullptr;

		virtual List<String> GetAllInputFiles() override { return {}; }
		virtual List<String> GetAllOutputFiles() override { return {}; }

		virtual ProjectItemType getType() const override { return ProjectItemType::ItemPreset; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;
	private:

	};

	class APAPI ProjectCustomItem : public ProjectItemData
	{
		RTTI_DERIVED(ProjectCustomItem, ProjectItemData);
	public:
		ProjectCustomItem(Project* prj, ProjectItem* item)
			: ProjectItemData(prj, item) { }

		String EditorExtension;
		String DestFile;
		String SourceFile;

		ParameterDictionary Properties;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SourceFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestFile); }

		virtual ProjectItemType getType() const override { return ProjectItemType::Custom; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;
	};

	/** Represents the type of asset that is natively supported by the engine. */
	class APAPI ProjectAssetItemData : public ProjectItemData
	{
		RTTI_DERIVED(ProjectAssetItemData, ProjectItemData);
	protected:
		ProjectAssetItemData(Project* prj, ProjectItem* item)
			: ProjectItemData(prj, item) { }
	};
	
	/** 
	 *  Represents a project folder 
	 *
	 *  When building, a project folder can either be converted into a archive file, or
	 *  just as a folder as a result
	 */
	class APAPI ProjectFolder : public ProjectItemData
	{
		RTTI_DERIVED(ProjectFolder, ProjectItemData);
	public:
		String PackType;
		String DestinationPack;

		String SubItemsSourceRelativeBase;
		String SubItemsDestinationRelativeBase;

		bool IncludeUnpackedSubFolderItems = false;

		ProjectFolder(Project* prj, ProjectItem* item)
			: ProjectItemData(prj, item) { }

		List<ProjectItem*> SubItems;

		virtual bool AlwaysBuild() const { return true; }

		virtual List<String> GetAllInputFiles() override;
		virtual List<String> GetAllOutputFiles() override;
		
		virtual ProjectItemType getType() const override { return ProjectItemType::Folder; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;
		void SavePackBuildConfig(ConfigurationSection* sect);

		ProjectItem* GetItem(const String& path);

	private:
		void AddPackBuildSubItems(ConfigurationSection* sect, int32& idx, bool includeUnpackedSubFolderItems);
	};

	/**
	 *  A texture
	 *  When building, a texture can come from 2 type of sources.
	 *   1. For cubemaps and volume maps, a series of 2D images/textures can be used to 
	 *      assemble the final result. Each 2D texture is used as a cubemap face or a slice.
	 *   2. From image/texture files. Pixel conversion, resizing, mipmap generation can be
	 *      accomplished by the build system when specified.
	 */
	class APAPI ProjectResTexture : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResTexture, ProjectAssetItemData);
	public:
		ProjectResTexture(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SourceFile;
		String SourceAlphaFile;
		String DestinationFile;

		ProjectTextureResizingOptions Resizing;

		TextureBuildMethod Method = TextureBuildMethod::D3D;
		TextureFilterType ResizeFilterType = TextureFilterType::BSpline;
		Apoc3D::Graphics::PixelFormat NewFormat = FMT_Unknown;
		TextureCompressionType CompressionType = TextureCompressionType::None;


		bool GenerateMipmaps = false;
		bool AssembleCubemap = false;
		bool AssembleVolumeMap = false;


		HashMap<uint, String> SubMapTable;
		HashMap<uint, String> SubAlphaMapTable;


		virtual ProjectItemType getType() const override { return ProjectItemType::Texture; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override;
		virtual List<String> GetAllOutputFiles() override;
	};

	class APAPI ProjectResMaterial : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResMaterial, ProjectAssetItemData);
	public:
		ProjectResMaterial(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String DestinationFile;

		virtual ProjectItemType getType() const override { return ProjectItemType::Material; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override;
		virtual List<String> GetAllOutputFiles() override;
	};

	class APAPI ProjectResMaterialSet : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResMaterialSet, ProjectAssetItemData);
	public:
		typedef HashMap<String, ConfigurationSection*> IncludeTable;

		struct NumberRange
		{
			int32 Start;
			int32 End;

			void Parse(const String& txt);
			bool isInRange(int32 i) const { return i >= Start && i <= End; }
		};



		ProjectResMaterialSet(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SourceFile;
		String DestinationLocation;
		String DestinationToken;

		virtual ProjectItemType getType() const override { return ProjectItemType::MaterialSet; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override;
		virtual List<String> GetAllOutputFiles() override;


		static bool ResolveGenerateExpressionsInSubtree(const ConfigurationSection* src, ConfigurationSection& dst, int32 curIdx, const String& errName);
		static ConfigurationSection* MakeIncludedSection(const ConfigurationSection* src, const String& includeText, const IncludeTable& includeSources);

	private:

		static void ParseMaterialTreeWithPreprocessing(List<String>& result, const String& baseMtrlName, const ConfigurationSection* sect, const IncludeTable& includeSources);
		static void ParseMaterialTree(List<String>& result, const String& baseMtrlName, const ConfigurationSection* sect, const IncludeTable& includeSources);

		static bool ResolveGenerateExpressions(String& val, int32 curIdx);

		static void ProcessIncludeParamInSubtree(ConfigurationSection* sect, const String& paramName, const String& paramValue);
		static ConfigurationSection* MakeIncludedSection(const String& includeText, const IncludeTable& includeSources);

	};

	/**
	 *  Represents model with animation
	 *  When building, a model is converted into a .mesh file, and a .anim file.
	 *  .mesh file only contains the geometry, material information of the model, while in the anim file
	 *  frames, bones are included.
	 */
	class APAPI ProjectResModel : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResModel, ProjectAssetItemData);
	public:
		ProjectResModel(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SrcFile;
		String DstFile;
		String DstAnimationFile;

		String PresetFile;

		MeshBuildMethod Method;

		bool CompactBuild = false;
		bool CollapseMeshs = false;
		bool CollapseAll = false;

		bool UseVertexFormatConversion = false;
		List<VertexElement> ConversionVertexElements;

		virtual ProjectItemType getType() const override { return ProjectItemType::Model; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SrcFile, PresetFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DstAnimationFile, DstFile); }
	};
	
	class APAPI ProjectResEffect : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResEffect, ProjectAssetItemData);
	public:
		typedef List<std::pair<String, String>> SettingList;

		ProjectResEffect(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String VS;
		String PS;
		String GS;

		String PListFile;
		String DestFile;

		String EntryPointVS;
		String EntryPointPS;
		String EntryPointGS;

		List<String> Targets;

		bool IsDebug = false;
		bool NoOptimization = false;

		SettingList Defines;

		virtual ProjectItemType getType() const override { return ProjectItemType::Effect; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override;
		virtual List<String> GetAllOutputFiles() override;

		static SettingList Split(const String& text);
		static String Pack(const SettingList& lst);

	private:
		bool SpecifySource(const String& srcName, const String& source);
	};
	class APAPI ProjectResCustomEffect : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResCustomEffect, ProjectAssetItemData);
	public:
		ProjectResCustomEffect(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SrcVSFile;
		String SrcPSFile;
		String DestFile;
		String EntryPointVS;
		String EntryPointPS;
		String Profile;

		virtual ProjectItemType getType() const override { return ProjectItemType::CustomEffect; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SrcVSFile, SrcPSFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestFile); }
	};

	class APAPI ProjectResEffectList : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResEffectList, ProjectAssetItemData);
	public:
		ProjectResEffectList(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String DestFile;

		virtual bool AlwaysBuild() const override { return true; }

		virtual ProjectItemType getType() const override { return ProjectItemType::EffectList; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return{}; }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestFile); }
	};

	class APAPI ProjectResShaderNetwork : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResShaderNetwork, ProjectAssetItemData);
	public:
		ProjectResShaderNetwork(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SrcFile;
		String DestFile;

		virtual ProjectItemType getType() const override { return ProjectItemType::ShaderNetwork; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SrcFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestFile); }
	};

	/**
	 *  Represents font assets.
	 *  Font asset can only be built from system fonts so far.
	 *  When using languages like Chinese, Korean, the generated font file could be up to MBs in size.
	 */
	class APAPI ProjectResFont : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResFont, ProjectAssetItemData);
	public:
		ProjectResFont(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		struct CharRange
		{
			int MinChar;
			int MaxChar;
		};

		List<CharRange> Ranges;
		String SourceFile;
		String DestFile;

		float Size = 0;
		bool AntiAlias = false;


		virtual ProjectItemType getType() const override { return ProjectItemType::Font; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SourceFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestFile); }
	};

	class APAPI ProjectResFontGlyphDist : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResFontGlyphDist, ProjectAssetItemData);
	public:
		ProjectResFontGlyphDist(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }
		
		String SourceFile;
		String DestFile;

		virtual ProjectItemType getType() const override { return ProjectItemType::FontGlyphDist; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SourceFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestFile); }
	};

	class APAPI ProjectResTAnim : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResTAnim, ProjectAssetItemData);
	public:
		ProjectResTAnim(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SourceFile;
		String DestinationFile;
		bool Reverse = false;

		HashMap<String, int> ObjectIndexMapping;

		virtual ProjectItemType getType() const override { return ProjectItemType::TransformAnimation; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SourceFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestinationFile); }
	};

	class APAPI ProjectResMAnim : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResMAnim, ProjectAssetItemData);
	public:
		ProjectResMAnim(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SourceFile;
		String DestinationFile;

		virtual ProjectItemType getType() const override { return ProjectItemType::MaterialAnimation; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SourceFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestinationFile); }
	};

	class APAPI ProjectResUILayout : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResUILayout, ProjectAssetItemData);
	public:
		ProjectResUILayout(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SourceFile;
		String DestinationFile;

		virtual ProjectItemType getType() const override { return ProjectItemType::UILayout; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SourceFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestinationFile); }
	};


	class APAPI ProjectResCopy : public ProjectAssetItemData
	{
		RTTI_DERIVED(ProjectResCopy, ProjectAssetItemData);
	public:
		ProjectResCopy(Project* prj, ProjectItem* item)
			: ProjectAssetItemData(prj, item) { }

		String SourceFile;
		String DestinationFile;

		virtual ProjectItemType getType() const override { return ProjectItemType::Copy; }
		virtual void Parse(const ConfigurationSection* sect) override;
		virtual void Save(ConfigurationSection* sect, bool savingBuild) override;

		virtual List<String> GetAllInputFiles() override { return MakeInputFileList(SourceFile); }
		virtual List<String> GetAllOutputFiles() override { return MakeOutputFileList(DestinationFile); }
	};


	class APAPI ProjectBuildStamp
	{
	public:
		void Load(IO::Stream& strm);
		void Save(IO::Stream& strm);

		void Clear();

		int64 LookupSourceTimestamp(const String& path) const;
		void SetSourceTimestamp(const String& path, time_t t);

		uint32 LookupItemSettingStamp(const String& path) const;
		void SetItemSettingStamp(const String& path, uint32 hash);

		bool hasData() const { return m_hasData; }
	private:
		HashMap<String, int64> m_sourceTimeStamps;
		HashMap<String, uint32> m_itemSettingStamps;

		bool m_hasData = false;
	};

	enum struct ProjectItemOutdateType
	{
		Current,
		NewerSource,
		DifferentSetting,
		DifferentSource
	};

	/**
	 *  Represents one asset in the project.
	 *  A ProjectItem is one to one to a ProjectItemData, which means
	 *  the specific data used in specific type of item. A texture
	 *  item will use the ProjectResTexture for the data.
	 */
	class APAPI ProjectItem
	{
	public:
		ProjectItem(Project* prj)
			: m_project(prj) { }

		ProjectItem(Project* prj, const String& name, ProjectItemData* data, ProjectFolder* parent = nullptr)
			: m_project(prj), m_parentFolder(parent), m_typeData(data), m_name(name) { }

		virtual void Rename(const String& newName);
		virtual void SetParent(ProjectFolder* parent);

		void Parse(const ConfigurationSection* sect);
		ConfigurationSection* Save(bool savingBuild);

		ProjectItem* SearchItem(const String& path) const;

		bool IsNotBuilt() const;

		bool IsOutDated();		/** Check if the item's built version is outdated */
		ProjectItemOutdateType GetOutDatedType();


		void LoadPrevBuildStamp(const String& path);
		void SaveCurrentBuildStamp(const String& path, ProjectBuildStamp& stmp);
		void MarkOutdatedOutputs();

		const String& getName() const { return m_name; }
		ProjectItemType getType() const { return m_typeData->getType(); }
		ProjectItemData* getData() const { return m_typeData; }
		ProjectFolder* getParentFolder() const { return m_parentFolder; }
		Project* getProject() const { return m_project; }

	private:
		uint32 GetCurrentBuildSettingStamp();

		Project* m_project = nullptr;
		ProjectFolder* m_parentFolder = nullptr;

		ProjectItemData* m_typeData = nullptr;

		String m_name;

		uint32 m_prevBuildSettingStamp = 0;

		String m_inheritSouceItem;

	};

	struct ProjectBuildScript
	{
		ConfigurationSection* ItemTree = nullptr;

		String BaseOutputDir;

		ProjectBuildScript() { }

		ProjectBuildScript(ConfigurationSection* sect, const String& baseOutDir) 
			: ItemTree(sect), BaseOutputDir(baseOutDir) { }
	};

	class APAPI Project
	{
	public:
		void AddItem(ProjectItem* item) { m_items.Add(item); }

		/**  Load the project from a ConfigurationSection */
		void Parse(const ConfigurationSection* sect);
		
		/**  Saves the project into a file */
		void Save(const String& file);

		void LoadBuildStamp(IO::Stream& strm);
		void SaveBuildStamp(IO::Stream& strm);
		void MarkOutdatedOutputs();

		/**
		 *  Generate a series of build action represented by ConfigurationSection object.
		 *  The sequence is based on the dependency of project items.
		 */
		void GenerateBuildScripts(List<ProjectBuildScript>& result);

		

		/**  Sets the absolute paths for the project. */
		void SetPath(const String& basePath, const String* outputPath);


		ProjectItem* GetItem(const String& path);


		const String& getName() const { return m_name; }

		const String& getTexturePath() const { return m_texturePath; }		/**  Gets the relative path for textures */
		const String& getMaterialPath() const { return m_materialPath; }	/**  Gets the relative path for materials */

		const String& getOutputPath() const { return m_outputPath; }		/**  Gets the absolute path for placing imported assets or project items */
		const String& getBasePath() const { return m_basePath; }			/** Gets the absolute path for the project's source assets. */

		const List<ProjectItem*>& getItems() const { return m_items; }

		const ProjectBuildStamp& getBuildStamp() const { return m_buildStamp; }

		void setTexturePath(const String& path) { m_texturePath = path; }

	private:
		ConfigurationSection* Save();

		List<ProjectItem*> m_items;
		String m_name;

		String m_texturePath;
		String m_materialPath;

		String m_basePath;
		String m_outputPath;
		String m_originalOutputPath;

		ProjectBuildStamp m_buildStamp;
	};



	namespace ProjectUtils
	{
		APAPI extern const TypeDualConverter<ProjectItemType> ProjectItemTypeConv;
		APAPI extern const TypeDualConverter<TextureFilterType> TextureFilterTypeConv;
		APAPI extern const TypeDualConverter<TextureBuildMethod> TextureBuildMethodConv;
		APAPI extern const TypeDualConverter<TextureCompressionType> TextureCompressionTypeConv;
		APAPI extern const TypeDualConverter<MeshBuildMethod> MeshBuildMethodConv;

		APAPI extern const String BuildAttachmentSectionGUID;
	};
}
#endif