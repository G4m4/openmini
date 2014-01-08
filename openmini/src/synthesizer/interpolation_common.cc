/// @filename interpolation_common.cc
/// @brief Common declarations for interpolation
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

#include "openmini/src/synthesizer/interpolation_common.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

LinearInterpolation::LinearInterpolation() {
  // Nothing to do here for now
}

float LinearInterpolation::operator()(const float* const data_cursor,
                                      const float ratio) const {
  const float left(*data_cursor);
  const float right(*(data_cursor + 1));
  return (right - left) * ratio + left;
}

unsigned int ExpectedOutLength(const unsigned int input_length,
                               const float ratio) {
  return CeilAndConvert<unsigned int>((static_cast<float>(input_length)
                                      / ratio));
}

unsigned int RequiredInLength(const unsigned int output_length,
                              const float ratio) {
  // What we actually want is at least 1 sample of advance, except of course
  // if out_length * ratio is an integer
  return CeilAndConvert<unsigned int>(ratio * output_length);
}

}  // namespace synthesizer
}  // namespace openmini
