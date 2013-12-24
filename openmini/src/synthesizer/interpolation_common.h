/// @filename interpolation_common.h
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

#ifndef OPENMINI_SRC_SYNTHESIZER_COMMON_INTERPOLATION_H_
#define OPENMINI_SRC_SYNTHESIZER_COMMON_INTERPOLATION_H_

#include "openmini/src/common.h"

namespace openmini {
namespace synthesizer {

/// @brief Interpolation functor interface
///
/// To be inherited by all interpolation functors
class InterpolationInterface {
 public:
  Sample operator()(const float* const data_cursor,
                    const float ratio) const;
};

/// @brief Simple linear interpolation
class LinearInterpolation : public InterpolationInterface {
 public:
  /// @brief Actual process function for one sample
  ///
  /// --------|--------------x--|-----------
  ///       data_cursor     data_cursor + 1
  ///         |<    ratio   >|
  ///
  /// @param[in]  data_cursor   Cursor to the beginning of the data
  /// @param[out] ratio    Location of the required interpolated value,
  ///                      relative to data_cursor
  ///
  /// @return The interpolated value
  Sample operator()(const float* const data_cursor,
                    const float ratio) const;
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_COMMON_INTERPOLATION_H_
