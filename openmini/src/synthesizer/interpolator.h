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
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

/// @brief Interpolator class
///
/// Given a given interpolation function (at compilation time)
/// and a given ratio (at runtime), interpolates from the given input data
class Interpolator {
 public:
  Interpolator();
  virtual ~Interpolator();

  /// @brief Actual processing function
  ///
  /// The interpolation function has to be chosen at the compilation
  ///
  /// @return one sample for the previously given parameters and input data
  template <typename TypeFunctor>
  Sample operator()(void) {
    const TypeFunctor interpolation_func;
    const int left_idx(ConvertFloorAsInteger(static_cast<float>(cursor_pos_)));
    const float cursor(static_cast<float>(cursor_pos_) - static_cast<float>(left_idx));
    cursor_pos_ += static_cast<double>(ratio_);
    return interpolation_func(&data_[left_idx], cursor);
  }

  /// @brief Resampling ratio setter
  ///
  /// @param[in]     ratio       Resampling ratio: >1 means upsampling
  virtual void SetRatio(const float ratio);

  /// @brief Input data setter
  ///
  /// Note that the internal data pointer will not be reseted, @see Reset
  ///
  /// @param[in]  data_cursor   Cursor to the data to interpolate from
  virtual void SetData(float* data_cursor,
                       const unsigned int data_length);

  /// @brief Check if there is still data to interpolate from
  ///
  /// @return true if all the input data, given the current parameters,
  /// has not been entirely read
  virtual bool AnythingToReadFrom(void) const;

  /// @brief Reset internal data pointer
  virtual void Reset(void);

 private:
  friend unsigned int ExpectedLength(const Interpolator& interpolator);
  // No assignment operator for this class
  Interpolator& operator=(const Interpolator& right);

  double cursor_pos_;  ///< Actual cursor position relative to the beginning
                      ///< of the data cursor
  float ratio_;  ///< Resampling ratio
  unsigned int data_length_;  ///< Length of the input data
  float* data_;  ///< Pointer to the data being interpolated from
};

/// @brief Based on current parameters,
/// retrieve the expected output signal length
unsigned int ExpectedLength(const Interpolator& interpolator);

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_INTERPOLATOR_H_
