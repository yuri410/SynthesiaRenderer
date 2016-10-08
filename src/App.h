#pragma once
#include "SRCommon.h"

namespace SR
{
    class App : public Apoc3DEx::Game
	{
	public:
		static const int DMCount = 8;

		App(RenderWindow* wnd);
		~App();

		virtual void Initialize();
		virtual void Finalize();

		virtual void Load();
		virtual void Unload();
		virtual void Update(const GameTime* time);
		virtual void Draw(const GameTime* time);

		virtual void OnFrameStart();
		virtual void OnFrameEnd();

	private:

		void ShowAbout(MenuItem* ctrl);

		void Slider_Changed(SliderBar* sb, bool finishing);
		
		void Label_Link(Label*, uint16);

		Form* m_aboutDlg;

		MenuBar* m_mainMenu;
		ProgressBar* m_testPB;
		SliderBar* m_testSB;
		SliderBar* m_testSB2;

		ComboBox* m_testCB;

		TextBox* m_testTB;

		Label* m_testLabel;

		float m_signal1 = 0;
		float m_signal2 = 0;
		int32 m_testVal = 0;

		float m_timer = 0;

		Sprite* m_sprite;
    };
}

