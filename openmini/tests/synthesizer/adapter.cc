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

// Using declarations for tested class
using openmini::synthesizer::Adapter;

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

