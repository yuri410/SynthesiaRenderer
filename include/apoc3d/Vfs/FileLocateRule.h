#pragma once
#ifndef APOC3D_FILELOCATERULE_H
#define APOC3D_FILELOCATERULE_H

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

using namespace Apoc3D::Collections;

namespace Apoc3D
{	
	namespace VFS
	{
		/**
		 *  Contains several paths to look when searching file.
		 */
		class APAPI LocateCheckPoint
		{
		public:	
			LocateCheckPoint() { }
			LocateCheckPoint(std::initializer_list<String> list);

			/**
			 *  Add a check point path
			 */ 
			void AddPath(const String& path);

			void Clear()
			{
				m_pathList.Clear();
			}


			/**
			 *  Check if check point at index is pointed to a archive file
			 */
			bool hasArchivePath(int index) const
			{
				return m_pathList[index].ArchivePath.length() != 0;
			}

			/**
			 *  Get the path of the check point at index
			 */
			const String& GetPath(int index) const
			{
				return m_pathList[index].Path;
			}

			/**
			 *  Get the archive path of the check point at index
			 */
			const String& GetArchivePath(int index) const
			{
				return m_pathList[index].ArchivePath;
			}

			/**
			 *  Get the number of check point in this rule.
			 */
			int getCount() const
			{
				return m_pathList.getCount();
			}

		private:
			struct Entry
			{
				String Path;
				String ArchivePath;

				Entry() { }

				Entry(const String& path, const String& ap)
					: Path(path), ArchivePath(ap) { }

				Entry(String&& path, String&& ap)
					: Path(std::move(path)), ArchivePath(std::move(ap)) { }
			};

			List<Entry> m_pathList;
		};


		/**
		 *  Defines the rule used for locate files in FileSystem.
		 * 	The rule is a sequence of operations, called a check point(LocateCheckPoint), 
		 *	which contains one location, such as directory/archive used to looks for the file. 
		 *	The check point first added will always have the highest priority to search the file.
		 *	The later ones will only get checked if the attempt to search file failed in checking the previous locations.
		 *
		 *	Using the locating rule is the only way to locate files inside archives.
		 *	Simply using DirectorySeprator to indicate archive location in the file path when locating will not work.
		 */
		class APAPI FileLocateRule
		{
		public:
			static void Initialize();

			static FileLocateRule Textures;
			static FileLocateRule Effects;
			static FileLocateRule Default;
			static FileLocateRule Materials;

			FileLocateRule() { }

			FileLocateRule(std::initializer_list<LocateCheckPoint> checkPoints)
				: m_pathChkPt(checkPoints) { }

			FileLocateRule& operator=(std::initializer_list<LocateCheckPoint> checkPoints)
			{
				m_pathChkPt = checkPoints;
				return *this;
			}


			void AddCheckPoint(const LocateCheckPoint& coll)
			{
				m_pathChkPt.Add(coll);
			}

			int getCount() const
			{
				return m_pathChkPt.getCount();
			}
			const LocateCheckPoint& getCheckPoint(int index) const
			{
				return m_pathChkPt[index];
			}

		private:
			List<LocateCheckPoint> m_pathChkPt;
		};
	}
}
#endif