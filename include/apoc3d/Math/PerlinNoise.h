#pragma once
#ifndef APOC3D_PERLINNOISE_H
#define APOC3D_PERLINNOISE_H

#include "apoc3d/Common.h"

namespace Apoc3D
{
	namespace Math
	{
		class APAPI PerlinNoise
		{
		public:
			// Constructor
			PerlinNoise();
			PerlinNoise(double persistence, double frequency, int octaves);
			PerlinNoise(double persistence, double frequency, int octaves, int randomseed);
			

			// noise value in [0, 1]
			double GetUnifiedValue2D(double x, double y) const { return GetValue2D(x, y) * 0.5 + 0.5; }

			// noise value in [-1, 1]
			double GetValue1D(double x) const;
			double GetValue2D(double x, double y) const;
			double GetValue3D(double x, double y, double z) const;


			// Get
			double Persistence() const { return m_persistence; }
			double Frequency()   const { return m_frequency;   }
			int    Octaves()     const { return m_octaves;     }
			int    RandomSeed()  const { return m_randomseed;  }

			// Set
			void Set(double _persistence, double _frequency, int _octaves, int _randomseed);

			void SetPersistence(double _persistence) { m_persistence = _persistence; }
			void SetFrequency(double _frequency)     { m_frequency = _frequency;     }
			void SetOctaves(int _octaves)            { m_octaves = _octaves;         }
			void SetRandomSeed(int _randomseed)      { m_randomseed = _randomseed;   }

			static double Noise1D(int x);
			static double Noise2D(int x, int y);
			static double Noise3D(int x, int y, int z);

		private:
			static double InterpolatedNoise1D(double x);
			static double InterpolatedNoise2D(double x, double y);
			static double InterpolatedNoise3D(double x, double y, double z);

			static double SampleNoise2D(int x, int y);
			static double SampleNoise3D(int x, int y, int z);

			double m_persistence = 0;
			double m_frequency = 0;

			int m_octaves = 0;
			int m_randomseed = 0;
		};

	}
}

#endif