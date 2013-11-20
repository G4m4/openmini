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
// size_t
#include <cstddef>
// aligned_alloc
#include <cstdlib>

#include "openmini/src/configuration.h"

// Flag _USE_SSE is defined in configuration.h, that's why this include
// is done after the configuration.h include
#if (_USE_SSE)
extern "C" {
#include <xmmintrin.h>
}
#endif  // (_USE_SSE)

namespace openmini {

/// @brief Ignore unused variables
template<typename Type> void IGNORE(const Type&) {}

/// @brief Assume that the following condition is always true
/// (on some compilers, allows optimization)
#if(_COMPILER_MSVC)
  #define ASSUME(_condition_) _assume((_condition_));
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
#else
  typedef float Sample;
#endif  // (_USE_SSE)

/// @brief Attribute for structures alignment
#if (_USE_SSE)
  #if (_COMPILER_MSVC)
    #define ALIGN __declspec(align(16))
  #else
    #define ALIGN __attribute__((aligned(16)))
  #endif
#else
  #define ALIGN
#endif  // (_USE_SSE)

/// @brief "Sample" type size in bytes
static const unsigned int SampleSizeBytes(sizeof(Sample));
/// @brief "Sample" type size compared to audio samples
/// (e.g., if Sample == float, SampleSize = 1)
static const unsigned int SampleSize(sizeof(Sample) / sizeof(float));

/// @brief Allocation function wrapper
///
/// Allow aligned memory allocation
///
/// @param  size    Size of the memory to allocate, in bytes
static inline void* Allocate(const size_t size) {
#if _USE_SSE

#if _COMPILER_MSVC
  return _aligned_malloc(size, SampleSizeBytes);
#else
  #if _COMPILER_GCC_OLD
    // Posix stuff
    void* memory(nullptr);
    posix_memalign(&memory, SampleSizeBytes, size);
    ASSERT(memory != nullptr);
    return memory;
  #else
    return aligned_alloc(SampleSizeBytes, size);
  #endif
#endif

#else  // _USE_SSE

  return malloc(size);
#endif  // _USE_SSE
}

/// @brief Deallocation function wrapper
///
/// Has to be called for each allocation performed with the above function
///
/// @param  memory    Pointer to the memory to be deallocated
static inline void Deallocate(void* memory) {
#if _USE_SSE

#if _COMPILER_MSVC
  _aligned_free(memory);
#else
  // C11
  free(memory);
#endif

#else  // _USE_SSE

  free(memory);

#endif  // _USE_SSE
}

}  // namespace openmini

#endif  // OPENMINI_SRC_COMMON_H_
