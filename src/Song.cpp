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


	struct BarModeInfo
	{
		int tick = 0;
		int tickDuration = 0;
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

		List<BarModeInfo> barChanges;

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

						BarModeInfo bi;
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

		m_duration = midi.getTotalTimeInSeconds();

		if (barChanges.getCount() == 0)
		{
			// assume default 4/4 time
			BarModeInfo bi;
			bi.tick = 0;
			bi.tickDuration = 4 * midi.getTicksPerQuarterNote();

			barChanges.Add(bi);
		}

		// generate bar timings by simulating timeline
		int totalLength = midi.getTotalTimeInTicks();
		int barModeIdx = 0;
		int eventIdx = 0;

		MidiFile clone = midi;
		clone.absoluteTicks();
		clone.joinTracks();

		int tpq = clone.getTicksPerQuarterNote();
		double defaultTempo = 120.0;
		double secondsPerTick = 60.0 / (defaultTempo * tpq);

		if (clone[0][0].isTempo())
		{
			secondsPerTick = clone[0][0].getTempoSPT(tpq);
		}

		double accumulatedLength = 0;
		int accumulatedTicks = 0;
		for (int t = 0; t < totalLength; t++)
		{
			bool barModeChanged = false;

			while (barModeIdx < barChanges.getCount() - 1 &&
				t >= barChanges[barModeIdx + 1].tick)
			{
				barModeIdx++;
				barModeChanged = true;
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
			
			if (accumulatedTicks >= barChanges[barModeIdx].tickDuration || barModeChanged)
			{
				AppendBar(accumulatedLength);

				accumulatedLength = 0;
				accumulatedTicks = 0;
			}

			accumulatedTicks++;
			accumulatedLength += secondsPerTick;
		}

		// final bar
		if (accumulatedTicks > 0)
		{
			AppendBar(accumulatedLength);
		}
	}

	void Song::SortEvents()
	{
		m_notes.Sort([](const Note& a, const Note& b)
		{
			if (a.Track == b.Track)
				return OrderComparer(a.Time, b.Time);

			return -OrderComparer(a.Track, b.Track);
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

	void Song::AppendBar(double duration)
	{
		if (m_bars.getCount())
			m_bars.Add(m_bars.LastItem() + duration);
		else
			m_bars.Add(duration);
	}


	void Song::Render(Sprite* sprite, float yScroll, float timeResolution)
	{
		Viewport vp = sprite->getRenderDevice()->getViewport();
		Size clSize(vp.Width, vp.Height);

		Font* fnt = FontManager::getSingleton().getFont(L"Bender_Black_14_O");

		struct
		{
			ColorValue face;
			ColorValue face_a;
			ColorValue bg;
		} const colorSets[] =
		{
			{ 0xffa1e55c, 0xff569d11, 0xff202818 },
			{ 0xff87aacf, 0xff376bae, 0xff293139 },
		};

		const int32 MaxKeyWidth = 35;

		int32 minKeyCount = clSize.Width / MaxKeyWidth;

		int32 minBase7 = m_minPitchBase7 - 2;
		int32 maxBase7 = m_maxPitchBase7 + 2;

		if (minBase7 < 0) minBase7 = 0;
		if (maxBase7 <= minBase7) maxBase7 = minBase7 + 1;

		int32 pitchCount7 = Math::Max(maxBase7 - minBase7, minKeyCount);
		//int32 pitchSpan = m_currentSong->m_maxPitchBase7 - m_currentSong->m_minPitchBase7;

		const double PitchRes = clSize.Width / pitchCount7;
		const double timeRes = timeResolution;

		for (const auto& b : m_bars)
		{
			Point startPt;
			Point endPt;

			startPt.X = 0; endPt.X = clSize.Width;
			startPt.Y = endPt.Y = clSize.Height - (int32)((b - yScroll) * timeRes);

			sprite->DrawLine(SystemUI::GetWhitePixel(), startPt, endPt, 0xff505050, 1, LineCapOptions::Butt);
		}

		int32 octaveCount = (maxBase7 - minBase7 + 6) / 7;

		for (int32 i = 0; i < octaveCount; i++)
		{
			int xPos = 7 * i - minBase7;

			Point startPt;
			Point endPt;

			startPt.Y = 0; endPt.Y = clSize.Height;
			startPt.X = endPt.X = (int32)(xPos * PitchRes);

			sprite->DrawLine(SystemUI::GetWhitePixel(), startPt, endPt, CV_Gray, 2, LineCapOptions::Butt);

			xPos += 3;
			startPt.X = endPt.X = (int32)(xPos * PitchRes);
			sprite->DrawLine(SystemUI::GetWhitePixel(), startPt, endPt, CV_Gray, 1, LineCapOptions::Butt);
		}

		for (int i = m_notes.getCount() - 1; i >= 0; i--)
		{
			const Note& n = m_notes[i];

			float xPos = (float)n.Base7 - minBase7;

			if (n.Accidental)
			{
				xPos += n.Accidental*0.5f;
			}

			Rectangle area;
			area.Y = clSize.Height - (int32)((n.Time - yScroll) * timeRes);
			area.X = (int32)(xPos * PitchRes);
			area.Width = n.Accidental ? PitchRes*0.6f : PitchRes;
			area.Height = (int32)(n.Duration * timeRes);

			area.Y -= area.Height;
			area.X -= area.Width / 2;
			area.X += PitchRes / 2;

			const auto& colorSet = colorSets[n.Track & 1];

			sprite->DrawRoundedRect(SystemUI::GetWhitePixel(), area, nullptr, 7.0f, 3, n.Accidental ? colorSet.face_a : colorSet.face);
			sprite->DrawRoundedRectBorder(SystemUI::GetWhitePixel(), area, nullptr, 1.0f, 6.0f, 3, colorSet.bg);

			Point labelPos = area.getBottomLeft();
			String label = n.GetName();

			Point labelSize = fnt->MeasureString(label);

			labelPos.X += (area.Width - labelSize.X) / 2 - 2;
			labelPos.Y -= labelSize.Y + 2;

			fnt->DrawString(sprite, label, labelPos, CV_White);
		}

		sprite->Flush();
	}
}