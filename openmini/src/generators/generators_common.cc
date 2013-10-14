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

namespace openmini {
namespace generators {

void InitSeed(int seed) {
  rng_seed = seed;
}

float GeneratorNormFloatRand::operator()(void) const {
  rng_seed *= 16807;
  return static_cast<float>(rng_seed) * 4.6566129e-010f;
}

// PhaseAccumulator

PhaseAccumulator::PhaseAccumulator()
    : increment_(0.0f),
      current_(0.0f),
      frequency_(0.0f),
      update_(false) {
  // Nothing to do here for now
}

float PhaseAccumulator::operator()(void) {
  ProcessParameters();
  const float out(current_);
  current_ = IncrementAndWrap(current_, increment_);
  return out;
}

void PhaseAccumulator::SetPhase(const float phase) {
  // Phase is supposed to be in [-1.0 ; 1.0], hence the assert
  ASSERT(phase <= 1.0f);
  ASSERT(phase >= -1.0f);
  // If we are not sure, we can use the following:
  // phase_ = Wrap(phase);
  current_ = phase;
}

void PhaseAccumulator::SetFrequency(const float frequency) {
  ASSERT(frequency > 0.0f);
  ASSERT(frequency < openmini::kSamplingRateHalf);

  frequency_ = frequency;
  update_ = true;
}

void PhaseAccumulator::ProcessParameters(void) {
  if (update_) {
    increment_ = (2.0f * frequency_) / openmini::kSamplingRate;
    update_ = false;
  }
}

Differentiator::Differentiator()
    : last_(0.0f) {
  // Nothing to do here
}

float Differentiator::operator()(const float sample) {
  const float before_diff(sample);
  const float after_diff(sample - last_);
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

float IncrementAndWrap(const float input, const float increment) {
  float output(input + increment);
  if (output > 1.0f) {
    output -= 2.0f;
  }
  return output;
}

float LinearInterpolation(
    const float left,
    const float right,
    const float ratio) {
  return (right - left) * ratio + left;
}

}  // namespace generators
}  // namespace openmini
