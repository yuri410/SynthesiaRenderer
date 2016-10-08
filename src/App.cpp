
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
		m_device->Clear(CLEAR_ColorAndDepth, CV_DarkGray, 1,0);
		m_device->BeginFrame();
		
		m_sprite->Begin(Sprite::SPRMix_RestoreStateAlphaBlended);
		
		/*const StyleSkin* skin = getSystemUISkin();

		for (int32 i=0;i<15;i++)
		{
			//Apoc3D::Math::Rectangle drect(20, i*40+40, i, 40);
			Point pos(i*40+40,40);
			guiDrawRegion3Vert(m_sprite, pos, i, CV_White, skin->SkinTexture, skin->VScrollBarCursor);
		}

		for (int32 i=0;i<15;i++)
		{
			//Apoc3D::Math::Rectangle drect(20, i*40+40, i, 40);
			Point pos(40,i*40+100);
			guiDrawRegion3(m_sprite, pos, i, CV_White, skin->SkinTexture, skin->HScrollBarCursor);
		}*/

		m_testPB->Draw(m_sprite);

		m_testSB->Draw(m_sprite);
		m_testSB2->Draw(m_sprite);

		m_testCB->Draw(m_sprite);

		m_testLabel->Draw(m_sprite);

		if (m_testCB->IsOverriding())
			m_testCB->DrawOverlay(m_sprite);


		m_testTB->Draw(m_sprite);

		Font* fnt = FontManager::getSingleton().getFont(L"english");
		if (m_signal1 > 0)
			fnt->DrawString(m_sprite, L"changing", Point(50, 500), CV_White);
		if (m_signal2 > 0)
			fnt->DrawString(m_sprite, L"changed", Point(50, 525), CV_White);

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


		m_testPB = new ProgressBar(m_UIskin, Point(100, 100), 150);
		m_testSB = new SliderBar(m_UIskin, Point(100, 200), BarDirection::Horizontal, 250);
		m_testSB->LargeTickDivisionCount = 20;
		m_testSB->eventValueChanged.Bind(this, &App::Slider_Changed);

		m_testSB2 = new SliderBar(m_UIskin, Point(100, 250), BarDirection::Horizontal, 250);
		//m_testSB2->LargeTickDivisionCount = 1;

		m_testSB2->eventValueChanged.Bind(this, &App::Slider_Changed);


		m_testCB = new ComboBox(m_UIskin, Point(100, 300), 100, { L"Item1", L"Item2", L"Item3", L"Item4", L"Item5", L"Item6", L"Item7", L"Item8", L"Item9" });

		m_testTB = new TextBox(m_UIskin, Point(300, 300), 250);
		m_testTB->BackgroundText = L"Background Text";

		String lblText = L"Label text \nThis is a " + ControlCodes::MakeHyperLink(L"hyperlink", 0) + L".\n" + ControlCodes::MakeHyperLink(L"testLinkTo\nNextLine\n", 1);
		lblText += ControlCodes::MakeHyperLink(L"BrokenLink", 0).substr(0, 2);
		lblText += ControlCodes::MakeHyperLink(L"LinkA", 2) + L" and " + ControlCodes::MakeHyperLink(L"LinkB", 3);

		m_testLabel = new Label(m_UIskin, Point(300, 350), lblText);
		m_testLabel->eventHyperlinkRelease += { this, &App::Label_Link };
	}

	void App::Unload()
	{
		Game::Unload();


		delete m_sprite;
	}

	void App::Update(const GameTime* time)
	{
		Game::Update(time);

		m_timer += time->ElapsedTime;

		m_testPB->CurrentValue = 0.5f * sinf(m_timer) + 0.5f;
		//m_testSB->CurrentValue = m_testPB->CurrentValue;

		m_testPB->Text = StringUtils::SingleToString(m_testPB->CurrentValue * 100, StrFmt::fpdec<1>::val);

		m_testPB->Update(time);
		m_testSB->Update(time);
		m_testSB2->Update(time);
		m_testCB->Update(time);

		m_testTB->Update(time);

		m_testLabel->Update(time);

		m_signal1 -= time->getElapsedTime();
		m_signal2 -= time->getElapsedTime();
	}


	void App::OpenMidi(MenuItem* c)
	{
		OpenFileDialog diag;
		diag.SetFilter(L"Midi", { L".midi", L".mid" });

		if (diag.ShowDialog() == DLGRES_OK)
		{
			Song song;
			song.Load(diag.getFilePath()[0]);

			DebugBreak();
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

	void App::Slider_Changed(SliderBar* sb, bool finishing)
	{
		if (finishing)
			m_signal2 = 0.5f;
		else
			m_signal1 = 0.5f;
	}

	void App::Label_Link(Label*, uint16)
	{
		m_testVal++;
	}
}
