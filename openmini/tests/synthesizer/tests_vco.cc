/// @filename tests_vco.cc
/// @brief VCO specific tests
/// @author gm
/// @copyright gm 2014
///
/// This file is part of OpenMini
///
/// OpenMini is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenMini is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with OpenMini.  If not, see <http://www.gnu.org/licenses/>.

#include "openmini/tests/tests.h"

// For the Differentiator
// TODO(gm): do not use SoundTailor Differentiator here
#include "soundtailor/src/generators/generators_common.h"

#include "openmini/src/synthesizer/vco.h"

// Using declarations for tested class
using openmini::synthesizer::Vco;
using soundtailor::generators::Differentiator;

/// @brief Random waveform generator
static std::uniform_int_distribution<unsigned int> kWaveformDistribution(
  0,
  openmini::Waveform::kCount - 1);

/// @brief Check that the output does not click when changing waveform
/// e.g. that the phase stay continuous
TEST(Vco, SmoothWaveformChange) {
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    Vco vco;

    const float kFrequency(kFreqDistribution(kRandomGenerator));

    // The history must be a non-integer number of periods:
    // this prevent having the transition falls on the period beginning/ending
    // Also, the transition cannot fall within a vectorized Sample
    // TODO(gm): fix it
    const float kSignalDataPeriod(3.3f);
    const unsigned int kHistoryLength(GetNextMultiple(
      static_cast<unsigned int>(std::floor((0.5f / kFrequency)
                                * kSignalDataPeriod)),
      openmini::SampleSize));

    vco.SetFrequency(kFrequency * SamplingRate::Instance().Get());

    // A small epsilon is added for differentiation imprecisions
    const float kMaxDelta(4.0f * kFrequency + 5e-5f);

    unsigned int i(openmini::SampleSize);
    Sample input(Fill(0.0f));
    // Creating an history
    while (i < kHistoryLength) {
      input = vco();
      i += openmini::SampleSize;
    }
    vco.SetWaveform(static_cast<openmini::Waveform::Type>(
                      kWaveformDistribution(kRandomGenerator)));
    IsContinuous is_continuous(kMaxDelta, GetLast(input));
    while (i < kHistoryLength + 4) {
      EXPECT_TRUE(is_continuous(vco()));
      i += openmini::SampleSize;
    }
  }  // iterations?
}
