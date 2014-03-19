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
#include "openmini/src/samplingrate.h"

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
  static inline void ASSUME(const bool condition) {_assume(condition);}
#elif(_COMPILER_GCC)
  static inline void ASSUME(const bool condition) {if (!(condition)) __builtin_unreachable();}
#else
  #define ASSUME(_condition_)
#endif  // _COMPILER_ ?

/// @brief Asserts condition == true
#if(_BUILD_CONFIGURATION_DEBUG)
  static inline void ASSERT(const bool condition) {assert(condition);}
#else
  // Maps to "assume" in release configuration for better optimization
  static inline void ASSERT(const bool condition) {ASSUME(condition);}
#endif

// Fixed globals for synthesis

/// @brief Block size used for internal sound synthesis
/// may be different from the one actually delivered at the output of the synth
static const unsigned int kBlockSize = 64;

/// @brief Min filter frequency (due to precision issues)
static const float kMinFilterFreq = 10.0f / SamplingRate::Instance().Get();

/// @brief Max filter frequency (slightly lower than half the sampling
/// frequency, due to precision issues)
static const float kMaxFilterFreq = (SamplingRate::Instance().GetHalf() - 10.0f)
                                    / SamplingRate::Instance().Get();

/// @brief Arbitrary lowest allowed key note (= C1)
static const unsigned int kMinKeyNote(24);
/// @brief Arbitrary highest allowed key note (= C7)
static const unsigned int kMaxKeyNote(96);

/// @brief Arbitrary smallest allowed attack/decay/release time
static const unsigned int kMinTime(0);
/// @brief Arbitrary highest allowed attack/decay/release time
static const unsigned int kMaxTime(static_cast<unsigned int>(
                                     SamplingRate::Instance().Get()));

/// @brief Standard value for Pi
static const double Pi(3.14159265358979);

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

/// @brief Type for Sample parameter "read only":
/// It should be passed by value since it allows to keep it into a register,
/// instead of passing its address and loading it.
#if (_USE_SSE)
  #if (_COMPILER_MSVC)
    typedef const Sample SampleRead;
  #else
    typedef const Sample SampleRead;
  #endif
#else
  typedef const Sample SampleRead;
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
