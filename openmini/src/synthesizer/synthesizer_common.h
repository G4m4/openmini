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

#include "openmini/src/common.h"

namespace openmini {
namespace synthesizer {

/// @brief Floor the input value and convert it as an integer
///
/// @param[in] value     Value to convert
int ConvertFloorAsInteger(const float value);

/// @brief Check if the input value is an integer
///
/// @param[in] value     Value to test
///
/// @return True if the value is an integer
bool IsInteger(const float value);

/// @brief Compute the frequency of a given piano key (A4 = 440Hz)
float NoteToFrequency(const unsigned int key_number);

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_SYNTHESIZER_COMMON_H_
