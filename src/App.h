#pragma once
#include "SRCommon.h"

namespace SR
{
	struct Song;

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

		void OpenMidi(MenuItem* c);
		void Export(MenuItem* c);

		void Exit(MenuItem* c);

		Form* m_aboutDlg;

		MenuBar* m_mainMenu;
		
		Sprite* m_sprite;
		Song* m_currentSong = nullptr;
		float m_viewingYScroll = 0;
    };
}

