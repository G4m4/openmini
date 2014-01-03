/// @filename ringbuffer.h
/// @brief Implementation of a simple resizable ringbuffer
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

#ifndef OPENMINI_SRC_SYNTHESIZER_RINGBUFFER_H_
#define OPENMINI_SRC_SYNTHESIZER_RINGBUFFER_H_

#include "openmini/src/common.h"

namespace openmini {
namespace synthesizer {

/// @brief Simple implementation of a circular buffer
///
/// Resizable, FIFO-type container; its general philosophy is that,
/// if one operation could not be done (pushing too much data, etc.)
/// it asserts - there are no return values nor exceptions.
///
/// It is asymmetrical: you push in one Sample at a time, and you pop out
/// as many floats as you want.
class RingBuffer {
 public:
  /// @brief Default constructor: the user may provide a buffer length
  explicit RingBuffer(const unsigned int capacity = 1);
  ~RingBuffer();

  /// @brief Pop elements out of the buffer
  ///
  /// Output may be zero-padded if more elements are poped than those available
  ///
  /// @param[out]   dest          Buffer to store the elements into
  /// @param[in]    count         Elements count to retrieve
  void Pop(float* dest, const unsigned int count);

  /// @brief Push elements into the buffer
  ///
  /// Specialization for custom Sample type: this one is optimized
  ///
  /// @param[in]   src        Sampel to push
  void Push(SampleRead value);

  /// @brief Explicitly clear buffer content but does not deallocate it
  void Clear(void);

  /// @brief Change the available count of elements within the buffer
  ///
  /// Previously stored data will be copied in it as much as possible;
  /// in case of room left it will be filled with zeroes
  ///
  /// @param[in]    capacity    New capacity of the container
  void Resize(const unsigned int capacity);

  /// @brief Returns true if the buffer is "usable"
  ///
  /// For now, this means that some memory is allocated
  bool IsGood(void) const;

  /// @brief How many elements may be pushed into the buffer
  unsigned int capacity(void) const;

  /// @brief How many elements may be popped from the buffer
  unsigned int size(void) const;

 private :
  float* data_;  ///< Internal elements buffer
  unsigned int capacity_;  ///< Internal buffer length
  unsigned int size_;  ///< Count of elements currently held within the buffer
  unsigned int writing_position_;  ///< Beginning of the writing part
  unsigned int reading_position_;  ///< Beginning of the reading part
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_RINGBUFFER_H_
