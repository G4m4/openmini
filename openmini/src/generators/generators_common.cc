/// @filename generators_common.cc
/// @brief OpenMini common stuff for all generators - implementation
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

#include "openmini/src/generators/generators_common.h"
#include "openmini/src/maths.h"

namespace openmini {
namespace generators {

// PhaseAccumulator

PhaseAccumulator::PhaseAccumulator(const float phase)
    : Generator_Base(phase),
      phase_(Fill(0.0f)),
      increment_(Fill(0.0f)) {
  // Nothing to do here for now
}

Sample PhaseAccumulator::operator()(void) {
  const Sample out(phase_);
  phase_ = IncrementAndWrap(phase_, increment_);
  return out;
}

void PhaseAccumulator::SetPhase(const float phase) {
  // Phase is supposed to be in [-1.0 ; 1.0], hence the assert
  ASSERT(phase <= 1.0f);
  ASSERT(phase >= -1.0f);
  // If we are not sure, we can use the following:
  // phase_ = Wrap(phase);
  phase_ = Fill(phase);
}

void PhaseAccumulator::SetFrequency(const float frequency) {
  ASSERT(frequency >= 0.0f);
  ASSERT(frequency <= 0.5f);

  const float base_increment(2.0f * frequency);
  increment_ = FillOnLength(base_increment);
  phase_ = FillIncremental(0.0f, base_increment);
}

float PhaseAccumulator::Phase(void) const {
  return GetByIndex<0>(phase_);
}

Differentiator::Differentiator(const float last)
    : last_(last) {
  // Nothing to do here
}

Sample Differentiator::operator()(const Sample sample) {
  const float before_diff(GetByIndex<3>(sample));
  const Sample prev(RotateOnRight(sample,
                                  last_));
  const Sample after_diff(Sub(sample, prev));
  last_ = before_diff;
  return after_diff;
}

// One Pole Filter

OnePoleFilter::OnePoleFilter(const double b0, const double b1)
    : b0_(b0),
      b1_(b1),
      last_(0.0f) {
  // Nothing to do here
}

float OnePoleFilter::operator()(const float input) {
  float out(input * static_cast<float>(b0_));
  out += static_cast<float>(b1_) * last_;
  last_ = out;
  return out;
}

// Miscellaneous

float ErfTabulated(const float input) {
  if (input < -3.0f) {
    // TODO(gm): interpolation for the special case
    // of being just before first table value
    return -1.0f;
  } else if (input > 3.0f) {
      return 1.0f;
  } else {
    // Finding "left" index
    const float indexf = (input + 3.0f)
                          * (static_cast<float>(kErfTableSize) / 6.0f);
    const int index = static_cast<int>(indexf);
    // This is how far we are after left value (0.0f < ratio < 1.0f)
    const float ratio = static_cast<float>(indexf - index);
    // Computation of the "right" value, with the special case
    // of being just after last table value
    float next_value = 1.0f;
    if (index < kErfTableSize - 1) {
        next_value = kErfTable[index + 1];
    }
    // A linear interpolation should be OK here
    return LinearInterpolation(kErfTable[index],
                                next_value,
                                ratio);
  }
}

Sample IncrementAndWrap(const Sample& input, const Sample& increment) {
#if (_USE_SIMD)
  const Sample output(Add(input, increment));
  const Sample constant(Fill(-2.0f));
  const Sample threshold(Fill(1.0f));
  const Sample addition_mask(_mm_cmpgt_ps(output, threshold));
  const Sample add(_mm_and_ps(addition_mask, constant));
  return Add(output, add);
#else
  Sample output(Add(input, increment));
  if (output > 1.0f) {
    const float constant(-2.0f);
    output = Add(output, constant);
  }
  return output;
#endif  // (_USE_SIMD)
}

float LinearInterpolation(
    const float left,
    const float right,
    const float ratio) {
  return (right - left) * ratio + left;
}

}  // namespace generators
}  // namespace openmini
