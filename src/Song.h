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

		double Time = 0;
		double Duration = 0;

		const char* GetName() const;
	};

	struct Sustain
	{
		int32 Track = 0;

		double Time;
		double Duration;
	};

	struct Song
	{
		void Load(const String& file);
		void SortEvents();

		List<Note> m_notes;
		List<Sustain> m_sustains;

		int32 m_minPitchBase7;
		int32 m_maxPitchBase7;
	};
}
