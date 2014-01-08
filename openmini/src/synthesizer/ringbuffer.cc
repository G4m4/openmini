/// @filename ringbuffer.cc
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

// std::fill, std::copy, std::min, std::max
#include <algorithm>

#include "openmini/src/maths.h"
#include "openmini/src/synthesizer/ringbuffer.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

RingBuffer::RingBuffer(const unsigned int capacity)
    : data_(nullptr),
      capacity_(FindImmediateNextMultiple(capacity, openmini::SampleSize)),
      size_(0),
      writing_position_(0),
      reading_position_(0) {
  ASSERT(capacity > 0);
  data_ = static_cast<float*>(Allocate(capacity_ * sizeof(*data_)));
  std::fill(&data_[0],
            &data_[capacity_],
            0.0f);
}

RingBuffer::~RingBuffer() {
  Deallocate(data_);
  data_ = nullptr;
}

void RingBuffer::Pop(float* dest, const unsigned int count) {
  ASSERT(IsGood());
  ASSERT(count > 0);

  // Is zero padding required ?
  // Using "ints" here cause count may be < size, or the opposite
  const unsigned int zeropadding_count(static_cast<unsigned int>(
    (std::max(static_cast<int>(count - size_), 0))));
  // Actual elements count to be copied
  const unsigned int copy_count(count - zeropadding_count);
  // If data needs to be copied...(e.g. ringbuffer size > 0)
  if (copy_count > 0) {
    // Length of the "right" part: from reading cursor to the buffer end
    const unsigned int right_part_size(std::min(capacity_ - reading_position_,
                                       copy_count));
    // Length of the "left" part: from the buffer beginning
    // to the last element to be copied
    const unsigned int left_part_size(copy_count - right_part_size);

    //  Copying the first part
    TransferData(&data_[reading_position_],
                 &data_[reading_position_ + right_part_size],
                 &dest[0]);
    if (0 != left_part_size) {
      //  copy the second part (if there is one)
      TransferData(&data_[0],
                   &data_[left_part_size],
                   &dest[right_part_size]);
    }

    reading_position_ += copy_count;
    reading_position_ = reading_position_ % capacity_;

    size_ -= copy_count;
  }  // If data needs to be copied...
  // Zero-padding
  if (zeropadding_count) {
    std::fill(&dest[copy_count],
              &dest[copy_count + zeropadding_count],
              0.0f);
  }
}

void RingBuffer::Push(SampleRead value) {
  ASSERT(IsGood());
  ASSERT(capacity_ - writing_position_ >= openmini::SampleSize);

  Store(&data_[writing_position_], value);

  writing_position_ += openmini::SampleSize;
  writing_position_ = writing_position_ % capacity_;
  size_ += openmini::SampleSize;
}

void RingBuffer::Clear(void) {
  writing_position_ = 0;
  reading_position_ = 0;
  size_ = 0;
  if (IsGood()) {
    std::fill(&data_[0],
              &data_[capacity_],
              0.0f);
  }
}

void RingBuffer::ResizeIfNeedBe(const unsigned int size) {
  const unsigned int actual_capacity(ComputeRequiredElements(size));
  if (actual_capacity > capacity_) {
    return Resize(actual_capacity);
  }
}

bool RingBuffer::IsGood(void) const {
  return data_ != nullptr;
}

unsigned int RingBuffer::capacity(void) const {
  ASSERT(IsGood());
  return capacity_;
}

unsigned int RingBuffer::size(void) const {
  ASSERT(IsGood());
  return ComputeMaxElements(size_);
}

void RingBuffer::Resize(const unsigned int size) {
  ASSERT(size > 0);

  // TODO(gm): this should probably be moved into ComputeCapacity()
  const unsigned int actual_capacity(FindImmediateNextMultiple(size,
                                     openmini::SampleSize)
  // This is the offset required in order to make future left space
  // a multiple of SampleSize.
  // E.g., IsMultiple(future capacity - currently filled data, SampleSize)
    + GetOffsetFromNextMultiple(size_, openmini::SampleSize));
  const unsigned int max_fill_count(std::min(size_, actual_capacity));
  float* temp(static_cast<float*>(Allocate(actual_capacity * sizeof(*data_))));
  ASSERT(temp != nullptr);
  std::fill(&temp[0],
            &temp[actual_capacity],
            0.0f);
  if (IsGood()) {
    // The current "interesting" part, between both position,
    // is copied to the new buffer beginning
    TransferData(&data_[reading_position_],
                 &data_[reading_position_ + max_fill_count],
                 &temp[0]);
    Deallocate(data_);
  }
  data_ = temp;
  capacity_ = actual_capacity;
  // Remember that the "interesting" part on the previous buffer
  // has been moved to the beginning of the new one !
  writing_position_ = max_fill_count;
  reading_position_ = 0;
  size_ = max_fill_count;
}

unsigned int RingBuffer::ComputeRequiredElements(
    const unsigned int size) const {
  // For this implementation, one internal sample = one output sample
  return size;
}

unsigned int RingBuffer::ComputeMaxElements(const unsigned int size) const {
  // For this implementation, one internal sample = one output sample
  return size;
}

void RingBuffer::TransferData(const float* const in_first,
                              const float* const in_last,
                              float* const out_first) {
  std::copy(in_first, in_last, out_first);
}

}  // namespace synthesizer
}  // namespace openmini
