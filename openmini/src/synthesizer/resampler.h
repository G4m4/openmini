/// @filename resampler.h
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

#ifndef OPENMINI_SRC_SYNTHESIZER_RESAMPLER_H_
#define OPENMINI_SRC_SYNTHESIZER_RESAMPLER_H_

#include <vector>

#include "openmini/src/common.h"
#include "openmini/src/synthesizer/interpolator.h"
#include "openmini/src/synthesizer/ringbuffer.h"

namespace openmini {
namespace synthesizer {

/// @brief Implementation of a resampler buffer
///
/// This is a special case of RingBuffer, in that it is able to resample
/// the popped data at any given output sampling frequency
class Resampler: public RingBuffer {
 public:
  /// @brief Default constructor
  explicit Resampler();
  ~Resampler();

  /// @brief Pop elements out of the buffer
  ///
  /// Output may be zero-padded if more elements are poped than those available
  ///
  /// @param[out]   dest          Buffer to store the elements into
  /// @param[in]    count         Elements count to retrieve
  void Pop(float* dest, const unsigned int count);

  /// @brief Check if the buffer is big enough to store and later retrieve
  /// at least "size" elements
  ///
  /// Resize if this is not the case.
  void ResizeIfNeedBe(const unsigned int size);

  /// @brief How many elements may be popped from the buffer
  unsigned int Size(void) const;

  /// @brief Change the input/output sampling frequencies
  ///
  /// This may lead to allocation
  ///
  /// @param[in]  in_freq    New input sampling frequency
  /// @param[in]  out_freq    New output sampling frequency
  void SetFrequencies(const float in_freq, const float out_freq);

 private:
  Interpolator interpolator_;  ///< Internal resampler for frequencies matching
  std::vector<float> internal_buf_;
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_RESAMPLER_H_
