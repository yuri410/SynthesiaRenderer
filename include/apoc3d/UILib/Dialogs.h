#pragma once
#ifndef APOC3D_UI_DIALOGS_H
#define APOC3D_UI_DIALOGS_H

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
#include "apoc3d/Collections/HashMap.h"
#include "apoc3d/Collections/List.h"
#include "apoc3d/Meta/EventDelegate.h"

namespace Apoc3D
{
	namespace UI
	{
		enum DialogResult
		{
			DLGRES_None = 0,
			DLGRES_OK = 1 << 0,
			DLGRES_Yes = 1 << 1,
			DLGRES_Abort = 1 << 2,
			DLGRES_Retry = 1 << 3,
			DLGRES_Ignore = 1 << 4,
			DLGRES_No = 1 << 5,
			DLGRES_Cancel = 1 << 6
		};

		class APAPI MessageDialogBox
		{
		public:
			static const int MsgBox_OK = DLGRES_OK;
			static const int MsgBox_OKCancel = DLGRES_OK | DLGRES_Cancel;
			static const int MsgBox_YesNo = DLGRES_Yes | DLGRES_No;
			typedef EventDelegate<DialogResult> InputHandler;

			MessageDialogBox(Apoc3D::Graphics::RenderSystem::RenderDevice* device, const StyleSkin* skin,
				const String& title, const String& text, int buttonsFlags);
			~MessageDialogBox();

			void ShowModal();

			DialogResult getResult() const { return m_result; }
			bool isActive() const;

			InputHandler eventInputResponsed;
		private:
			void AddButton(const String& caption, const StyleSkin* skin, DialogResult dr);
			void Button_Release(Button* ctrl);

			Apoc3D::Graphics::RenderSystem::RenderDevice* m_renderDevice;

			Form* m_form;
			Label* m_content;

			Apoc3D::Collections::List<Button*> m_controlButtons;
			Apoc3D::Collections::HashMap<void*, DialogResult> m_buttonMapping;

			DialogResult m_result;
		};

		class APAPI InputDialogBox
		{
		public:
			typedef EventDelegate<InputDialogBox*> ConfirmInputHandler;

			InputDialogBox(Apoc3D::Graphics::RenderSystem::RenderDevice* device, const StyleSkin* skin,
				const String& title, const String& text, bool multiline);
			~InputDialogBox();

			void ShowModal();
			bool isActive() const;

			DialogResult getResult() const { return m_result; }
			const String& getTextInput() const;

			ConfirmInputHandler eventInputConfirmed;
		private:
			void AddButton(const String& caption, const StyleSkin* skin, DialogResult dr);
			void Button_OkRelease(Button* ctrl);
			void Button_CancelRelease(Button* ctrl);

			Apoc3D::Graphics::RenderSystem::RenderDevice* m_renderDevice;

			Form* m_form;
			Label* m_content;
			TextBox* m_inputField;

			Button* m_btnOk;
			Button* m_btnCancel;

			DialogResult m_result;
		};
	}
}

#endif