/// @filename generator_triangle_dpw.cc
/// @brief Triangle signal generator using DPW algorithm - implementation
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

#include "openmini/src/generators/generator_triangle_dpw.h"

namespace openmini {
namespace generators {

TriangleDPW::TriangleDPW()
  : sawtooth_gen_(),
    differentiator_(),
    normalization_factor_(0.0f),
    frequency_(0.0f),
    update_(false) {
  // Nothing to do here for now
}

float TriangleDPW::operator()(void) {
  ProcessParameters();
  // Raw sawtooth signal
  float current(sawtooth_gen_());
  // "Parabolized" here
  current -= current * std::abs(current);
  // Differentiation & Normalization
  current = differentiator_(current) * normalization_factor_;
  return current;
}

void TriangleDPW::SetPhase(const float phase) {
  // Phase is supposed to be in [-1.0 ; 1.0], hence the assert
  ASSERT(phase <= 1.0f);
  ASSERT(phase >= -1.0f);
  // If we are not sure, we can use the following:
  // phase_ = Wrap(phase);
  sawtooth_gen_.SetPhase(phase);
}

void TriangleDPW::SetFrequency(const float frequency) {
  ASSERT(frequency > 0.0f);
  ASSERT(frequency < openmini::kSamplingRateHalf);

  frequency_ = frequency;
  sawtooth_gen_.SetFrequency(frequency);

  update_ = true;
}

void TriangleDPW::ProcessParameters(void) {
  if (update_) {
    normalization_factor_ = openmini::kSamplingRate / (2.0f * frequency_);
    update_ = false;
  }
}

}  // namespace generators
}  // namespace openmini
