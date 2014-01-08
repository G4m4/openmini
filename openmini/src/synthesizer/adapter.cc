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
  interpolator_.SetRatio(in_freq / out_freq);
}

// TODO(gm): simplify this mess with a better vocabulary!

unsigned int Adapter::ComputeRequiredElements(const unsigned int size) const {
  return RequiredInLength(size, Ratio());
}

unsigned int Adapter::ComputeMaxElements(const unsigned int size) const {
  return ExpectedOutLength(size, Ratio());
}

float Adapter::Ratio(void) const {
  return interpolator_.Ratio();
}

void Adapter::TransferData(const float* const in_first,
                           const float* const in_last,
                           float* const out_first) {
  // Instead of a simple copy the resampling is done here
  const unsigned int in_length(in_last - in_first);
  if (in_length > 0) {
    interpolator_.Process(in_first,
                          in_length,
                          out_first,
                          ExpectedOutLength(in_length, Ratio()));
  }
}

}  // namespace synthesizer
}  // namespace openmini
