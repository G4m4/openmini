/// @filename interpolator.h
/// @brief Generic interpolator object
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

#ifndef OPENMINI_SRC_SYNTHESIZER_INTERPOLATOR_H_
#define OPENMINI_SRC_SYNTHESIZER_INTERPOLATOR_H_

#include "openmini/src/common.h"

namespace openmini {
namespace synthesizer {

/// @brief Interpolator class
///
/// Given a given interpolation function (at compilation time)
/// and a given ratio (at runtime), interpolates from the given input data.
///
/// This class provides utilities for the interpolation of non-contiguous
/// blocks into a contiguous output.
class Interpolator {
 public:
  Interpolator();
  ~Interpolator();

  /// @brief Resampling ratio setter
  ///
  /// @param[in]     ratio       Resampling ratio: >1 means upsampling
  void SetRatio(const float ratio);

  /// @brief Resampling ratio getter
  float Ratio(void) const;

  /// @brief Resampling internal cursor position getter
  double CursorPos(void) const;

  /// @brief Interpolation function
  ///
  /// Based on previously set parameters (ratio, etc.), retrieve exactly
  /// "output_length" elements from the given input data. The input data is
  /// supposed to be made up of enough elements
  /// (e.g. at least output_length * ratio)
  ///
  /// @param[in]  input   Input data
  /// @param[in]  input_length   Input data length
  /// @param[out] output    Output data
  /// @param[in]  output_length   Output data length
  void Process(const float* const input,
               const unsigned int input_length,
               float* const output,
               const unsigned int output_length);

  /// @brief Reset internal data pointer to the given cursor position
  ///
  /// @param[in]  cursor_pos   New value to set the cursor position to
  void Reset(const double cursor_pos = 0.0);

 private:
  // No assignment operator for this class
  Interpolator& operator=(const Interpolator& right);

  /// @brief Actual processing function
  ///
  /// The interpolation function has to be chosen at the compilation
  ///
  /// @return one sample for the previously given parameters and input data
  template <typename TypeFunctor>
  float operator()(const float context[2], const float cursor) {
    const TypeFunctor interpolation_func;
    return interpolation_func(context, cursor);
  }

  double cursor_pos_;  ///< Actual cursor position relative to the beginning
                      ///< of the data cursor
  float ratio_;  ///< Interpolation ratio
  float history_[2];  ///< Interpolation history
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_INTERPOLATOR_H_
