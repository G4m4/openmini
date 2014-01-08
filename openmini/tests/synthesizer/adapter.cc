/// @filename adapter.cc
/// @brief Adapter specific tests
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

#include "openmini/src/synthesizer/adapter.h"
#include "openmini/src/synthesizer/interpolation_common.h"

// Using declarations for tested class
using openmini::synthesizer::Adapter;
using openmini::synthesizer::ExpectedOutLength;

/// @brief With a ratio of exactly 1, whatever the block size,
/// the adapter should be a passthrough
TEST(Adapter, Passthrough) {
  // Creating random data
  std::vector<float> data(kDataTestSetSize);
  std::vector<float> data_out(kDataTestSetSize);
  std::generate(data.begin(),
                data.end(),
                std::bind(kNormDistribution, kRandomGenerator));

  Adapter adapter(kDataTestSetSize);
  adapter.SetFrequencies(openmini::kSamplingRate, openmini::kSamplingRate);

  unsigned int data_index(0);
  // First, we push data in
  while (data_index < data.size()) {
    const Sample current(Fill(&data[data_index]));
    adapter.Push(current);
    data_index += openmini::SampleSize;
  }

  // Now we pop data out with various random block sizes
  data_index = 0;
  while (data_index < data_out.size()) {
    std::uniform_int_distribution<int> kBlockSizeDistribution(1,
                                                              adapter.size());
    const unsigned int kBlockSize(kBlockSizeDistribution(kRandomGenerator));
    adapter.Pop(&data_out[data_index], kBlockSize);
    data_index += kBlockSize;
  }

  // Data integrity check
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
}

/// @brief When trying to output anything pushed at one sample frequency
/// into a different output format, we have to check if resizing of the internal
/// temporary buffer size is properly done
TEST(Adapter, Resizing) {
  const float kSamplingFreqIn(96000.0f);
  const float kSamplingFreqOut(48000.0f);
  const float kRatio(kSamplingFreqIn / kSamplingFreqOut);
  const unsigned int kExpectedLength(ExpectedOutLength(kDataTestSetSize,
                                                       kRatio));
  std::vector<float> data(kDataTestSetSize);
  std::vector<float> data_out(kExpectedLength);
  std::generate(data.begin(),
                data.end(),
                std::bind(kNormDistribution, kRandomGenerator));

  Adapter adapter;
  adapter.SetFrequencies(kSamplingFreqIn, kSamplingFreqOut);
  adapter.ResizeIfNeedBe(kDataTestSetSize);

  unsigned int data_index(0);
  // First, we push data in
  while (data_index < data.size()) {
    const Sample current(Fill(&data[data_index]));
    adapter.Push(current);
    data_index += openmini::SampleSize;
  }

  data_index = 0;
  // TODO(gm): Fix this end-of-buffer mess
  while (data_index < data_out.size() - 32) {
    std::uniform_int_distribution<int> kBlockSizeDistribution(2,
                                                              adapter.size());
    const unsigned int kBlockSize(kBlockSizeDistribution(kRandomGenerator));
    adapter.Pop(&data_out[data_index], kBlockSize);
    data_index += kBlockSize;
  }

  // No checks done:
  // this test should only not throw for out-of-range array access!
  EXPECT_TRUE(true);
}

/// @brief When trying to output a 1kHz sinusoid, whatever the block size,
/// generated at 96kHz into a 48kHz output one should give us the same signal
/// (+ noise) than one 1kHz sinusoid directly generated at 48kHz
TEST(Adapter, 96kTo48kSinusoid) {
  const float kFrequency(1000.0f);
  const float kSamplingFreqIn(96000.0f);
  const float kSamplingFreqOut(48000.0f);
  const float kRatio(kSamplingFreqIn / kSamplingFreqOut);
  const unsigned int kExpectedLength(ExpectedOutLength(kDataTestSetSize,
                                                       kRatio));
  // Creating sinusoid data
  std::vector<float> data(kDataTestSetSize);
  std::vector<float> data_expected(kExpectedLength);
  std::vector<float> data_out(kExpectedLength);
  std::generate(data.begin(),
                data.end(),
                SinusGenerator(kFrequency, kSamplingFreqIn));
  std::generate(data_expected.begin(),
                data_expected.end(),
                SinusGenerator(kFrequency, kSamplingFreqOut));

  Adapter adapter;
  adapter.SetFrequencies(kSamplingFreqIn, kSamplingFreqOut);

  adapter.ResizeIfNeedBe(kExpectedLength);
  unsigned int data_index(0);

  while (data_index < data.size()) {
    const Sample current(Fill(&data[data_index]));
    adapter.Push(current);
    data_index += openmini::SampleSize;
  }

  data_index = 0;
  // TODO(gm): Fix this end-of-buffer mess
  while (data_index < data_out.size() - 32) {
    std::uniform_int_distribution<int> kBlockSizeDistribution(1,
                                                              adapter.size());
    const unsigned int kBlockSize(kBlockSizeDistribution(kRandomGenerator));
    adapter.Pop(&data_out[data_index], kBlockSize);
    data_index += kBlockSize;
  }

  // Data integrity check
  // a minor epsilon is required due to noise introduced by the interpolation
  const float kEpsilon(1e-6f);
  // TODO(gm): Fix this end-of-buffer mess
  for (unsigned int i(0); i < data_out.size() - 32; ++i) {
    EXPECT_NEAR(data_expected[i], data_out[i], kEpsilon);
  }
}
