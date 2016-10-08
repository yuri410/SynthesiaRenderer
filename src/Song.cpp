#include "Song.h"
#include "Library/MidiFile.h"

namespace SR
{

	int base12ToBase7(int pitch)
	{
		int octave = pitch / 12;
		int chroma = pitch % 12;
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


	void getMinMaxPitch(const MidiFile& midifile, int& minpitch, int& maxpitch) 
	{
		minpitch = maxpitch = -1;

		int key = 0;
		for (int i = 0; i < midifile.size(); i++)
		{
			for (int j = 0; j < midifile[i].size(); j++) 
			{
				if (midifile[i][j].isNoteOn()) 
				{
					key = midifile[i][j].getP1();
					if (minpitch < 0 || minpitch > key)
					{
						minpitch = key;
					}
					if (maxpitch < 0 || maxpitch < key) 
					{
						maxpitch = key;
					}
				}
			}
		}

		//if (grandQ) 
		{
			if (minpitch > 40) 
			{
				minpitch = 40;
			}
			if (maxpitch < 80) 
			{
				maxpitch = 80;
			}
		}
		//if (diatonicQ) {
		//	minpitch = base12ToBase7(minpitch);
		//	maxpitch = base12ToBase7(maxpitch);
		//}
	}



	void Song::Load(const String& file)
	{
		MidiFile midi(StringUtils::toPlatformNarrowString(file).c_str());

		
	}
}