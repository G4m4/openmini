/// @filename tests_adsd.cc
/// @brief ADSD envelop generator specific tests
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

#include "openmini/src/modulators/adsd.h"

// Using declarations for tested generator
using openmini::modulators::Adsd;

/// @brief Time parameters allowed max (half a second)
static const unsigned int kMaxTime(static_cast<unsigned int>(
                                     openmini::kSamplingRateHalf));

/// @brief Time parameters random generator
static std::uniform_int_distribution<unsigned int> kTimeDistribution(0,
                                                                     kMaxTime);

/// @brief Generates an envelop, check for its range (must be >= 0)
TEST(Modulators, Adsd) {
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random parameters
    const unsigned int kAttack(kTimeDistribution(kRandomGenerator));
    const unsigned int kDecay(kTimeDistribution(kRandomGenerator));
    const unsigned int kSustain(kTimeDistribution(kRandomGenerator));
    const float kSustainLevel(kNormPosDistribution(kRandomGenerator));

    // Generating data
    Adsd generator;
    generator.SetParameters(kAttack, kDecay, kDecay, kSustainLevel);

    const float kEpsilon(1e-3f);

    generator.TriggerOn();
    unsigned int i(0);
    // Attack & Decay
    while (i < kAttack + kDecay) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
    // Sustain
    while (i < kAttack + kDecay + kSustain) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
    // Release
    generator.TriggerOff();
    while (i < kAttack + kDecay + kSustain + kDecay) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
    // A little bit after release
    while (i < kAttack + kDecay + kSustain + kDecay + kDecay) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
  }  // iterations?
}

/// @brief Generates an envelop with one or both timing parameters null
TEST(Modulators, AdsdNullParameters) {
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random parameters
    // Each parameter has half a chance to be null
    const unsigned int kAttack(kBoolDistribution(kRandomGenerator) ? kTimeDistribution(kRandomGenerator) : 0);
    const unsigned int kDecay(kBoolDistribution(kRandomGenerator) ? kTimeDistribution(kRandomGenerator) : 0);
    const unsigned int kSustain(kTimeDistribution(kRandomGenerator));
    const float kSustainLevel(kNormPosDistribution(kRandomGenerator));

    // Generating data
    Adsd generator;
    generator.SetParameters(kAttack, kDecay, kDecay, kSustainLevel);

    const float kEpsilon(1e-3f);

    generator.TriggerOn();
    unsigned int i(0);
    // Attack & Decay
    while (i < kAttack + kDecay) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
    // Sustain
    while (i < kAttack + kDecay + kSustain) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
    // Release
    generator.TriggerOff();
    while (i < kAttack + kDecay + kSustain + kDecay) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
    // A little bit after release
    while (i < kAttack + kDecay + kSustain + kDecay + kDecay) {
      const float sample(generator());
      EXPECT_LE(0.0f - kEpsilon, sample);
      EXPECT_GE(1.0f + kEpsilon, sample);
      i += 1;
    }
  }  // iterations?
}
