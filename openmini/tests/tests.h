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
#include "openmini/src/samplingrate.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

using openmini::IGNORE;

// Using declarations for openmini maths stuff
using openmini::Sample;
using openmini::SampleRead;
using openmini::Fill;
using openmini::FillWithFloatGenerator;
using openmini::GetByIndex;
using openmini::Add;
using openmini::AddHorizontal;
using openmini::Sub;
using openmini::Mul;
using openmini::MulConst;
using openmini::Abs;
using openmini::Sgn;
using openmini::SgnNoZero;
using openmini::Store;
using openmini::Round;
using openmini::SamplingRate;

// Tests-specific maths (comparison operators) stuff

static inline bool IsMaskNull(SampleRead value) {
#if (_USE_SSE)
  return 0 == _mm_movemask_ps(value);
#else
  return value == 0.0f;
#endif
}

static inline bool IsMaskFull(SampleRead value) {
#if (_USE_SSE)
  return 15 == _mm_movemask_ps(value);
#else
  return value == 1.0f;
#endif
}

static inline bool GreaterThan(const float threshold, SampleRead value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmpgt_ps(Fill(threshold), value));
  return IsMaskFull(test_result);
#else
  return threshold > value;
#endif
}

static inline bool GreaterEqual(const float threshold, SampleRead value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmpge_ps(Fill(threshold), value));
  return IsMaskFull(test_result);
#else
  return threshold >= value;
#endif
}

#if (_USE_SSE)
static inline bool GreaterEqual(SampleRead threshold, SampleRead value) {
  const Sample test_result(_mm_cmpge_ps(threshold, value));
  return IsMaskFull(test_result);
}
#endif

static inline bool LessThan(const float threshold, SampleRead value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmplt_ps(Fill(threshold), value));
  return IsMaskFull(test_result);
#else
  return threshold < value;
#endif
}

static inline bool LessEqual(const float threshold, SampleRead value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmple_ps(Fill(threshold), value));
  return IsMaskFull(test_result);
#else
  return threshold <= value;
#endif
}

#if (_USE_SSE)
static inline bool LessEqual(SampleRead threshold, SampleRead value) {
  const Sample test_result(_mm_cmple_ps(threshold, value));
  return IsMaskFull(test_result);
}
#endif

static inline bool Equal(const float threshold, SampleRead value) {
#if (_USE_SSE)
  const Sample test_result(_mm_cmpeq_ps(Fill(threshold), value));
  return IsMaskFull(test_result);
#else
  return threshold == value;
#endif
}

#if (_USE_SSE)
static inline bool Equal(SampleRead threshold, SampleRead value) {
  const Sample test_result(_mm_cmpeq_ps(threshold, value));
  return IsMaskFull(test_result);
}
#endif

// Using declarations for openmini stuff
using openmini::IGNORE;
using openmini::kMinKeyNote;
using openmini::kMaxKeyNote;
using openmini::synthesizer::NoteToFrequency;
using openmini::synthesizer::FindImmediateNextMultiple;

static const unsigned int kDataTestSetSize(32768);
static const unsigned int kIterations(16);
static const unsigned int kSignalDataPeriodsCount(32);
/// @brief Arbitrary lowest allowed fundamental
static const float kMinFundamentalNorm(10.0f / SamplingRate::Instance().Get());
/// @brief Arbitrary highest allowed fundamental
static const float kMaxFundamentalNorm(1000.0f / SamplingRate::Instance().Get());

// Smaller performance test sets in debug
#if (_BUILD_CONFIGURATION_DEBUG)
static const unsigned int kFilterDataPerfSetSize(16 * 1024);
static const unsigned int kGeneratorDataPerfSetSize(16 * 1024);
static const float kSynthesizerPerfSetLength(1.0f);
#else  // (_BUILD_CONFIGURATION_DEBUG)
static const unsigned int kFilterDataPerfSetSize(16 * 1024 * 256);
static const unsigned int kGeneratorDataPerfSetSize(16 * 1024 * 256);
static const float kSynthesizerPerfSetLength(10.0f);
#endif  // (_BUILD_CONFIGURATION_DEBUG)

