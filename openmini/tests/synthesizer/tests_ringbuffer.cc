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
  while (ringbuf.Size() < data.size()) {
    const Sample current(VectorMath::Fill(&data[data_index]));
    ringbuf.Push(current);
    data_index += SampleSize;
  }

  // Now we pop data out with various random block sizes
  data_index = 0;
  while (data_index < data_out.size()) {
    std::uniform_int_distribution<int> kBlockSizeDistribution(1,
                                                              ringbuf.Size());
    const unsigned int kBlockSize(kBlockSizeDistribution(kRandomGenerator));
    ringbuf.Pop(&data_out[data_index], kBlockSize);
    data_index += kBlockSize;
  }

  // Data integrity check
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
}

/// @brief Simultaneous push and pop: this behaviour is closer
/// to an actual audio usage
TEST(Synthesizer, RingBufferTypicalUse) {
  const unsigned int kBlockSize(3303);
  const unsigned int kDataLength(GetNextMultiple(GetNextMultiple(32768, 3303),
                                                 SampleSize));

  RingBuffer ringbuf(kBlockSize);
  // Creating random data
  std::vector<float> data(kDataLength);
  std::vector<float> data_out(kDataLength);
  std::generate(data.begin(),
                data.end(),
                std::bind(kNormDistribution, kRandomGenerator));

  unsigned int in_data_idx(0);
  unsigned int out_data_idx(0);
  while (in_data_idx < kDataLength) {
    // At each iteration the ringbuffer is filled, then data extracted by blocks
    unsigned int block_idx(0);
    while ((block_idx < kBlockSize) && (in_data_idx < kDataLength)) {
      const Sample current(VectorMath::Fill(&data[in_data_idx]));
      ringbuf.Push(current);
      block_idx += SampleSize;
      in_data_idx += SampleSize;
    }
    ringbuf.Pop(&data_out[out_data_idx], kBlockSize);
    out_data_idx += kBlockSize;
  }

  // Data integrity check - not checking last samples due to misalignment issues
  for (unsigned int i(0); i < data_out.size() - SampleSize; ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
}

/// @brief Push zero-length data, should work and be a NOP
TEST(Synthesizer, RingBufferZeroPush) {
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
  while (ringbuf.Size() < data.size()) {
    // ...And sometimes push zero-length data
    std::bernoulli_distribution kBoolDistribution;
    const bool kYesOrNo(kBoolDistribution(kRandomGenerator));
    if (kYesOrNo) {
      ringbuf.Push(&data[data_index], 0);
    }
    else {
      const Sample current(VectorMath::Fill(&data[data_index]));
      ringbuf.Push(current);
      data_index += SampleSize;
    }
  }

  // Now we pop data out with various random block sizes...
  data_index = 0;
  while (data_index < data_out.size()) {
    std::uniform_int_distribution<int> kBlockSizeDistribution(1,
                                                              ringbuf.Size());
    const unsigned int kBlockSize(kBlockSizeDistribution(kRandomGenerator));
    ringbuf.Pop(&data_out[data_index], kBlockSize);
    data_index += kBlockSize;
  }

  // Data integrity check
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
}

/// @brief Pop zero-length data, should work and be a NOP
TEST(Synthesizer, RingBufferZeroPop) {
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
  while (ringbuf.Size() < data.size()) {
    const Sample current(VectorMath::Fill(&data[data_index]));
    ringbuf.Push(current);
    data_index += SampleSize;
  }

  // Now we pop data out with various random block sizes...
  data_index = 0;
  while (data_index < data_out.size()) {
    // ...And sometimes pop zero-length data
    std::bernoulli_distribution kBoolDistribution;
    const bool kYesOrNo(kBoolDistribution(kRandomGenerator));
    if (kYesOrNo) {
      ringbuf.Pop(&data_out[data_index], 0);
    } else {
      std::uniform_int_distribution<int> kBlockSizeDistribution(1,
        ringbuf.Size());
      const unsigned int kBlockSize(kBlockSizeDistribution(kRandomGenerator));
      ringbuf.Pop(&data_out[data_index], kBlockSize);
      data_index += kBlockSize;
    }
  }

  // Data integrity check
  for (unsigned int i(0); i < data_out.size(); ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
}
