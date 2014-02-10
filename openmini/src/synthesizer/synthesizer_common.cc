/// @filename synthesizer_common.cc
/// @brief OpenMini common stuff for synthesizer classes - declarations
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

#include "openmini/src/synthesizer/synthesizer_common.h"

// memcpy()
#include <cstring>

namespace openmini {
namespace synthesizer {

int Round(const float value) {
  return static_cast<int>(std::floor(value + 0.5f));
}

bool IsInteger(const float value) {
  return std::floor(value) == value;
}

bool IsMultipleOf(const unsigned int value, const unsigned int multiple) {
  ASSERT(multiple > 0);

  return value % multiple == 0;
}

float NoteToFrequency(const unsigned int key_number) {
  const float exponent((static_cast<float>(key_number) - 49.0f) / 12.0f);
  return std::pow(2.0f, exponent) * 440.0f;
}

unsigned int FindImmediateNextMultiple(const unsigned int input,
                                       const unsigned int multiple) {
  ASSERT(multiple != 0);
  ASSERT(input != 0);
  const int remainder(input % multiple);
  if (0 == remainder) {
    return input;
  }
  return input + multiple - remainder;
}

unsigned int GetOffsetFromNextMultiple(const unsigned int input,
                                       const unsigned int multiple) {
  return input % multiple;
}

void CopyFloatArray(float* const dest,
                    const Sample* const src,
                    const unsigned int length) {
  std::memcpy(dest,
              reinterpret_cast<const float* const>(src),
              length * sizeof(*dest));
}

}  // namespace synthesizer
}  // namespace openmini
