/// @filename samplingrate.cc
/// @brief SamplingRate singleton - simple holder for the sampling rate
/// @author gm
/// @copyright gm 2014
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

#include "openmini/src/samplingrate.h"
#include "openmini/src/common.h"

namespace openmini {

SamplingRate& SamplingRate::Instance(void) {
  static SamplingRate instance;
  return instance;
}

float SamplingRate::Get(void) const {
  return sampling_rate_;
}

float SamplingRate::GetHalf(void) const {
  return sampling_rate_ / 2.0f;
}

void SamplingRate::Set(const float sampling_rate) {
  ASSERT(sampling_rate > 0.0f);
  sampling_rate_ = sampling_rate;
}

SamplingRate::SamplingRate()
    : sampling_rate_(96000.0f) {
  // Nothing to do here for now
}

}  // namespace openmini
