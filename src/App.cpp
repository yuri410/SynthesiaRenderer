
#include "App.h"
#include "Song.h"
#include "UI/FileDialog.h"
#include "IOUtils.h"

#include <cstddef>

namespace SR
{
	const ColorValue clearColor = 0xff303030;

    App::App(RenderWindow* wnd)
        : Game(wnd)
    {
		
    }

	App::~App()
	{		

	}

	void App::OnFrameStart() { }
	void App::OnFrameEnd() { }
	void App::Draw(const GameTime* time)
	{
		m_device->Clear(CLEAR_ColorAndDepth, clearColor, 1,0);
		m_device->BeginFrame();
		
		m_sprite->Begin(Sprite::SPRMix_RestoreStateAlphaBlended);
		
		if (m_currentSong)
		{
			Size clSize = m_window->getClientSize();

			m_currentSong->Render(m_sprite, m_viewingYScroll, m_timeResolution, m_pitchShift);
		}

		m_sprite->End();

		if (m_exportSession)
		{
			if (!m_exportSession->isFinished())
			{
				m_exportSession->DoStep(m_sprite, m_device, m_exportBuffer, m_currentSong);
			}
			else
			{
				DELETE_AND_NULL(m_exportSession);
			}
		}

		Game::Draw(time);

		m_exportBar->Enabled = false;
		m_exportBar->Visible = false;

		if (m_exportSession)
		{
			m_exportBar->Enabled = true;
			m_exportBar->Visible = true;
			m_exportBar->CurrentValue = m_exportSession->GetProgress();
			m_exportBar->Position = Point(m_window->getClientSize().Width - m_exportBar->getWidth() - 5, 5);

			m_sprite->Begin(Sprite::SPRMix_RestoreStateAlphaBlended);

			m_exportBar->Draw(m_sprite);

			m_sprite->End();
		}

		m_device->EndFrame();

	}
	
	void App::Initialize()
	{
		m_window->setTitle(L"Synthesia Renderer");
		Game::Initialize();

		m_sprite = m_device->getObjectFactory()->CreateSprite();

	}
	void App::Finalize()
	{
		Game::Finalize();
	}
	void App::Load()
	{
		Game::Load();

		m_console->Minimize();

		{
			FileLocation fl = FileSystem::getSingleton().Locate(L"Bender_Black_14_O.fnt", FileLocateRule::Default);
			FontManager::getSingleton().LoadFont(m_device, L"Bender_Black_14_O", fl);
		}

		ObjectFactory* objFac = m_device->getObjectFactory();
		m_exportBuffer = objFac->CreateRenderTarget(1280, 720, FMT_A8R8G8B8);

		m_exportBar = new ProgressBar(m_UIskin, Point(5, 5), 250);

		m_mainMenu = new MenuBar(m_UIskin);

		m_mainMenu->Add(
		{
			L"File",
			{
				{ L"Open...",{ this, &App::OpenMidi } },
				{ L"Export...",{ this, &App::Export } },
				{ L"Quick Export",{ this, &App::QuickExport } },

				{ L"-" },
				{ L"Exit",{ this, &App::Exit } },
			}

		}, m_UIskin, nullptr);

		m_mainMenu->Add(
		{
			L"Pitch", nullptr, &m_pitchShiftMenu, true
		}, m_UIskin, nullptr);

		m_mainMenu->Add(
		{
			L"Help",
			{
				{ L"About...",{ this, &App::ShowAbout } }
			}

		}, m_UIskin, nullptr);

		{
			for (int p = -6; p < 6; p++)
			{
				MenuItem* mi = new MenuItem(L"");
				mi->UserPointer = (void*)p;
				mi->event.Bind(this, &App::MenuItem_ApplyPitchShift);
				m_pitchShiftMenu->Add(mi, nullptr);
			}

			UpdatePitchShiftOptionTexts();
		}

		SystemUI::MainMenu = m_mainMenu;

		{
			m_aboutDlg = new Form(m_UIskin, m_device, FBS_Fixed, L"About");
			m_aboutDlg->setSize(460, 175);

			Label* lbl = new Label(m_UIskin, Point(30 + 128, 30),
				L"Test. \n"
				L"Powered by Apoc3D.",
				m_aboutDlg->getWidth() - 40 - 128, TextHAlign::Left);

			PictureBox* photo = new PictureBox(m_UIskin, Point(15, 30), 1);
			photo->setSize(128, 128);

			m_aboutDlg->getControls().Add(photo);
			m_aboutDlg->getControls().Add(lbl);

			SystemUI::Add(m_aboutDlg);
		}
	}

