#pragma once

#include "SRCommon.h"

namespace SR
{
	struct Note 
	{
		int32 Track = 0;

		int32 Octave = 0;
		int32 SemiTone = 0;
		int32 Base7 = 0;
		int32 Base12 = 0;
		int32 Accidental = 0;

		double Time = 0;
		double Duration = 0;

		const wchar_t* GetName(int pitchShift) const;
	};

	struct Sustain
	{
		int32 Track = 0;

		double Time = 0;
		double Duration = 0;
	};

	struct TrackInfo
	{
		int32 ID = 0;

		int32 MedianPitch = 0;
	};


	struct Song
	{
		void Load(const String& file);
		void SortEvents();

		void Render(Sprite* sprite, float yScroll, float timeResolution, int32 pitchShift);

		List<Note> m_notes;
		List<Sustain> m_sustains;
		List<TrackInfo> m_tracks;
		List<double> m_bars;

		int32 m_minPitchBase7;
		int32 m_maxPitchBase7;
		double m_duration;

	private:
		void AppendBar(double duration);
	};
}
