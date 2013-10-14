/// @filename tests_generators_common.h
/// @brief OpenMini common generators tests
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

#include "openmini/src/generators/generators_common.h"

// Using declarations for tested generator
using openmini::generators::PhaseAccumulator;
using openmini::generators::Differentiator;

/// @brief Generates a triangle, check for null mean (no DC offset)
TEST(Generators, PhaseAccumulatorMean) {
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
    PhaseAccumulator generator;
    generator.SetFrequency(kFrequency * openmini::kSamplingRateHalf);

    const float kExpected(0.0f);
    // Epsilon is quite big here, this generator being very crude
    const float kEpsilon(1e-1f);
    const float kActual(ComputeMean(generator, kDataLength));

    EXPECT_NEAR(kExpected, kActual, kEpsilon);
  }  // iterations?
}

/// @brief Generates a triangle,
/// check for normalized range (within [-1.0f ; 1.0f])
TEST(Generators, PhaseAccumulatorRange) {
  const GeneratorNormFrequency freq_generator;
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    const float kFrequency(freq_generator());
    PhaseAccumulator generator;
    generator.SetFrequency(kFrequency * openmini::kSamplingRateHalf);

    for (unsigned int i(0); i < kDataTestSetSize; ++i) {
      const float sample(generator());
      EXPECT_GE(1.0f, sample);
      EXPECT_LE(-1.0f, sample);
    }
  }
}

/// @brief Generates a triangle and check for expected zero crossing
/// according parameterized frequency (1 expected zero crossings per period)
TEST(Generators, PhaseAccumulatorZeroCrossings) {
  const GeneratorNormFrequency freq_generator;
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    const float kFrequency(freq_generator());
    const unsigned int kDataLength(static_cast<unsigned int>(
                                     std::floor((1.0f / kFrequency)
                                                * kSignalDataPeriodsCount)));
    PhaseAccumulator generator;
    generator.SetFrequency(kFrequency * openmini::kSamplingRateHalf);

    // Due to rounding one or even two zero crossings may be lost/added
    const int kEpsilon(2);
    const int kActual(ComputeZeroCrossing(generator, kDataLength));

    EXPECT_NEAR(kSignalDataPeriodsCount, kActual, kEpsilon);
  }
}

/// @brief Differentiate a constant, check for null derivative
TEST(Generators, DifferentiatedConstant) {
  // Filling a vector with one random value in [0.0f ; 1.0f]
  std::vector<float> data(kDataTestSetSize, GeneratorNormFrequency()());

  Differentiator differentiator;
  // Not checking the first value!
  differentiator(data[0]);
  for (unsigned int i(1); i < kDataTestSetSize; ++i) {
    EXPECT_FLOAT_EQ(0.0f, differentiator(data[i]));
  }
}

/// @brief Generates a triangle, check for its differentiated output:
/// it is supposed to be almost null everywhere except at discontinuities
TEST(Generators, DifferentiatedSawtooth) {
  const GeneratorNormFrequency freq_generator;
  const float kFrequency(freq_generator());
  PhaseAccumulator generator;
  generator.SetFrequency(kFrequency * openmini::kSamplingRateHalf);
  std::vector<float> data(kDataTestSetSize);
  std::generate(data.begin(),
                data.end(),
                generator);

  // This is the sawtooth period e.g. each time the discontinuity occurs
  const int kPeriod(static_cast<int>(
    std::floor(kFrequency * openmini::kSamplingRateHalf)));
  // The sawtooth is not perfect:
  // there may be a small DC offset for its derivative
  const float kThreshold(0.1f);

  Differentiator differentiator;
  for (unsigned int i(0); i < kDataTestSetSize; ++i) {
    const float diff(differentiator(data[i]));
    if (i % kPeriod != 0) {
      EXPECT_GT(kThreshold, diff);
    }
  }
}
