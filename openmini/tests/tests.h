/// @filename tests.h
/// @brief Tests common include file
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

#ifndef OPENMINI_TESTS_TESTS_H_
#define OPENMINI_TESTS_TESTS_H_

// std::generate
#include <algorithm>
// std::floor, std::pow
#include <cmath>
// rand(), RAND_MAX
#include <cstdlib>
// std::vector
#include <vector>

#include "gtest/gtest.h"

#include "openmini/src/configuration.h"
#include "openmini/src/common.h"

// Using declarations for openmini stuff
using openmini::IGNORE;

static const unsigned int kDataTestSetSize(64 * 1024);
static const unsigned int kIterations(32);
static const unsigned int kSignalDataPeriodsCount(32);
/// @brief Arbitrary highest allowed fundamental
static const float kMaxFundamentalNorm(10000.0f / openmini::kSamplingRateHalf);
/// @brief Arbitrary lowest allowed key note (= C1)
static const unsigned int kMinKeyNote(4);
/// @brief Arbitrary highest allowed key note (= C8)
static const unsigned int kMaxKeyNote(88);

// Smaller performance test sets in debug
#if (CHTZ_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize(64 * 1024);
#else  // (CHTZ_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize(64 * 1024 * 1024);
#endif  // (CHTZ_CONFIGURATION_DEBUG)

/// @brief Generates normalized frequencies from an uniform random distribution
/// (naive implementation using std::rand)
class GeneratorNormFrequency {
 public:
  /// @brief Actual generation functor
  ///
  /// @return a random number in ] 0.0f ; kMaxFundamentalNorm [
  float operator()(void) const {
    return (std::fmod(static_cast<float>(std::rand()) / RAND_MAX,
                      kMaxFundamentalNorm));
  }
};

/// @brief Compute the mean value of a signal generator for the given length
///
/// @param[in]    generator      Generator to compute value from
/// @param[in]    length         Sample length
///
/// @return the generator mean for such length
template <typename TypeGenerator>
float ComputeMean(TypeGenerator generator, const unsigned int length) {
  float sum(0.0f);
  for (unsigned int i(0); i < length; ++i) {
    sum += generator();
  }
  return sum / static_cast<float>(length);
}

/// @brief Returns the sign of a given value - zero for zero input
template <typename TypeValue>
int sgn(const TypeValue val) {
  return (static_cast<TypeValue>(0) < val) - (val < static_cast<TypeValue>(0));
}

/// @brief Compute zero crossings of a signal generator for the given length
///
/// @param[in]    generator      Generator to compute value from
/// @param[in]    length         Sample length
///
/// @return zero crossings occurence for such length
template <typename TypeGenerator>
int ComputeZeroCrossing(TypeGenerator generator, const unsigned int length) {
  int zero_crossings(0);
  float previous(generator());
  for (unsigned int i(1); i < length; ++i) {
    const float current(generator());
    if (sgn<float>(previous) != sgn<float>(current)) {
      zero_crossings += 1;
    }
    previous = current;
  }
  return zero_crossings;
}

/// @brief Compute the frequency of a given piano key (A4 = 440Hz)
static inline float NoteToFrequency(const unsigned int key_number) {
  const float exponent((static_cast<float>(key_number) - 49.0f) / 12.0f);
  return std::pow(2.0f, exponent) * 440.0f;
}

#endif  // OPENMINI_TESTS_TESTS_H_
