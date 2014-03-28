/// @filename tests.cc
/// @brief Tests common source file
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

// std::pow
#include <cmath>

#include "openmini/tests/tests.h"

NormFrequencyDistribution::NormFrequencyDistribution(void)
  : uniform_real_distribution(kMinFundamentalNorm, kMaxFundamentalNorm) {
  // Nothing to do here for now
}

bool ClickWasFound(const float* buffer,
                   const unsigned int length,
                   const float epsilon) {
  OPENMINI_ASSERT(buffer != nullptr);
  OPENMINI_ASSERT(length > 0);
  OPENMINI_ASSERT(epsilon > 1.0f);

  // Compute mean derivative
  float mean_derivative(0.0f);
  for (unsigned int i(0); i < length - 1; ++i) {
    mean_derivative += std::fabs(buffer[i + 1] - buffer[i]);
  }
  mean_derivative /= length;
  // This makes us do the derivative 2 times but does not require any memory
  const float kThreshold(epsilon * mean_derivative);
  for (unsigned int i(0); i < length - 1; ++i) {
    const float derivative(buffer[i + 1] - buffer[i]);
    if (derivative > kThreshold) {
      return true;
    }
  }

  return false;
}

SinusGenerator::SinusGenerator(const float freq, const float sampling_freq)
    : increment_(2.0 * openmini::Pi * freq / sampling_freq),
      current_phase_(0.0) {
  OPENMINI_ASSERT(freq > 0.0f);
  OPENMINI_ASSERT(sampling_freq > 0.0f);
  OPENMINI_ASSERT(freq <= sampling_freq / 2.0f);
}

SinusGenerator::~SinusGenerator() {
  // Nothing to do here for now
}

float SinusGenerator::operator()(void) {
  const float out(static_cast<float>(std::cos(current_phase_)));
  current_phase_ += increment_;
  return out;
}

Sample RoundToIncrement(SampleRead input, const float increment) {
  return MulConst(increment, Round(MulConst(1.0f / increment, input)));
}
