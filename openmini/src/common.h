/// @filename common.h
/// @brief OpenMini common utilities header
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

#ifndef OPENMINI_SRC_COMMON_H_
#define OPENMINI_SRC_COMMON_H_

#include <cassert>
#include <malloc.h>

#include <xmmintrin.h>

#include "openmini/src/configuration.h"

namespace openmini {

/// @brief Ignore unused variables
template<typename Type> void IGNORE(const Type&) {}

/// @brief Assume that the following condition is always true
/// (on some compilers, allows optimization)
#if(_COMPILER_MSVC)
  #define ASSUME(_condition_) do { __assume(_condition_); } while (0)
#elif(_COMPILER_GCC)
  #define ASSUME(_condition_) do { if (!(_condition_)) __builtin_unreachable(); } while (0)
#else
  #define ASSUME(_condition_)
#endif  // _COMPILER_ ?

/// @brief Asserts _condition_ == true
#if(_BUILD_CONFIGURATION_DEBUG)
  #define ASSERT(_condition_) assert(_condition_)
#else
  // Maps to "assume" in release configuration for better optimization
  #define ASSERT(_condition_) ASSUME(_condition_)
#endif

/// @brief Asserts _condition_ == true
#if defined(_FORBID_SIMD)
  #define _USE_SSE 0
#else
  #if ((_COMPILER_MSVC) || (_COMPILER_GCC))
    #define _USE_SSE 1
  #endif
#endif

// Fixed globals for synthesis

/// @brief Block size used for internal sound synthesis
/// may be different from the one actually delivered at the output of the synth
static const int kBlockSize = 64;

/// @brief Sampling rate used for internal sound synthesis
/// may be different from the one actually delivered at the output of the synth
static const float kSamplingRate = 96000.0f;

/// @brief Half of the above defined sampling rate : max reachable frequency
static const float kSamplingRateHalf = kSamplingRate / 2.0f;

/// @brief Arbitrary lowest allowed key note (= C1)
static const unsigned int kMinKeyNote(24);
/// @brief Arbitrary highest allowed key note (= C7)
static const unsigned int kMaxKeyNote(96);

// (Using the "enum in its own namespace" trick)
/// @brief Allowed Waveform for all generators
namespace Waveform {
enum Type {
  kTriangle = 0,
  kSawtooth,
  kCount
};
}  // namespace Waveform

/// @brief "Sample" type - actually, this is the data computed at each "tick";
/// If using vectorization it may longer than 1 audio sample
#if (_USE_SSE)
  typedef __m128 Sample;
  #define ALIGN __declspec(align(16))
#else
  typedef float Sample;
  #define ALIGN
#endif  // (_USE_SSE)

/// @brief "Sample" type size in bytes
static const int SampleSizeBytes(sizeof(Sample));
/// @brief "Sample" type size compared to audio samples
/// (e.g., if Sample == float, SampleSize = 1)
static const int SampleSize(sizeof(Sample) / sizeof(float));


static inline void* AllocateAligned(const size_t size) {
  return _aligned_malloc(size, SampleSizeBytes);
}

static inline void DeallocateAligned(void* memory) {
  return _aligned_free(memory);
}

}  // namespace openmini

#endif  // OPENMINI_SRC_COMMON_H_
