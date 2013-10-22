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
static const unsigned int kMinKeyNote(4);
/// @brief Arbitrary highest allowed key note (= C8)
static const unsigned int kMaxKeyNote(88);

// (Using the "enum in its own namespace" trick)
/// @brief Allowed Waveform for all generators
namespace Waveform {
enum Type {
  kTriangle = 0,
  kMaxCount
};
}  // namespace Waveform

}  // namespace openmini

#endif  // OPENMINI_SRC_COMMON_H_
