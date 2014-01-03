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

namespace openmini {
namespace synthesizer {

Interpolator::Interpolator()
    : cursor_pos_(0.0),
      ratio_(1.0f),
      data_length_(0),
      data_(nullptr) {
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

void Interpolator::SetData(float* const data_cursor,
                           const unsigned int data_length) {
  ASSERT(data_cursor != nullptr);
  ASSERT(data_length > 0);
  data_ = data_cursor;
  data_length_ = data_length;
}

bool Interpolator::AnythingToReadFrom(void) const {
  return cursor_pos_ <= (static_cast<float>(data_length_) - 1.0f);
}

void Interpolator::Reset(void) {
  cursor_pos_ = 0.0f;
}

unsigned int ExpectedLength(const Interpolator& interpolator) {
  return CeilAndConvert<unsigned int>(
    (static_cast<float>(interpolator.data_length_) / interpolator.ratio_));
}

}  // namespace synthesizer
}  // namespace openmini
