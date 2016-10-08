#include "FileDialog.h"

#include <Windows.h>

namespace SR
{
	void FileDialog::SetFilter(const String& fileTypeName, const List<String>& exts)
	{
		// make filter
		wchar_t filter[1024] = { 0 };

		String right;
		//*.a;*.b
		for (int32 i = 0; i < exts.getCount(); i++)
		{
			right.append(L"*");
			right.append(exts[i]);
			if (i + 1 < exts.getCount())
			{
				right.append(L";");
			}
		}

		String left = fileTypeName;
		left.append(L"Files (");
		left.append(right);
		left.append(L")");
		memcpy(filter, left.c_str(), sizeof(wchar_t)*left.length());
		filter[left.length()] = 0;

		memcpy(filter + (left.length() + 1), right.c_str(), sizeof(wchar_t)*right.length());

		SetFilter(filter);
	}
	void FileDialog::SetAllFilesFilter()
	{
		wchar_t filter[1024] = { 0 };

		String right = L"*.*";

		String left = L"All Files (";
		left.append(right);
		left.append(L")");
		memcpy(filter, left.c_str(), sizeof(wchar_t)*left.length());
		filter[left.length()] = 0;

		memcpy(filter + (left.length() + 1), right.c_str(), sizeof(wchar_t)*right.length());

		SetFilter(filter);
	}

	DialogResult OpenFileDialog::ShowDialog()
	{
		OPENFILENAME ofn;       // common dialog box structure

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = 0;//GetForegroundWindow();
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrInitialDir = InitialDirectory.c_str();
		//Filter = L"All Files\0*.*\0";
		ofn.lpstrFilter = Filter;// Filter.c_str();//"All Files\0*.*\0\0"

		ofn.nFilterIndex = FilterIndex;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_EXPLORER;
		ofn.lpstrTitle = Title.c_str();

		if (MultiSelect)
		{
			ofn.Flags |= OFN_ALLOWMULTISELECT;
		}
		// Display the Open dialog box. 

		if (GetOpenFileName(&ofn) == TRUE)
		{
			if (MultiSelect)
			{
				m_filePath.Clear();

				// the first one is either the file path(when only one is selected) or the base path of all the files selected
				String basePath;
				String name;
				int pos = 0;
				while (1)
				{
					if (!ofn.lpstrFile[pos])
					{
						if (name.empty())
							break;

						if (basePath.empty())
						{
							basePath = name;
							name.clear();
						}
						else
						{
							if (!basePath.empty())
							{
								m_filePath.Add(PathUtils::Combine(basePath, name));
							}
							else
							{
								m_filePath.Add(name);
							}
							name.clear();
						}

					}
					else
					{
						name.append(1, ofn.lpstrFile[pos]);
					}

					pos++;
				}

				// add the only when file when only one item is selected in the dialog
				if (m_filePath.getCount() == 0)
				{
					m_filePath.Add(basePath);
				}
			}
			else
			{
				m_filePath.Clear();
				m_filePath.Add(ofn.lpstrFile);//StringUtils::Split(ofn.lpstrFile, L"\0");
			}

			return DLGRES_OK;
		}
		return DLGRES_Cancel;

	}

	DialogResult SaveFileDialog::ShowDialog()
	{
		OPENFILENAME ofn;       // common dialog box structure

								// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = 0;//GetForegroundWindow();
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrInitialDir = InitialDirectory.c_str();
		ofn.lpstrFilter = Filter;
		ofn.nFilterIndex = FilterIndex;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_NOCHANGEDIR;
		ofn.lpstrTitle = Title.c_str();

		m_filePath.Clear();
		if (GetSaveFileName(&ofn) == TRUE)
		{
			m_filePath.Add(ofn.lpstrFile);

			return DLGRES_OK;
		}
		return DLGRES_Cancel;

	}

}
