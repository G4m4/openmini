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
// std::vector
#include <vector>

#include "gtest/gtest.h"

#include "openmini/src/common.h"
#include "openmini/src/configuration.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

// Using declarations for openmini stuff
using openmini::IGNORE;
using openmini::kMinKeyNote;
using openmini::kMaxKeyNote;
using openmini::synthesizer::NoteToFrequency;

static const unsigned int kDataTestSetSize(64 * 1024);
static const unsigned int kIterations(32);
static const unsigned int kSignalDataPeriodsCount(32);
/// @brief Arbitrary highest allowed fundamental
static const float kMaxFundamentalNorm(10000.0f / openmini::kSamplingRateHalf);

// Smaller performance test sets in debug
#if (CHTZ_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize(64 * 1024);
#else  // (CHTZ_CONFIGURATION_DEBUG)
static const int kDataPerfSetSize(64 * 1024 * 1024);
#endif  // (CHTZ_CONFIGURATION_DEBUG)

/// @brief Generates normalized random floats from an uniform distribution
class GeneratorNormFloatRand {
 public:
  virtual ~GeneratorNormFloatRand() {
    // Nothing to do here for now
  };
  /// @brief Actual generation functor
  ///
  /// @return a random number in ] -1.0f ; 1.0f [
  float operator()(void) const;
};

/// @brief Generates a positive normalized floating point value
/// on a given interval > 0.0
class GeneratorRangedFloat : public GeneratorNormFloatRand {
 public:
  virtual ~GeneratorRangedFloat() {
    // Nothing to do here for now
  };
  /// @brief Constructor: parameterizes the generator output range
  ///
  /// @param[in]     min     Output range lower bound
  /// @param[out]    max     Output range upper bound
  GeneratorRangedFloat(const float min, const float max);
  /// @brief Actual generation functor
  ///
  /// @return a random number in ] min ; max [
  virtual float operator()(void) const;
 private:
  const float min_;
  const float max_;
};

/// @brief Generates a normalized integer value in a given interval
/// TODO(gm): this should not aggregate a generator...
class GeneratorRangedInteger  {
 public:
  virtual ~GeneratorRangedInteger() {
    // Nothing to do here for now
  };
  /// @brief Constructor: parameterizes the generator output range
  ///
  /// @param[in]     min     Output range lower bound
  /// @param[out]    max     Output range upper bound
  GeneratorRangedInteger(const int min, const int max);
  /// @brief Actual generation functor
  ///
  /// @return a random number in ] min ; max [
  virtual int operator()(void) const;
 private:
  GeneratorRangedFloat generator_;
};

/// @brief Generates normalized frequencies in ] 0.0f ; kMaxFundamentalNorm [
class GeneratorNormFrequency : public GeneratorRangedFloat {
 public:
  virtual ~GeneratorNormFrequency() {
    // Nothing to do here for now
  };
  GeneratorNormFrequency();
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

#endif  // OPENMINI_TESTS_TESTS_H_
