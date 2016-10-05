#pragma once
#ifndef APOC3D_FILE_H
#define APOC3D_FILE_H

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

namespace Apoc3D
{
	namespace VFS
	{
		/**
		 *  Represent the properties of a file. Also provides some
		 *  cross-platform utility function on file system.
		 */
		class APAPI File
		{
		protected:
			bool m_isInArchive;
			int64 m_fileSize;
			String m_fileName;
			String m_filePath;

			File(const String& file, int64 size, bool isInPack);
			
		public:
			bool isInArchive() const { return m_isInArchive; }
			int64 getFileSize() const { return m_fileSize; }
			const String& getFileName() const { return m_fileName; }
			const String& getDirectory() const { return m_filePath; }

			static int64 GetFileSize(const std::string& path);
			static bool FileExists(const std::string& path);
			static time_t GetFileModifiyTime(const std::string& path);
			static bool DirectoryExists(const std::string& path);

			static int64 GetFileSize(const String& path);
			static bool FileExists(const String& path);
			static time_t GetFileModifiyTime(const String& path);
			static bool DirectoryExists(const String& path);

			static bool SetFileModifiyTime(const String& path, time_t mt);


			static bool ListDirectoryFiles(const String& path, Apoc3D::Collections::List<String>& items);
			static bool ListDirectoryFilesRecursive(const String& path, Apoc3D::Collections::List<String>& items, int32 maxDepth = -1);

		};
	}
}


#endif
