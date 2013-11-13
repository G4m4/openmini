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
// std::bind
#include <functional>
#include <random>
#include <vector>

#include "gtest/gtest.h"

#include "openmini/src/common.h"
#include "openmini/src/configuration.h"
#include "openmini/src/maths.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

// Using declarations for openmini maths stuff
using openmini::Sample;
using openmini::Fill;
using openmini::GetByIndex;
using openmini::Add;
using openmini::AddHorizontal;
using openmini::Sub;
using openmini::Mul;
using openmini::MulConst;
using openmini::Abs;
using openmini::Sgn;
using openmini::Store;

// Tests-specific maths (comparison operators) stuff

static inline bool IsMaskNull(const Sample& value) {
#if (_USE_SSE)
  return 0 == _mm_movemask_ps(value);
#else
  return value == 0.0f;
#endif
}

static inline bool GreaterThan(const float threshold, const Sample& value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmpgt_ps(Fill(threshold), value));
  return !IsMaskNull(test_result);
#else
  return threshold > value;
#endif
}

static inline bool GreaterEqual(const float threshold, const Sample& value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmpge_ps(Fill(threshold), value));
  return !IsMaskNull(test_result);
#else
  return threshold >= value;
#endif
}

static inline bool LessThan(const float threshold, const Sample& value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmplt_ps(Fill(threshold), value));
  return !IsMaskNull(test_result);
#else
  return threshold < value;
#endif
}

static inline bool LessEqual(const float threshold, const Sample& value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmple_ps(Fill(threshold), value));
  return !IsMaskNull(test_result);
#else
  return threshold <= value;
#endif
}

static inline bool Equal(const float threshold, const Sample& value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmpeq_ps(Fill(threshold), value));
  return !IsMaskNull(test_result);
#else
  return threshold == value;
#endif
}

// Using declarations for openmini stuff
using openmini::IGNORE;
using openmini::kMinKeyNote;
using openmini::kMaxKeyNote;
using openmini::synthesizer::NoteToFrequency;
using openmini::synthesizer::FindImmediateNextMultiple;

static const unsigned int kDataTestSetSize(32768);
static const unsigned int kIterations(16);
static const unsigned int kSignalDataPeriodsCount(32);
/// @brief Arbitrary highest allowed fundamental
static const float kMaxFundamentalNorm(10000.0f / openmini::kSamplingRate);

// Smaller performance test sets in debug
#if (_BUILD_CONFIGURATION_DEBUG)
static const unsigned int kDataPerfSetSize(16 * 1024);
static const unsigned int kSynthesizerPerfSetSize(
  static_cast<unsigned int>(openmini::kSamplingRate));
#else  // (_BUILD_CONFIGURATION_DEBUG)
static const unsigned int kDataPerfSetSize(16 * 1024 * 256);
static const unsigned int kSynthesizerPerfSetSize(
  static_cast<unsigned int>(openmini::kSamplingRate * 10.0f));
#endif  // (_BUILD_CONFIGURATION_DEBUG)

/// @brief Uniform distribution of normalized frequencies
/// in ] 0.0f ; kMaxFundamentalNorm [
class NormFrequencyDistribution : public std::uniform_real_distribution<float> {
 public:
  NormFrequencyDistribution();
  ~NormFrequencyDistribution() {
    // Nothing to do here for now
  };
};
static const NormFrequencyDistribution kFreqDistribution;
static const std::uniform_real_distribution<float> kNormDistribution;
static std::default_random_engine kRandomGenerator;

/// @brief Compute the mean value of a signal generator for the given length
///
/// @param[in]    generator      Generator to compute value from
/// @param[in]    length         Sample length
///
/// @return the generator mean for such length
template <typename TypeGenerator>
float ComputeMean(TypeGenerator& generator, const unsigned int length) {
  Sample sum(Fill(0.0f));
  unsigned int sample_idx(0);
  while (sample_idx < length) {
    const Sample sample(generator());
    sum = Add(sum, sample);
    sample_idx += openmini::SampleSize;
  }
  return AddHorizontal(sum) / static_cast<float>(length);
}

/// @brief Compute the mean power of a signal generator for the given length
///
/// @param[in]    generator      Generator to compute value from
/// @param[in]    length         Sample length
///
/// @return the generator mean for such length
template <typename TypeGenerator>
float ComputePower(TypeGenerator& generator, const unsigned int length) {
  Sample power(Fill(0.0f));
  unsigned int sample_idx(0);
  while (sample_idx < length) {
    const Sample sample(generator());
    const Sample squared(Mul(sample, sample));
    power = Add(power, squared);
    sample_idx += openmini::SampleSize;
  }
  return AddHorizontal(power) / static_cast<float>(length);
}

/// @brief Compute zero crossings of a signal generator for the given length
///
/// @param[in]    generator      Generator to compute value from
/// @param[in]    length         Sample length
///
/// @return zero crossings occurence for such length
template <typename TypeGenerator>
int ComputeZeroCrossing(TypeGenerator& generator, const unsigned int length) {
  int zero_crossings(0);
  float previous_sgn(0.0f);
  for (unsigned int i(0); i < length; i += openmini::SampleSize) {
    const Sample current_sgn_v(Sgn(generator()));
    for (unsigned int index(0); index < openmini::SampleSize; index += 1) {
      const float current_sgn = GetByIndex(current_sgn_v, index);
      if (previous_sgn != current_sgn) {
        zero_crossings += 1;
      }
      previous_sgn = current_sgn;
    }
  }
  return zero_crossings;
}

/// @brief Basic click detection using derivative
///
/// @param[in]    buffer         Buffer to test
/// @param[in]    length         Buffer length
/// @param[in]    epsilon        Max allowed factor compared to derivative mean
///                              (> 1.0)
///
/// @return true as soon as a click was found
bool ClickWasFound(const float* buffer,
                   const unsigned int length,
                   const float epsilon);

#endif  // OPENMINI_TESTS_TESTS_H_
