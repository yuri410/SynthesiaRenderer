#pragma once

#include "../SRCommon.h"

namespace SR
{
	class FileDialog
	{
	public:
		wchar_t Filter[1024];
		uint FilterIndex;
		String InitialDirectory;
		String Title;

		void SetFilter(const wchar_t* str)
		{
			int len = 0;
			while (len < 1024)
			{
				if (!str[len] && !str[len + 1])
				{
					len += 2;
					break;
				}
				len++;
			}
			memcpy(Filter, str, len * sizeof(wchar_t));
		}
		void SetFilter(const String& fileTypeName, const List<String>& exts);
		void SetAllFilesFilter();

	protected:
		wchar_t szFile[10240];

		FileDialog()
		{
			SetFilter(L"All\0*.*\0\0");
		}
	};

	/** Prompts the user to open a file. */
	class OpenFileDialog : public FileDialog
	{
	public:
		OpenFileDialog() 
		{
			Title = L"Open";
		}

		DialogResult ShowDialog();

		const List<String>& getFilePath() const { return m_filePath; }

		bool MultiSelect = false;

	private:
		List<String> m_filePath;
	};

	class SaveFileDialog : public FileDialog
	{
	public:
		SaveFileDialog()
		{
			Title = L"Save";
		}

		DialogResult ShowDialog();

		const List<String>& getFilePath() const { return m_filePath; }

	private:
		List<String> m_filePath;
	};
}
