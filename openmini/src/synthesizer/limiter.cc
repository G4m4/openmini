/// @filename limiter.cc
/// @brief Limiter object implementation
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

#include "openmini/src/common.h"
#include "openmini/src/maths.h"
#include "openmini/src/synthesizer/limiter.h"

namespace openmini {
namespace synthesizer {

Limiter::Limiter(const float threshold)
    : threshold_neg_(Fill(-threshold)),
      threshold_pos_(Fill(threshold)) {
  // Nothing to do here for now
}

Limiter::~Limiter() {
  // Nothing to do here for now
}

Sample Limiter::operator()(SampleRead input) {
  return Min(Max(input, threshold_neg_), threshold_pos_);
}

}  // namespace synthesizer
}  // namespace openmini
