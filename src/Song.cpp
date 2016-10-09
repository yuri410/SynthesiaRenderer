#include "Song.h"
#include "Library/MidiFile.h"

namespace SR
{
	const char* Note::GetName() const
	{
		switch (SemiTone)
		{
			case  0: return "C";
			case  1: return "C#";
			case  2: return "D";
			case  3: return "Eb";
			case  4: return "E";
			case  5: return "F";
			case  6: return "F#";
			case  7: return "G";
			case  8: return "Ab";
			case  9: return "A";
			case 10: return "Bb";
			case 11: return "B";
		}
		return "";
	}


	int base12ToBase7(int pitch)
	{
		int octave = pitch / 12;
		int chroma = pitch % 12;

		octave -= 2;
		if (octave < 0)
			octave = 0;

		int output = 0;
		switch (chroma)
		{
			case  0: output = 0; break; // C
			case  1: output = 0; break; // C#
			case  2: output = 1; break; // D
			case  3: output = 2; break; // Eb
			case  4: output = 2; break; // E
			case  5: output = 3; break; // F
			case  6: output = 3; break; // F#
			case  7: output = 4; break; // G
			case  8: output = 4; break; // G#
			case  9: output = 5; break; // A
			case 10: output = 6; break; // Bb
			case 11: output = 6; break; // B
		}
		return output + 7 * octave;
	}

	void Song::Load(const String& file)
	{
		MidiFile midi(StringUtils::toPlatformNarrowString(file).c_str());

		midi.doTimeAnalysis();
		
		struct KeyState
		{
			bool on;
			double onTime;
		};
		KeyState keyStates[0xff] = { 0 };
		KeyState sustainPedalState = { 0 };

		int32 minPitch = -1;
		int32 maxPitch = -1;

		for (int i = 0; i < midi.size(); i++)
		{
			for (int j = 0; j < midi[i].size(); j++) 
			{
				MidiEvent& m = midi[i][j];

				if (m.isNoteOn())
				{
					int key = m.getKeyNumber();

					keyStates[key].on = true;
					keyStates[key].onTime = m.seconds;
				}
				else if (m.isNoteOff())
				{
					int key = m.getKeyNumber();

					keyStates[key].on = false;

					Note n;
					n.Track = i;
					n.Time = keyStates[key].onTime;
					n.Duration = m.seconds - n.Time;
					n.Octave = key / 12;
					n.SemiTone = key % 12;

					n.Octave -= 2;
					if (n.Octave < 0)
						n.Octave = 0;

					n.Base7 = base12ToBase7(key);


					m_notes.Add(n);
				}
				else if (m.isController())
				{
					int val = m.getP1();
					if (val == 64)
					{
						bool isOn = m.getP2() >= 64 ? true : false;

						if (isOn)
						{
							sustainPedalState.on = true;
							sustainPedalState.onTime = m.seconds;
						}
						else
						{
							sustainPedalState.on = false;

							Sustain s;
							s.Track = i;
							s.Time = sustainPedalState.onTime;
							s.Duration = m.seconds - s.Time;

							m_sustains.Add(s);
						}
					}
				}

				if (m.isNote())
				{
					int pitch = m.getKeyNumber();

					if (minPitch < 0 || minPitch > pitch)
					{
						minPitch = pitch;
					}
					if (maxPitch < 0 || maxPitch < pitch)
					{
						maxPitch = pitch;
					}
				}
			}
		}

		if (minPitch > 40)
		{
			minPitch = 40;
		}
		if (maxPitch < 80)
		{
			maxPitch = 80;
		}

		m_minPitchBase7 = base12ToBase7(minPitch);
		m_maxPitchBase7 = base12ToBase7(maxPitch);
	}

	void Song::SortEvents()
	{
		m_notes.Sort([](const Note& a, const Note& b)
		{
			return OrderComparer(a.Time, b.Time);
		});
		m_sustains.Sort([](const Sustain& a, const Sustain& b)
		{
			return OrderComparer(a.Time, b.Time);
		});
	}
}