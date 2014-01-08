/// @filename interpolator.cc
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

