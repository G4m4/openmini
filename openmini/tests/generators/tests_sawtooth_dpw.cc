/// @filename tests_sawtooth_dpw.cc
/// @brief Sawtooth DPW generator specific tests
/// @author gm
/// @copyright gm 2013
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

#include "openmini/src/generators/generator_sawtooth_dpw.h"

// Using declarations for tested generator
using openmini::generators::SawtoothDPW;

/// @brief Generates a sawtooth, check for null mean (no DC offset)
TEST(Generators, SawtoothDPWMean) {
  const GeneratorNormFrequency freq_generator;
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random normalized frequency
    const float kFrequency(freq_generator());

    // We are generating complete periods to prevent false positive
    const unsigned int kDataLength(static_cast<unsigned int>(
                                     std::floor((1.0f / kFrequency)
                                                * kSignalDataPeriodsCount)));

    // Generating data
    SawtoothDPW generator;
    generator.SetFrequency(kFrequency * openmini::kSamplingRateHalf);

    const float kExpected(0.0f);
    const float kEpsilon(1e-2f);
    const float kActual(ComputeMean(generator, kDataLength));

    EXPECT_NEAR(kExpected, kActual, kEpsilon);
  }  // iterations?
}

/// @brief Generates a sawtooth,
/// check for normalized range (within [-1.0f ; 1.0f])
TEST(Generators, SawtoothDPWRange) {
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    const float kFrequency(std::fmod(GeneratorNormFrequency()(),
                                     kMaxFundamentalNorm));
    SawtoothDPW generator;
    generator.SetFrequency(kFrequency * openmini::kSamplingRateHalf);

    for (unsigned int i(0); i < kDataTestSetSize; ++i) {
      const float sample(generator());
      EXPECT_GE(1.0f, sample);
      EXPECT_LE(-1.0f, sample);
    }
  }
}

/// @brief Generates a sawtooth and check for expected zero crossing
/// according parameterized frequency (1 expected zero crossings per period)
TEST(Generators, SawtoothDPWZeroCrossings) {
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    const float kFrequency(std::fmod(GeneratorNormFrequency()(),
                                     kMaxFundamentalNorm));
    const unsigned int kDataLength(static_cast<unsigned int>(
                                     std::floor((1.0f / kFrequency)
                                                * kSignalDataPeriodsCount)));
    SawtoothDPW generator;
    generator.SetFrequency(kFrequency * openmini::kSamplingRateHalf);

    // Due to rounding one or even two zero crossings may be lost/added
    const int kEpsilon(2);
    const int kActual(ComputeZeroCrossing(generator, kDataLength));

    EXPECT_NEAR(kSignalDataPeriodsCount, kActual, kEpsilon);
  }
}

/// @brief Generates a sawtooth at each frequency corresponding
/// to key notes from C1 to C8, check for expected zero crossing
TEST(Generators, SawtoothDPWNotes) {
  for (unsigned int key_note(kMinKeyNote);
       key_note < kMaxKeyNote;
       ++key_note) {
    const float kFrequency(NoteToFrequency(key_note));
    const unsigned int kDataLength(
       static_cast<unsigned int>(std::floor((1.0f / kFrequency)
                                            * kSignalDataPeriodsCount
                                            * openmini::kSamplingRateHalf)));
    SawtoothDPW generator;
    generator.SetFrequency(kFrequency);

    // Due to rounding one or even two zero crossings may be lost/added
    const int kEpsilon(2);
    const int kActual(ComputeZeroCrossing(generator, kDataLength));

    EXPECT_NEAR(kSignalDataPeriodsCount, kActual, kEpsilon);
  }
}