/// @filename tests_ringbuffer.cc
/// @brief RingBuffer specific tests
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

#include "openmini/src/synthesizer/ringbuffer.h"

// Using declarations for tested class
using openmini::synthesizer::RingBuffer;

/// @brief Push and pop random data of random length,
/// and check that no data gets corrupted
TEST(Synthesizer, RingBufferRandomPushPop) {
  std::uniform_int_distribution<int> kLengthDistribution(1, kDataTestSetSize);
  const unsigned int kRingbufferLength(kLengthDistribution(kRandomGenerator));
  RingBuffer ringbuf(kRingbufferLength);
  // Creating random data
  std::vector<float> data(kRingbufferLength);
  std::vector<float> data_out(kRingbufferLength);
  std::generate(data.begin(),
                data.end(),
                std::bind(kNormDistribution, kRandomGenerator));

  unsigned int data_index(0);
  // First, we push the data with constant block sizes
  while (ringbuf.size() < data.size()) {
    const Sample current(Fill(&data[data_index]));
    ringbuf.Push(current);
    data_index += openmini::SampleSize;
  }

  // Now we pop data out with various random block sizes
  data_index = 0;
  while (data_index < data_out.size()) {
    std::uniform_int_distribution<int> kBlockSizeDistribution(1,
                                                              ringbuf.size());
    const unsigned int kBlockSize(kBlockSizeDistribution(kRandomGenerator));
    ringbuf.Pop(&data_out[data_index], kBlockSize);
    data_index += kBlockSize;
  }

  // Data integrity check
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
}

/// @brief Simultaneous push and pop: this allows to test "wrapping"
TEST(Synthesizer, RingBufferCircularCheck) {
  std::uniform_int_distribution<int> kLengthDistribution(1, kDataTestSetSize);
  const unsigned int kRingbufferLength(FindImmediateNextMultiple(
    kLengthDistribution(kRandomGenerator),
    openmini::SampleSize));
  RingBuffer ringbuf(kRingbufferLength);
  // Creating random data
  std::vector<float> data(kRingbufferLength * 2);
  std::vector<float> data_out(kRingbufferLength * 2);
  std::generate(data.begin(),
                data.end(),
                std::bind(kNormDistribution, kRandomGenerator));

  unsigned int data_index(0);
  // First, we fill the ringbuffer
  while (ringbuf.size() < data.size() / 2) {
    const Sample current(Fill(&data[data_index]));
    ringbuf.Push(current);
    data_index += openmini::SampleSize;
  }

  // Then we extract the first half
  ringbuf.Pop(&data_out[0], data.size() / 2);

  // Then we fill the ringbuffer again
  while (ringbuf.size() < data.size() / 2) {
    const Sample current(Fill(&data[data_index]));
    ringbuf.Push(current);
    data_index += openmini::SampleSize;
  }

  // Then we extract the second half
  ringbuf.Pop(&data_out[data.size() / 2], data.size() / 2);

  // Data integrity check
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
}
