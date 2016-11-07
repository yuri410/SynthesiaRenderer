#include "Song.h"
#include "Library/MidiFile.h"

namespace SR
{
	const wchar_t* Note::GetName() const
	{
		switch (SemiTone)
		{
			case  0: return L"C";
			case  1: return L"Db";
			case  2: return L"D";
			case  3: return L"Eb";
			case  4: return L"E";
			case  5: return L"F";
			case  6: return L"F#";
			case  7: return L"G";
			case  8: return L"Ab";
			case  9: return L"A";
			case 10: return L"Bb";
			case 11: return L"B";
		}
		return L"";
	}

	void base12ToBase7(int pitch, int& base7, int& accidental)
	{
		int octave = pitch / 12;
		int chroma = pitch % 12;

		octave -= 2;
		if (octave < 0)
			octave = 0;

		accidental = 0;

		int output = 0;
		switch (chroma)
		{
			case  0: output = 0; break; // C
			case  1: output = 0; accidental = 1; break; // C#
			case  2: output = 1; break; // D
			case  3: output = 2; accidental = -1; break; // Eb
			case  4: output = 2; break; // E
			case  5: output = 3; break; // F
			case  6: output = 3; accidental = 1; break; // F#
			case  7: output = 4; break; // G
			case  8: output = 4; accidental = 1; break; // G#
			case  9: output = 5; break; // A
			case 10: output = 6; accidental = -1; break; // Bb
			case 11: output = 6; break; // B
		}
		base7 = output + 7 * octave;
	}


	struct BarChangeInfo
	{
		int tick = 0;
		int tickDuration = 0;
		//double Time = 0;
		//double Duration = 0;
	};

	void Song::Load(const String& file)
	{
		MidiFile midi(StringUtils::toPlatformNarrowString(file).c_str());

		midi.doTimeAnalysis();

		struct KeyState
		{
			bool on;
			double onTime;
		};

		List<BarChangeInfo> barChanges;

		int32 minPitch = -1;
		int32 maxPitch = -1;

		for (int i = 0; i < midi.size(); i++)
		{
			KeyState keyStates[0xff] = { 0 };
			KeyState sustainPedalState = { 0 };

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

					n.Base12 = key;
					base12ToBase7(key, n.Base7, n.Accidental);

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
				else if (m.isMeta())
				{
					int p1 = m.getP1();
					int p2 = m.getP2();
					if (p1 == 0x58 && p2 == 0x04)
					{
						int time0 = m[3];
						int time1 = 1 << m[4];

						double quaterCount = time0 / (time1 / 4.0);

						BarChangeInfo bi;
						bi.tick = m.tick;
						bi.tickDuration = (int)(quaterCount * midi.getTicksPerQuarterNote());

						barChanges.Add(bi);
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

		int dummy;
		base12ToBase7(minPitch, m_minPitchBase7, dummy);
		base12ToBase7(maxPitch, m_maxPitchBase7, dummy);

		if (barChanges.getCount() == 0)
		{
			// assume default 4/4 time
			BarChangeInfo bi;
			bi.tick = 0;
			bi.tickDuration = 4 * midi.getTicksPerQuarterNote();

			barChanges.Add(bi);
		}

		// generate bar timings
		int totalLength = midi.getTotalTimeInTicks();
		int barModeIdx = 0;
		int eventIdx = 0;

		MidiFile clone = midi;
		clone.absoluteTicks();
		clone.joinTracks();

		int tpq = clone.getTicksPerQuarterNote();
		double defaultTempo = 120.0;
		double secondsPerTick = 60.0 / (defaultTempo * tpq);

		if (clone[0][eventIdx].isTempo())
		{
			secondsPerTick = clone[0][eventIdx].getTempoSPT(tpq);
		}

		double accumulatedLength = 0;
		int accumulatedTicks = 0;
		for (int t = 0; t < totalLength; t++)
		{
			while (barModeIdx < barChanges.getCount() - 1 &&
				t >= barChanges[barModeIdx + 1].tick)
			{
				barModeIdx++;
			}

			while (eventIdx < clone[0].getSize() - 1 &&
				t >= clone[0][eventIdx + 1].tick)
			{
				eventIdx++;

				if (clone[0][eventIdx].isTempo())
				{
					secondsPerTick = clone[0][eventIdx].getTempoSPT(tpq);
				}
			}

			if (accumulatedTicks >= barChanges[barModeIdx].tickDuration)
			{
				if (m_bars.getCount())
					m_bars.Add(m_bars.LastItem() + accumulatedLength);
				else
					m_bars.Add(accumulatedLength);

				accumulatedLength = 0;
				accumulatedTicks = 0;
			}

			accumulatedTicks++;
			accumulatedLength += secondsPerTick;
		}

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

		struct TrackInfoEx
		{
			int32 ID = 0;

			int32 MedianPitch = 0;

			List<int32> pitches;
		};
		HashMap<int32, TrackInfoEx> tracks;

		for (auto& e : m_notes)
		{
			TrackInfoEx* ti = tracks.TryGetValue(e.Track);

			if (ti == nullptr)
			{
				TrackInfoEx nti;
				nti.ID = tracks.getCount();
				
				tracks.Add(e.Track, nti);

				ti = &tracks[e.Track];
			}

			if (ti)
			{
				ti->pitches.Add(e.Base7);

				e.Track = ti->ID;
			}
		}

		List<TrackInfoEx> trackInfo;
		tracks.FillValues(trackInfo);

		m_tracks.Clear();
		for (auto& e : trackInfo)
		{
			e.MedianPitch = e.pitches[e.pitches.getCount() / 2];

			TrackInfo ti;
			ti.ID = e.ID;
			ti.MedianPitch = e.MedianPitch;
			m_tracks.Add(ti);
		}

		m_tracks.Sort([](const TrackInfo& a, const TrackInfo& b)
		{
			return OrderComparer(a.MedianPitch, b.MedianPitch);
		});
	}
}