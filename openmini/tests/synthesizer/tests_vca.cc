/// @filename tests_vca.cc
/// @brief VCA specific tests
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

#include "openmini/src/synthesizer/vca.h"

// Using declarations for tested class
using openmini::synthesizer::Vca;

/// @brief Time parameters allowed max
static const unsigned int kMaxTime(kDataTestSetSize / 4);
/// @brief Time parameters random generator
static std::uniform_int_distribution<unsigned int> kTimeDistribution(0,
                                                                     kMaxTime);

/// @brief Length of the tail to check after each envelop
static const unsigned int kTail(256);

/// @brief Modulate a sinus, check for its range (must be within [-1.0 ; 1.0])
TEST(Synthesizer, VcaRange) {
  const float kFrequency(1000.0f);
  SinusGenerator input_signal(kFrequency, openmini::kSamplingRate);
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random parameters
    const unsigned int kAttack(kTimeDistribution(kRandomGenerator));
    const unsigned int kDecay(kTimeDistribution(kRandomGenerator));
    const unsigned int kSustain(kTimeDistribution(kRandomGenerator));
    const float kSustainLevel(kNormPosDistribution(kRandomGenerator));

    Vca modulator;
    modulator.SetAttack(kAttack);
    modulator.SetDecay(kDecay);
    modulator.SetSustain(kSustainLevel);

    const float kEpsilon(1e-4f);

    modulator.TriggerOn();
    unsigned int i(0);
    while (i < kAttack + kDecay * 2 + kTail) {
      if ((i >= kAttack + kDecay + kSustain)
          && ((openmini::modulators::kSustain == modulator.GetCurrentSection())
          || (openmini::modulators::kDecay == modulator.GetCurrentSection()))
          ) {
        modulator.TriggerOff();
      }
      const Sample input(FillWithGenerator(input_signal));
      const Sample amplitude(Abs(modulator(input)));
      EXPECT_TRUE(LessEqual(0.0f - kEpsilon, amplitude));
      EXPECT_TRUE(GreaterEqual(1.0f + kEpsilon, amplitude));
      i += openmini::SampleSize;
    }
  }  // iterations?
}

/// @brief Modulates a constant, check for its proper timings:
/// - when in attack samples should be in a continuous upward slope
/// - when in decay samples should be in a continuous downward slope
/// - when in sustain samples should all be equal
/// - when in release samples should be in a continuous downward slope
TEST(Synthesizer, VcaTimings) {
  const Sample kConstant(Fill(0.5f));
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random parameters
    const unsigned int kAttack(kTimeDistribution(kRandomGenerator));
    const unsigned int kDecay(kTimeDistribution(kRandomGenerator));
    const unsigned int kSustain(kTimeDistribution(kRandomGenerator));
    const float kSustainLevel(kNormPosDistribution(kRandomGenerator));

    Vca modulator;
    modulator.SetAttack(kAttack);
    modulator.SetDecay(kDecay);
    modulator.SetSustain(kSustainLevel);

    modulator.TriggerOn();
    unsigned int i(openmini::SampleSize);
    // Envelops should all begin at zero!
    Sample previous(modulator(kConstant));
    EXPECT_EQ(0.0f, previous);
    // Attack
    while (i <= kAttack) {
      const Sample sample(modulator(kConstant));
      EXPECT_TRUE(LessEqual(previous, sample));
      previous = sample;
      i += openmini::SampleSize;
    }
    // Decay
    while (i <= kAttack + kDecay) {
      const Sample sample(modulator(kConstant));
      EXPECT_TRUE(GreaterEqual(previous, sample));
      previous = sample;
      i += openmini::SampleSize;
    }
    // Sustain
    while (i <= kAttack + kDecay + kSustain) {
      const Sample sample(modulator(kConstant));
      EXPECT_TRUE(Equal(Mul(kSustainLevel, kConstant), sample));
      i += openmini::SampleSize;
    }
    previous = Mul(kSustainLevel, kConstant);
    // Release
    modulator.TriggerOff();
    while (i <= kAttack + kDecay + kSustain + kDecay) {
      const Sample sample(modulator(kConstant));
      EXPECT_TRUE(GreaterEqual(previous, sample));
      previous = sample;
      i += openmini::SampleSize;
    }
  }  // iterations?
}
