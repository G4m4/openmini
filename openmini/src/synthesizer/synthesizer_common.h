/// @filename synthesizer_common.h
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

#ifndef OPENMINI_SRC_SYNTHESIZER_SYNTHESIZER_COMMON_H_
#define OPENMINI_SRC_SYNTHESIZER_SYNTHESIZER_COMMON_H_

#include <cmath>

#include "openmini/src/common.h"

namespace openmini {
namespace synthesizer {

/// @brief Floor the input value and convert it as the given type
///
/// @param[in] value     Value to convert
template <typename TypeOut, typename TypeIn>
TypeOut FloorAndConvert(const TypeIn value) {
  return static_cast<TypeOut>(std::floor(value));
}

/// @brief Ceil the input value and convert it as the given type
///
/// @param[in] value     Value to convert
template <typename TypeOut, typename TypeIn>
TypeOut CeilAndConvert(const TypeIn value) {
  return static_cast<TypeOut>(std::ceil(value));
}

/// @brief Round the input value to the nearest integer
///
/// @param[in] value     Value to convert
int Round(const float value);

/// @brief Check if the input value is an integer
///
/// @param[in] value     Value to test
///
/// @return True if the value is an integer
bool IsInteger(const float value);

/// @brief Check if the input value is a multiple of another
///
/// @param[in]  value       Value to test
/// @param[in]  multiple    Multiple to check for
bool IsMultipleOf(const unsigned int value, const unsigned int multiple);

/// @brief Compute the frequency of a given piano key (A4 = 440Hz)
float NoteToFrequency(const unsigned int key_number);

/// @brief Find the smallest multiple bigger than the given number
///
/// @param[in]   input     Number to be rounded from
/// @param[in]   multiple  Multiple to be rounded to
unsigned int GetNextMultiple(const unsigned int input,
                             const unsigned int multiple);


/// @brief Find the offset to add to the number to get it to the next
/// immediate multiple
///
/// @param[in]   input     Number to be rounded from
/// @param[in]   multiple  Multiple to be rounded to
unsigned int GetOffsetFromNextMultiple(const unsigned int input,
                                       const unsigned int multiple);

/// @brief Copy a buffer content into another
///
/// @param[in]  dest    Destination buffer
/// @param[in]  src   Source buffer
/// @param[in]  length    Count of elements to be copied (this is not the size!)
void CopyFloatArray(float* const dest,
                    const Sample* const src,
                    const unsigned int length);

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_SYNTHESIZER_COMMON_H_
