#pragma once

#include "SRCommon.h"

namespace SR
{
	struct Note 
	{
		int32 Octave;
		int32 SemiTone;

		float Time;
		float Duration;
	};

	struct Sustain
	{
		float Time;
		float Duration;
	};

	struct Song
	{
		void Load(const String& file);

		List<Note> m_notes;
		List<Sustain> m_sustains;

		int32 m_minPitch;
	};
}
