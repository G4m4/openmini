/// @filename interpolator.cc
/// @brief Generic interpolator object - implementation
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

#include <cmath>

#include "openmini/src/common.h"
#include "openmini/src/synthesizer/interpolator.h"
#include "openmini/src/synthesizer/interpolation_common.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

Interpolator::Interpolator()
    : cursor_pos_(0.0),
      ratio_(1.0f),
      history_(0.0f) {
  // Nothing to do here for now
}

Interpolator::~Interpolator() {
    // Nothing to do here for now
}

void Interpolator::SetRatio(const float ratio) {
  ASSERT(ratio > 0.0f);
  ratio_ = ratio;
}

float Interpolator::Ratio(void) const {
  return ratio_;
}

void Interpolator::Process(const float* const input,
                           const unsigned int input_length,
                           float* const output,
                           const unsigned int output_length) {
  ASSERT(input != nullptr);
  ASSERT(input_length >= RequiredInLength(output_length, ratio_));
  ASSERT(output != nullptr);
  ASSERT(output_length > 0);

  unsigned int current_out_idx(0);
  double temp_cursor(cursor_pos_);
  float context[2];
  while (current_out_idx < output_length) {
    // Default case: all is good,
    // far from the beginning and from the end of the input data
    unsigned int left_idx(FloorAndConvert<unsigned int>(temp_cursor));
    float current_cursor(static_cast<float>(temp_cursor)
                         - static_cast<float>(left_idx));
    context[0] = input[left_idx];
    context[1] = input[left_idx + 1];
    if (temp_cursor < 0.0) {
      // Should never happen
      ASSERT(temp_cursor < -1.0);
      // Being "late" relative to the cursor, e.g.:
      //
      // -----x----------------------|-----------
      //    cursor                 input[0]
      //      |< abs(current_cursor) >|
      left_idx = 0;
      current_cursor += 1.0f;
      context[0] = history_;
      context[1] = input[left_idx];
    }
    if (left_idx == input_length - 1) {
      // Cursor on (or after, because of the floor above) the last input sample
      context[0] = input[left_idx];
      context[1] = input[left_idx];
    }
    // Should never happen
    ASSERT(left_idx != input_length);

    output[current_out_idx]
      = this->operator()<LinearInterpolation>(context, current_cursor);

    current_out_idx += 1;
    temp_cursor += static_cast<double>(ratio_);
  }

  history_ = context[1];
  cursor_pos_ += static_cast<double>(ratio_) * current_out_idx;
  cursor_pos_ -= static_cast<double>(input_length);
}

void Interpolator::Reset(const double cursor_pos) {
  cursor_pos_ = cursor_pos;
}

}  // namespace synthesizer
}  // namespace openmini
