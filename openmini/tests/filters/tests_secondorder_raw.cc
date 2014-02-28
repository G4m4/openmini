/// @filename tests_secondorder_raw.cc
/// @brief OpenMini 2nd order filters tests
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

#include "openmini/src/filters/secondorder_raw.h"

// Using declarations for tested filter
using openmini::filters::SecondOrderRaw;

/// @brief Maximum possible resonance without overshoot due to oscillations
static const float kResonanceWithoutOverShoot(0.7f);

/// @brief Filters a random signal, check for mean close to the one
/// of the input signal (no DC offset introduced)
TEST(Filters, SecondOrderRawOutputMean) {
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random normalized frequency
    const float kFrequency(kFreqDistribution(kRandomGenerator));
    SecondOrderRaw filter;

    filter.SetParameters(kFrequency, kResonanceWithoutOverShoot);

    Sample expected_mean(Fill(0.0f));
    Sample actual_mean(Fill(0.0f));
    for (unsigned int i(0); i < kDataTestSetSize; i += openmini::SampleSize) {
      const Sample input(Fill(kNormDistribution(kRandomGenerator)));
      const Sample filtered(filter(input));
      actual_mean = Add(actual_mean, filtered);
      expected_mean = Add(expected_mean, input);
    }

    const float kActual(AddHorizontal(actual_mean));
    const float kExpected(AddHorizontal(expected_mean));
    const float kEpsilon(1e-3f * kDataTestSetSize);

    EXPECT_NEAR(kExpected, kActual, kEpsilon);
  }  // iterations?
}

/// @brief Filters a random signal with max frequency cutoff
/// (half the sampling rate) and default Q (0.7)
/// Check for minimal output/input error
TEST(Filters, SecondOrderRawPassthrough) {
  // Max cutoff frequency, in order to have a passthrough like filter
  const float kFrequency((SamplingRate::Instance().GetHalf() - 10.0f)
                         / SamplingRate::Instance().Get());
  SecondOrderRaw filter;

  filter.SetParameters(kFrequency, kResonanceWithoutOverShoot);

  Sample diff_mean(Fill(0.0f));
  for (unsigned int i(0); i < kDataTestSetSize; i += openmini::SampleSize) {
    const Sample input(Fill(kNormDistribution(kRandomGenerator)));
    const Sample filtered(filter(input));
    diff_mean = Add(diff_mean, Sub(filtered, input));
  }
  const float kExpected(0.0f);
  const float kActual(AddHorizontal(diff_mean));
  const float kEpsilon(1e-6f * kDataTestSetSize);

  EXPECT_NEAR(kExpected, kActual, kEpsilon);
}

/// @brief Filters random data (performance test)
/// @brief Generates a signal (performance tests)
TEST(Filters, SecondOrderRawPerf) {
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    const float kFrequency(kFreqDistribution(kRandomGenerator));
    SecondOrderRaw filter;
    filter.SetParameters(kFrequency, kResonanceWithoutOverShoot);

    unsigned int sample_idx(0);
    while (sample_idx < kFilterDataPerfSetSize) {
      const Sample kCurrent(Fill(kNormDistribution(kRandomGenerator)));
      // No actual test!
      EXPECT_TRUE(LessEqual(-2.0f, filter(kCurrent)));
      sample_idx += openmini::SampleSize;
    }
  }
}
