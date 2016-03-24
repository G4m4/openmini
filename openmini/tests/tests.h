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
#include <array>
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

#include "soundtailor/tests/analysis.h"

// Using declarations for openmini stuff
using openmini::IGNORE;
using openmini::kMinKeyNote;
using openmini::kMaxKeyNote;
using openmini::SamplingRate;
using openmini::synthesizer::NoteToFrequency;
using openmini::synthesizer::GetNextMultiple;

// Using declarations for soundtailor tests utilities
using soundtailor::ComputeMean;
using soundtailor::ComputePower;
using soundtailor::ComputeZeroCrossing;
using soundtailor::IsContinuous;
using soundtailor::ZeroCrossing;

static const unsigned int kDataTestSetSize(32768);
static const unsigned int kIterations(16);
static const unsigned int kSignalDataPeriodsCount(32);
/// @brief Arbitrary lowest allowed fundamental
static const float kMinFundamentalNorm(10.0f / SamplingRate::Instance().Get());
/// @brief Arbitrary highest allowed fundamental
static const float kMaxFundamentalNorm(1000.0f
                                       / SamplingRate::Instance().Get());

// Smaller performance test sets in debug
#if (_BUILD_CONFIGURATION_DEBUG)
static const unsigned int kFilterDataPerfSetSize(16 * 1024);
static const unsigned int kGeneratorDataPerfSetSize(16 * 1024);
static const float kSynthesizerPerfSetLength(0.1f);
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
