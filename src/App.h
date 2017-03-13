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
		void QuickExport(MenuItem* c);

		void Exit(MenuItem* c);

		void MenuItem_ApplyPitchShift(MenuItem* c);
		void UpdatePitchShiftOptionTexts();

		void SetExport(const String& fp);

		Form* m_aboutDlg;

		MenuBar* m_mainMenu;
		SubMenu* m_pitchShiftMenu = nullptr;

		ProgressBar* m_exportBar = nullptr;

		Sprite* m_sprite;
		Song* m_currentSong = nullptr;
		String m_currentSongPath;
		float m_viewingYScroll = 0;
		float m_timeResolution = 100;
		int32 m_pitchShift = 0;

		MessageDialogBox* m_msgDlg_fileOverwrite = nullptr;
		String m_choosenExportPath;

		RenderTarget* m_exportBuffer = nullptr;

		class ExportSession* m_exportSession = nullptr;
    };


	class ExportSession
	{
	public:
		ExportSession(float timeRes, double songDuration, int32 pitchShift, const String& exportPath, RenderTarget* exportBuffer);
		~ExportSession();

		void DoStep(Sprite* spr, RenderDevice* dev, RenderTarget* rt, Song* song);

		bool isFinished() const { return m_currentPass >= m_passCount; }
		
		float GetProgress() const;
	private:
		FileOutStream* m_fileOutStream = nullptr;

		int32 m_currentPass = 0;
		int32 m_currentPassStage = 0;
		int32 m_currentSaveRow = 0;

		int32 m_passCount = 0;
		int32 m_bufWidth = 0;
		int32 m_bufHeight = 0;
		int32 m_contentHeight = 0;

		float m_songDuration = 0;
		float m_timeResolution = 0;
		int32 m_pitchShift = 0;

		struct PngSaveContext* m_pngSave = nullptr;

	};

}

