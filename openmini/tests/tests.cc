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

/// @brief Random generator seed
// It is prime, and mutually prime with the LCG multiplicator
static int rng_seed = 317;

float GeneratorNormFloatRand::operator()(void) const {
  rng_seed *= 16807;
  return static_cast<float>(rng_seed) * 4.6566129e-010f;
}

GeneratorRangedFloat::GeneratorRangedFloat(const GeneratorRangedFloat& other)
    : GeneratorNormFloatRand(),
      min_(other.min()),
      max_(other.max()) {
  // Nothing to do here for now
}

GeneratorRangedFloat::~GeneratorRangedFloat() {
  // Nothing to do here for now
}

GeneratorRangedFloat::GeneratorRangedFloat(const float min, const float max)
    : GeneratorNormFloatRand(),
      min_(min),
      max_(max) {
  // Positive values only
  ASSERT(min >= 0.0f);
  ASSERT(max >= min);
}

float GeneratorRangedFloat::operator()(void) const {
  const float normalized_positive((GeneratorNormFloatRand::operator()() + 1.0f)
                                  * 0.5f);
  return normalized_positive * (max_ - min_) + min_;
}

float GeneratorRangedFloat::min(void) const {
  return min_;
}

float GeneratorRangedFloat::max(void) const {
  return max_;
}

GeneratorRangedInteger::GeneratorRangedInteger(const int min, const int max)
    : generator_(static_cast<float>(min), static_cast<float>(max)) {
  // Nothing to do here for now
}

int GeneratorRangedInteger::operator()(void) const {
  return static_cast<int>(generator_());
}

GeneratorNormFrequency::GeneratorNormFrequency(void)
    : GeneratorRangedFloat(0.0f, kMaxFundamentalNorm) {
  // Nothing to do here for now
}

bool ClickWasFound(const float* buffer,
                   const unsigned int length,
                   const float epsilon) {
  ASSERT(buffer);
  ASSERT(length > 0);
  ASSERT(epsilon > 1.0f);

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

int FindImmediateNextMultiple(const int input,
                              const int multiple) {
  ASSERT(multiple != 0);
  ASSERT(input != 0);
  const int remainder(input % multiple);
  if (0 == remainder) {
    return input;
  }
  return input + multiple - remainder;
}