/// @brief Uniform distribution of normalized frequencies
/// in ] 0.0f ; kMaxFundamentalNorm [
class NormFrequencyDistribution : public std::uniform_real_distribution<float> {
 public:
  NormFrequencyDistribution();
  ~NormFrequencyDistribution() {
    // Nothing to do here for now
  }
};
static NormFrequencyDistribution kFreqDistribution;
static std::uniform_real_distribution<float> kNormDistribution(-1.0f, 1.0f);
static std::uniform_real_distribution<float> kNormPosDistribution(0.0f, 1.0f);
static std::bernoulli_distribution kBoolDistribution;
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

/// @brief Helper structure for retrieving zero crossings informations
template <typename TypeGenerator>
struct ZeroCrossing {
  explicit ZeroCrossing(TypeGenerator& generator)
      : generator_(generator),
        // TODO(gm): this may introduces an additional zero crossing,
        // it must be initialized to the first input value
        previous_sgn_(0.0f),
        cursor_(0) {
    // Nothing to do here
  }

  /// @brief Get next zero crossing absolute index
  unsigned int GetNextZeroCrossing(unsigned int max_length) {
    while (cursor_ < max_length) {
      int index_zc(GetZeroCrossingRelative(generator_()));
      if (index_zc >= 0) {
        const unsigned int out(index_zc + cursor_);
        cursor_ += openmini::SampleSize;
        return out;
      }
      cursor_ += openmini::SampleSize;
    }
    return max_length;
  }

  unsigned int Cursor(void) {
    return cursor_;
  }

 private:
  /// @brief Actual zero crossing detection method
  ///
  /// Beware, it cannot detect zero crossings closer than 4 samples!
  /// TODO(gm): Fix it
  ///
  /// @return the (relative) index of the next zero crossing, or -1
  int GetZeroCrossingRelative(Sample input) {
    const Sample sign_v(SgnNoZero(input));
    for (unsigned int index(0); index < openmini::SampleSize; index += 1) {
      const float current_sgn(GetByIndex(sign_v, index));
      if (previous_sgn_ != current_sgn) {
        previous_sgn_ = current_sgn;
        return index;
      }
      previous_sgn_ = current_sgn;
    }
    return -1;
  }

  TypeGenerator generator_;
  float previous_sgn_;
  unsigned int cursor_;
};

/// @brief Compute zero crossings of a signal generator for the given length
///
/// @param[in]    generator      Generator to compute value from
/// @param[in]    length         Sample length
///
/// @return zero crossings occurence for such length
template <typename TypeGenerator>
int ComputeZeroCrossing(TypeGenerator& generator, const unsigned int length) {
  ZeroCrossing<TypeGenerator> zero_crossing(generator);
  int out(0);
  unsigned int zero_crossing_idx(zero_crossing.GetNextZeroCrossing(length));
  while (zero_crossing_idx < length) {
    out += 1;
    zero_crossing_idx = zero_crossing.GetNextZeroCrossing(length);
  }
  return out;
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

/// @brief Basic sinus generator
///
/// No special care has been taken performance or quality-wise
class SinusGenerator {
 public:
  /// @brief Default constructor, the generated and sampling frequencies
  /// have to be provided
  ///
  /// @param[in]  freq    Generated sinus frequency
  /// @param[in]  sampling_freq    Generated signal sampling frequency
  SinusGenerator(const float freq, const float sampling_freq);
  ~SinusGenerator();

  /// @brief Actual processing method
  ///
  /// @return the next sample in order to create a sinus
  float operator()(void);

 private:
  // No assignment operator for this functor
  SinusGenerator& operator=(const SinusGenerator& right);

  const double increment_;
  double current_phase_;
};

/// @brief Round the input value to the given increment
Sample RoundToIncrement(SampleRead input, const float increment);

template <typename TypeValue>
struct IsClose {
  explicit IsClose(const TypeValue threshold)
     : threshold_(threshold) {
  }
  bool operator()(const TypeValue left, const TypeValue right) {
    return (right - left < threshold_);
  }

 private:
  // No assignment operator for this class
  IsClose& operator=(const IsClose& right);

  const TypeValue threshold_;
};

/// @brief Helper for removing close consecutive elements of a container
template <typename TypeContainer, typename TypeValue>
void RemoveClose(TypeContainer* container, const TypeValue threshold) {
  container->erase(std::unique(container->begin(),
                               container->end(),
                               IsClose<TypeValue>(threshold)),
                   container->end());
}

#endif  // OPENMINI_TESTS_TESTS_H_
