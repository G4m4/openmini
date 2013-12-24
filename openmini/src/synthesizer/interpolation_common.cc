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

namespace openmini {
namespace synthesizer {

float LinearInterpolation::operator()(const float* const data_cursor,
                                      const float ratio) const {
  const float left(*data_cursor);
  const float right(*(data_cursor + 1));
  return (right - left) * ratio + left;
}

}  // namespace synthesizer
}  // namespace openmini