	void App::Unload()
	{
		Game::Unload();

		DELETE_AND_NULL(m_msgDlg_fileOverwrite);
		DELETE_AND_NULL(m_exportBuffer);

		delete m_sprite;
	}

	void App::Update(const GameTime* time)
	{
		Game::Update(time);

		Mouse* mouse = InputAPIManager::getSingleton().getMouse();
		if (mouse->getDZ())
		{
			m_viewingYScroll += mouse->getDZ() / 120.0f;

			if (m_viewingYScroll < 0)
				m_viewingYScroll = 0;

			float maxScroll = 0;

			if (m_currentSong)
			{
				int viewHeight = m_window->getClientSize().Height - m_mainMenu->getHeight();

				maxScroll = (float)m_currentSong->m_duration - viewHeight / m_timeResolution;
				maxScroll = Math::Max(0.0f, maxScroll);
			}

			if (m_viewingYScroll > maxScroll)
				m_viewingYScroll = maxScroll;
		}

		if (m_msgDlg_fileOverwrite && !m_msgDlg_fileOverwrite->isActive())
		{
			if (m_msgDlg_fileOverwrite->getResult() == DLGRES_Yes)
			{
				if (m_exportSession == nullptr)
					SetExport(m_choosenExportPath);
			}

			DELETE_AND_NULL(m_msgDlg_fileOverwrite);
		}

		m_exportBar->Text = L"Exporting " + StringUtils::SingleToString(m_exportBar->CurrentValue * 100, StrFmt::fpdec<0>::val) + L"%";
		m_exportBar->Update(time);
	}


	void App::OpenMidi(MenuItem* c)
	{
		if (m_exportSession)
			return;

		OpenFileDialog diag;
		diag.SetFilter(L"Midi", { L".midi", L".mid" });

		if (diag.ShowDialog() == DLGRES_OK)
		{
			if (m_currentSong)
			{
				DELETE_AND_NULL(m_currentSong);
			}

			const String& fp = diag.getFilePath()[0];

			m_currentSong = new Song();
			m_currentSong->Load(fp);
			m_currentSong->SortEvents();

			m_currentSongPath = fp;

			m_viewingYScroll = 0;
			m_pitchShift = 0;

			UpdatePitchShiftOptionTexts();
		}
	}
	void App::Export(MenuItem* c)
	{
		if (m_currentSong && m_exportSession == nullptr)
		{
			SaveFileDialog diag;
			diag.SetFilter(L"Png Image", { L".png" });

			if (diag.ShowDialog() == DLGRES_OK)
			{
				SetExport(diag.getFilePath()[0]);
			}
		}
	}
	void App::QuickExport(MenuItem* c)
	{
		if (m_currentSong && m_exportSession == nullptr)
		{
			m_choosenExportPath = PathUtils::Combine(PathUtils::GetDirectory(m_currentSongPath), PathUtils::GetFileNameNoExt(m_currentSongPath) + L".png");

			if (File::FileExists(m_choosenExportPath))
			{
				if (m_msgDlg_fileOverwrite == nullptr)
				{
					m_msgDlg_fileOverwrite = new MessageDialogBox(m_device, m_UIskin, 
						L"File Save", L"File \"" + PathUtils::GetFileName(m_choosenExportPath) + L"\" already exists. Overwrite?",
						MessageDialogBox::MsgBox_YesNo);

					m_msgDlg_fileOverwrite->ShowModal();
				}
			}
			else
			{
				SetExport(m_choosenExportPath);
			}
		}
	}

