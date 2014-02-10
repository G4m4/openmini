/// @filename resampler.cc
/// @brief Implementation of a resampler buffer
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

#include <algorithm>

#include "openmini/src/synthesizer/resampler.h"

#include "openmini/src/synthesizer/interpolation_common.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

/// @brief Default temporary buffer length
static const unsigned int kDefaultMemoryLength(4096);

Resampler::Resampler()
    : RingBuffer(kDefaultMemoryLength),
      interpolator_(),
      internal_buf_(kDefaultMemoryLength) {
  // Nothing to do here for now
}

Resampler::~Resampler() {
  // Nothing to do here for now
}

void Resampler::Pop(float* dest, const unsigned int count) {
  ASSERT(IsGood());
  ASSERT(count > 0);

  // First we pop all required data
  std::fill(internal_buf_.begin(), internal_buf_.end(), 0.0f);
  const unsigned int kRequiredSize(RequiredInLength(count,
                                   interpolator_.Ratio()));
  RingBuffer::Pop(&internal_buf_[0], kRequiredSize);
  // Then we interpolate it
  interpolator_.Process(&internal_buf_[0],
                        kRequiredSize,
                        dest,
                        count);
}

void Resampler::ResizeIfNeedBe(const unsigned int size) {
  // Check internal temporary buffer length
  const unsigned int kRequiredSize(
    FindImmediateNextMultiple(RequiredInLength(size, interpolator_.Ratio()),
                              openmini::kBlockSize));
  if (internal_buf_.size() < kRequiredSize) {
    internal_buf_.resize(kRequiredSize);
  }
  // Check internal ringbuffer length
  RingBuffer::ResizeIfNeedBe(kRequiredSize);
}

unsigned int Resampler::Size(void) const {
  return ExpectedOutLength(RingBuffer::Size(), interpolator_.Ratio());
}

void Resampler::SetFrequencies(const float in_freq, const float out_freq) {
  ASSERT(in_freq > 0.0f);
  ASSERT(out_freq > 0.0f);
  ASSERT(in_freq <= openmini::kSamplingRate);
  ASSERT(out_freq <= openmini::kSamplingRate);

  interpolator_.SetRatio(in_freq / out_freq);
}

}  // namespace synthesizer
}  // namespace openmini
