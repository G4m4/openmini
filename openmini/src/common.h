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

// Expose Soundtailor types
#include "externals/soundtailor/soundtailor/src/common.h"

using soundtailor::BlockIn;
using soundtailor::BlockOut;
using soundtailor::Sample;
using soundtailor::SampleRead;
using soundtailor::SampleSize;
using soundtailor::SampleSizeBytes;

#include "openmini/src/configuration.h"
#include "openmini/src/samplingrate.h"

namespace openmini {

/// @brief Ignore unused variables
template<typename Type> void IGNORE(const Type&) {}

/// @brief Assume that the following condition is always true
/// (on some compilers, allows optimization)
#if(_COMPILER_MSVC)
  static inline void ASSUME(const bool condition) {__assume(condition);}
#elif(_COMPILER_GCC)
  static inline void ASSUME(const bool condition) {if (!(condition)) __builtin_unreachable();}
#else
  #define ASSUME(_condition_)
#endif  // _COMPILER_ ?

/// @brief Asserts condition == true
#if(_BUILD_CONFIGURATION_DEBUG)
  #define OPENMINI_ASSERT(_condition_) (assert((_condition_)))
#else
  // Maps to "assume" in release configuration for better optimization
  #define OPENMINI_ASSERT(_condition_) {::openmini::ASSUME((_condition_));}
#endif

#if(_COMPILER_MSVC)
  #define RESTRICT __restrict
#elif(_COMPILER_GCC)
  #define RESTRICT __restrict__
#endif  // _COMPILER_ ?

// Fixed globals for synthesis

/// @brief Block size used for internal sound synthesis
/// may be different from the one actually delivered at the output of the synth
static const unsigned int kBlockSize = 64;

/// @brief Arbitrary lowest allowed key note (= C0)
static const unsigned int kMinKeyNote(12);
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

/// @brief Allocation function wrapper
///
/// Allow aligned memory allocation
///
/// @param  size    Size of the memory to allocate, in bytes
static inline void* Allocate(const size_t size) {
#if _COMPILER_MSVC
  return _aligned_malloc(size, SampleSizeBytes);
#else
  #if _COMPILER_GCC_OLD
    // Posix stuff
    void* memory(nullptr);
    posix_memalign(&memory, SampleSizeBytes, size);
    OPENMINI_ASSERT(memory != nullptr);
    return memory;
  #else
    return aligned_alloc(SampleSizeBytes, size);
  #endif
#endif
}

/// @brief Deallocation function wrapper
///
/// Has to be called for each allocation performed with the above function
///
/// @param  memory    Pointer to the memory to be deallocated
static inline void Deallocate(void* memory) {
#if _COMPILER_MSVC
  _aligned_free(memory);
#else
  // C11
  free(memory);
#endif
}

}  // namespace openmini

#endif  // OPENMINI_SRC_COMMON_H_