	void App::Exit(MenuItem* c)
	{
		m_window->Exit();
	}

	void App::ShowAbout(MenuItem* ctrl)
	{
		m_aboutDlg->ShowModal();
	}

	void App::MenuItem_ApplyPitchShift(MenuItem* c)
	{
		if (c)
		{
			m_pitchShift = (int32)c->UserPointer;

			UpdatePitchShiftOptionTexts();
		}
	}

	void App::UpdatePitchShiftOptionTexts()
	{
		for (MenuItem* mi : m_pitchShiftMenu->getItems())
		{
			int p = (int)mi->UserPointer;

			String txt = p == m_pitchShift ? L" • " : L"   ";
			txt += StringUtils::IntToString(p, StringUtils::SF_ShowPositiveSign);
			txt += L" st";

			mi->setText(txt);
		}
	}

	void App::SetExport(const String& fp)
	{
		assert(m_exportSession == nullptr);

		m_exportSession = new ExportSession(m_timeResolution, m_currentSong->m_duration, m_pitchShift, fp, m_exportBuffer );
	}

	//////////////////////////////////////////////////////////////////////////

	ExportSession::ExportSession(float timeRes, double songDuration, int32 pitchShift, const String& exportPath, RenderTarget* exportBuffer)
		: m_timeResolution(timeRes), m_songDuration((float)songDuration), m_pitchShift(pitchShift)
	{
		m_bufWidth = exportBuffer->getWidth();
		m_bufHeight = exportBuffer->getHeight();
		m_contentHeight = Math::Round(songDuration * timeRes);

		m_passCount = (m_contentHeight + m_bufHeight - 1) / m_bufHeight;

		m_fileOutStream = new FileOutStream(exportPath);

		m_pngSave = BeginStreamPng(m_bufWidth, m_contentHeight, *m_fileOutStream);
	}

	ExportSession::~ExportSession()
	{
		EndStreamPng(m_pngSave);

		DELETE_AND_NULL(m_fileOutStream);
	}


	void ExportSession::DoStep(Sprite* spr, RenderDevice* dev, RenderTarget* rt, Song* song)
	{
		int32 yPos = m_bufHeight * (m_passCount - m_currentPass - 1);

		if (m_currentPassStage == 0)
		{
			float yScroll = m_songDuration * yPos / m_contentHeight;

			dev->SetRenderTarget(0, rt);
			dev->Clear(CLEAR_ColorBuffer, clearColor, 1, 0);

			spr->Begin(Sprite::SPRMix_RestoreStateAlphaBlended);

			song->Render(spr, yScroll, m_timeResolution, m_pitchShift);

			spr->End();
			dev->SetRenderTarget(0, nullptr);

			m_currentPassStage++;
		}
		else
		{
			int32 passHeight = Math::Min(m_contentHeight - yPos, m_bufHeight);
			int32 startY = m_bufHeight - passHeight;

			if (m_currentSaveRow < passHeight)
			{
				int32 saveY = startY + m_currentSaveRow;
				int32 saveHeight = Math::Min(passHeight - m_currentSaveRow, 60);

				StreamInPng(m_pngSave, rt, saveY, saveHeight, true);
				m_currentSaveRow += 60;
			}
			else
			{
				m_currentSaveRow = 0;
				m_currentPassStage = 0;
				m_currentPass++;
			}
		}
	}

	float ExportSession::GetProgress() const
	{
		float baseProgress = (float)m_currentPass / m_passCount;
		float subProgress = m_currentPassStage ? 0.1f : 0.0f;

		int32 yPos = m_bufHeight * (m_passCount - m_currentPass - 1);
		int32 passHeight = Math::Min(m_contentHeight - yPos, m_bufHeight);

		subProgress += 0.9f * Math::Saturate((float)m_currentSaveRow / passHeight);

		return baseProgress + subProgress / m_passCount;
	}

}
