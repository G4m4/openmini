/// @filename adapter.h
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

#ifndef OPENMINI_SRC_SYNTHESIZER_ADAPTER_H_
#define OPENMINI_SRC_SYNTHESIZER_ADAPTER_H_

#include "openmini/src/synthesizer/interpolator.h"
#include "openmini/src/synthesizer/ringbuffer.h"

namespace openmini {
namespace synthesizer {

/// @brief Audio format adapter
///
/// This object combines a buffer for block size matching, and a resampler
/// for sampling rate matching: it is adapted for audio format matching,
/// for instance at the output of a complete audio chain.
///
/// It is asymmetrical the way the buffer it inherits from is assymetrical:
/// Pushing is quite efficient and straightforward, one sample at a time,
/// without any post-process. Popping is best made by large blocks since
/// it is where resampling happens.
class Adapter : public RingBuffer {
 public:
  /// @brief Default constructor: the user may provide a buffer length
  explicit Adapter(const unsigned int capacity = 1);
  virtual ~Adapter();

  /// @brief Pop elements out of the buffer,
  /// at the pre-given output sampling frequency
  ///
  /// Output may be zero-padded if more elements are poped than those available.
  ///
  /// This is where all sampling frequency matching happens,
  /// so it may take some time!
  ///
  /// @param[out]   dest          Buffer to store the elements into
  /// @param[in]    count         Elements count to retrieve
  virtual void Pop(float* dest, const unsigned int count);

  /// @brief Set input/output sampling frequencies
  ///
  /// @param[in]  in_freq    Input sampling frequency
  /// @param[in]  out_freq    Output sampling frequency
  virtual void SetFrequencies(const float in_freq, const float out_freq);

 protected:
  /// @brief Compute required elements to be taken in order to output
  /// at least "size" elements
  ///
  /// @param[in]  size   Minimal amout of elements to be retrieved
  virtual unsigned int ComputeRequiredElements(const unsigned int size) const;

  /// @brief Compute required internal capacity in order to be able to output
  /// at least "size" elements
  ///
  /// @param[in]  size   Minimal amout of elements to be retrieved
  virtual unsigned int ComputeCapacity(const unsigned int size) const;

  /// @brief Transfer the data from input to output
  ///
  /// An additional resampling process is done in here
  ///
  /// @param[in]  in_first, in_last    Range of elements to transfer
  /// @param[in]  out_first   Beginnning of the output range
  virtual void TransferData(const float* const in_first,
                            const float* const in_last,
                            float* const out_first);

 private:
  /// @brief Based on input/output sampling frequencies,
  /// compute the current resampling ratios
  float ComputeInputRatio(void) const;
  float ComputeOutputRatio(void) const;

  Interpolator interpolator_;  ///< Internal resampler for frequencies matching
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_ADAPTER_H_
