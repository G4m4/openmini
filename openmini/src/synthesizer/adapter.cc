/// @filename adapter.cc
/// @brief Combines a buffer and a resampler for audio format matching
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

#include "openmini/src/synthesizer/adapter.h"

#include "openmini/src/synthesizer/interpolation_common.h"

namespace openmini {
namespace synthesizer {

Adapter::Adapter(const unsigned int capacity)
    : RingBuffer(capacity),
      interpolator_() {
  // Nothing to do here for now
}

Adapter::~Adapter() {
  // Nothing to do here for now
}

void Adapter::Pop(float* dest, const unsigned int count) {
  const unsigned int actual_count(ComputeRequiredElements(count));

  return RingBuffer::Pop(dest, actual_count);
}

void Adapter::SetFrequencies(const float in_freq, const float out_freq) {
  interpolator_.SetRatio(out_freq / in_freq);
}

// TODO(gm): simplify this mess with a better vocabulary!

unsigned int Adapter::ComputeRequiredElements(const unsigned int size) const {
  return CeilAndConvert<unsigned int>(static_cast<float>(size)
                                      / ComputeOutputRatio());
}

unsigned int Adapter::ComputeCapacity(const unsigned int size) const {
  return CeilAndConvert<unsigned int>(ComputeInputRatio() * size);
}

float Adapter::ComputeInputRatio(void) const {
  return interpolator_.Ratio();
}

float Adapter::ComputeOutputRatio(void) const {
  return 1.0f / interpolator_.Ratio();
}

void Adapter::TransferData(float* const in_first,
                           float* const in_last,
                           float* const out_first) {
  // Instead of a simple copy the resampling is done here
  const unsigned int data_length(in_last - in_first);
  interpolator_.SetData(in_first, data_length);
  interpolator_.Reset();
  float* out_cursor(out_first);
  while (interpolator_.AnythingToReadFrom()) {
    *out_cursor = interpolator_.operator()<LinearInterpolation>();
    out_cursor += 1;
  }
}

}  // namespace synthesizer
}  // namespace openmini
