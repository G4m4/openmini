/// @filename tests_interpolator.cc
/// @brief Interpolator specific tests
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

#include "openmini/src/synthesizer/interpolation_common.h"
#include "openmini/src/synthesizer/interpolator.h"

// Using declarations for tested class
using openmini::synthesizer::Interpolator;
using openmini::synthesizer::LinearInterpolation;
using openmini::synthesizer::ExpectedOutLength;
using openmini::synthesizer::RequiredInLength;

/// @brief Linear interpolation with a ratio of exactly 1,
/// should be a passthrough
TEST(Interpolator, LinearPassthrough) {
  // Creating random data
  std::vector<float> data(kDataTestSetSize);
  std::vector<float> data_out(kDataTestSetSize);
  std::generate(data.begin(),
                data.end(),
                std::bind(kNormDistribution, kRandomGenerator));

  Interpolator interpolator;
  interpolator.SetRatio(1.0f);
  interpolator.Process(&data[0], data.size(), &data_out[0], data_out.size());

  // Data integrity check
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_FLOAT_EQ(data[i], data_out[i]);
  }
}

/// @brief Linear interpolation with a ratio of exactly 2 on a 1kHz sinusoid
/// should give us the same signal (+ noise) than a generated 2kHz one
TEST(Interpolator, LinearDoubleRatio) {
  const float kRatio(2.0f);
  std::vector<float> data(kDataTestSetSize);
  // Creating sinusoid data
  const unsigned int kExpectedLength(ExpectedOutLength(kDataTestSetSize,
                                                       kRatio));
  std::vector<float> data_expected(kExpectedLength);
  std::vector<float> data_out(kExpectedLength);
  const float kFrequency(1000.0f);
  std::generate(data.begin(),
                data.end(),
                SinusGenerator(kFrequency, openmini::kSamplingRate));
  std::generate(data_expected.begin(),
                data_expected.end(),
                SinusGenerator(kRatio * kFrequency, openmini::kSamplingRate));

  Interpolator interpolator;
  interpolator.SetRatio(kRatio);
  interpolator.Process(&data[0], data.size(), &data_out[0], kExpectedLength);

  // A minor epsilon is required due to noise introduced by the interpolation
  const float kEpsilon(1e-6f);
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_NEAR(data_expected[i], data_out[i], kEpsilon);
  }
}

/// @brief Linear interpolation with a ratio of exactly 0.5 on a 1kHz sinusoid
/// should give us the same signal (+ noise) than a generated 500Hz one
TEST(Interpolator, LinearHalfRatio) {
  const float kRatio(0.5f);
  std::vector<float> data(kDataTestSetSize);
  // Creating sinusoid data
  const unsigned int kExpectedLength(ExpectedOutLength(kDataTestSetSize,
                                                       kRatio));
  std::vector<float> data_expected(kExpectedLength);
  std::vector<float> data_out(kExpectedLength);
  const float kFrequency(1000.0f);
  std::generate(data.begin(),
                data.end(),
                SinusGenerator(kFrequency, openmini::kSamplingRate));
  std::generate(data_expected.begin(),
                data_expected.end(),
                SinusGenerator(kRatio * kFrequency, openmini::kSamplingRate));

  Interpolator interpolator;
  interpolator.SetRatio(kRatio);
  interpolator.Process(&data[0], data.size(), &data_out[0], kExpectedLength);

  // A minor epsilon is required due to noise introduced by the interpolation
  const float kEpsilon(1e-3f);
  // Last element not being checked, since interpolation is not an extrapolation
  for (unsigned int i(0); i < data_out.size() - 1; ++i) {
    EXPECT_NEAR(data_expected[i], data_out[i], kEpsilon);
  }
}

/// @brief Linear interpolation with a ratio of slightly more than 1,
/// on a known signal
TEST(Interpolator, LinearNonIntegerRate) {
  const float kRatio(1.1f);
  std::vector<float> data(kDataTestSetSize);
  // Creating sinusoid data
  const unsigned int kExpectedLength(ExpectedOutLength(kDataTestSetSize,
                                                       kRatio));
  std::vector<float> data_expected(kExpectedLength);
  std::vector<float> data_out(kExpectedLength);
  const float kFrequency(1000.0f);
  std::generate(data.begin(),
                data.end(),
                SinusGenerator(kFrequency, openmini::kSamplingRate));
  std::generate(data_expected.begin(),
                data_expected.end(),
                SinusGenerator(kRatio * kFrequency, openmini::kSamplingRate));

  Interpolator interpolator;
  interpolator.SetRatio(kRatio);
  interpolator.Process(&data[0], data.size(), &data_out[0], kExpectedLength);

  // The epsilon is quite high here (e.g. likely audible)
  // TODO(gm): optimize the interpolator
  const float kEpsilon(6e-4f);
  for (unsigned int i(0); i < data_out.size() - 1; ++i) {
    EXPECT_NEAR(data_expected[i], data_out[i], kEpsilon);
  }
}
