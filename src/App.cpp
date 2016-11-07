
#include "App.h"
#include "Song.h"
#include "UI/FileDialog.h"

#include <cstddef>

namespace SR
{
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
		m_device->Clear(CLEAR_ColorAndDepth, 0xff303030, 1,0);
		m_device->BeginFrame();
		
		m_sprite->Begin(Sprite::SPRMix_RestoreStateAlphaBlended);
		
		if (m_currentSong)
		{
			Size clSize = m_window->getClientSize();

			Font* fnt = FontManager::getSingleton().getFont(L"Bender_Black_14_O");

			struct  
			{
				ColorValue face;
				ColorValue face_a;
				ColorValue bg;
			} const colorSets[] = 
			{
				{ 0xff87aacf, 0xff376bae, 0xff293139 },
				{ 0xffa1e55c, 0xff569d11, 0xff202818 },
			};

			const int32 MaxKeyWidth = 35;

			int32 minKeyCount = clSize.Width / MaxKeyWidth;

			int32 minBase7 = m_currentSong->m_minPitchBase7 - 2;
			int32 maxBase7 = m_currentSong->m_maxPitchBase7 + 2;

			if (minBase7 < 0) minBase7 = 0;
			if (maxBase7 <= minBase7) maxBase7 = minBase7 + 1;
			
			int32 pitchCount7 = Math::Max(maxBase7 - minBase7, minKeyCount);
			//int32 pitchSpan = m_currentSong->m_maxPitchBase7 - m_currentSong->m_minPitchBase7;

			const double PitchRes = clSize.Width / pitchCount7;
			const double TimeRes = 100;


			int32 octaveCount = (maxBase7 - minBase7 + 6) / 7;

			for (int32 i = 0; i < octaveCount; i++)
			{
				float xPos = 7 * i - minBase7;

				Point startPt;
				Point endPt;

				startPt.Y = 0; endPt.Y = clSize.Height;
				startPt.X = endPt.X = (int32)(xPos * PitchRes);

				m_sprite->DrawLine(SystemUI::GetWhitePixel(), startPt, endPt, CV_Gray, 2, LineCapOptions::Butt);

				xPos += 3;
				startPt.X = endPt.X = (int32)(xPos * PitchRes);
				m_sprite->DrawLine(SystemUI::GetWhitePixel(), startPt, endPt, CV_Gray, 1, LineCapOptions::Butt);
			}

			for (const auto& b : m_currentSong->m_bars)
			{
				Point startPt;
				Point endPt;

				startPt.X = 0; endPt.X = clSize.Width;
				startPt.Y = endPt.Y = clSize.Height - (int32)((b - m_viewingYScroll) * TimeRes);

				m_sprite->DrawLine(SystemUI::GetWhitePixel(), startPt, endPt, 0xff505050, 1, LineCapOptions::Butt);
			}

			for (const Note& n : m_currentSong->m_notes)
			{
				float xPos = (float)n.Base7 - minBase7;

				if (n.Accidental)
				{
					xPos += n.Accidental*0.5f;
				}

				Rectangle area;
				area.Y = clSize.Height - (int32)((n.Time - m_viewingYScroll) * TimeRes);
				area.X = (int32)(xPos * PitchRes);
				area.Width = n.Accidental ? PitchRes*0.6f : PitchRes;
				area.Height = (int32)(n.Duration * TimeRes);

				area.Y -= area.Height;
				area.X -= area.Width / 2;
				area.X += PitchRes / 2;

				const auto& colorSet = colorSets[n.Track & 1];

				m_sprite->DrawRoundedRect(SystemUI::GetWhitePixel(), area, nullptr, 7.0f, 3, n.Accidental ? colorSet.face_a : colorSet.face);
				m_sprite->DrawRoundedRectBorder(SystemUI::GetWhitePixel(), area, nullptr, 1.0f, 6.0f, 3, colorSet.bg);

				Point labelPos = area.getBottomLeft();
				String label = n.GetName();

				Point labelSize = fnt->MeasureString(label);

				labelPos.X += (area.Width - labelSize.X) / 2 - 2;
				labelPos.Y -= labelSize.Y + 2;

				fnt->DrawString(m_sprite, label, labelPos, CV_White);
			}


		}

		m_sprite->End();

		Game::Draw(time);

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

		m_mainMenu = new MenuBar(m_UIskin);


		{
			MenuItem* pojMenu = new MenuItem(L"File");
			SubMenu* pojSubMenu = new SubMenu(m_UIskin, nullptr);

			MenuItem* mi = new MenuItem(L"Open...");
			mi->event.Bind(this, &App::OpenMidi);
			pojSubMenu->Add(mi, 0);

			mi = new MenuItem(L"Export...");
			mi->event.Bind(this, &App::Export);
			pojSubMenu->Add(mi, 0);


			mi = new MenuItem(L"-");
			pojSubMenu->Add(mi, 0);

			mi = new MenuItem(L"Exit");
			mi->event.Bind(this, &App::Exit);
			pojSubMenu->Add(mi, 0);

			m_mainMenu->Add(pojMenu, pojSubMenu);
		}

		{
			MenuItem* aboutMenu = new MenuItem(L"Help");

			SubMenu* aboutSubMenu = new SubMenu(m_UIskin, nullptr);

			MenuItem* mi = new MenuItem(L"About...");
			mi->event.Bind(this, &App::ShowAbout);
			aboutSubMenu->Add(mi, 0);

			m_mainMenu->Add(aboutMenu, aboutSubMenu);
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
		}
	}


	void App::OpenMidi(MenuItem* c)
	{
		OpenFileDialog diag;
		diag.SetFilter(L"Midi", { L".midi", L".mid" });

		if (diag.ShowDialog() == DLGRES_OK)
		{
			if (m_currentSong)
			{
				DELETE_AND_NULL(m_currentSong);
			}

			m_currentSong = new Song();
			m_currentSong->Load(diag.getFilePath()[0]);
			m_currentSong->SortEvents();


		}
	}
	void App::Export(MenuItem* c)
	{

	}

	void App::Exit(MenuItem* c)
	{
		m_window->Exit();
	}

	void App::ShowAbout(MenuItem* ctrl)
	{
		m_aboutDlg->ShowModal();
	}

}
