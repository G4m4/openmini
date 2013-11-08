/// @filename generator_sawtooth_dpw.cc
/// @brief Sawtooth signal generator using DPW algorithm - implementation
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

#include "openmini/src/generators/generator_sawtooth_dpw.h"

namespace openmini {
namespace generators {

SawtoothDPW::SawtoothDPW(const float phase)
    : TriangleDPW(phase) {
  // Nothing to do here for now
}

float SawtoothDPW::operator()(void) {
  // Raw sawtooth signal
  float current(sawtooth_gen_());
  // Parabolization
  current *= current;
  // Differentiation & Normalization
  current = differentiator_(current);
  current *= normalization_factor_;
  return current;
}

void SawtoothDPW::SetFrequency(const float frequency) {
  ASSERT(frequency >= 0.0f);
  ASSERT(frequency <= 0.5f);

  sawtooth_gen_.SetFrequency(frequency);
  normalization_factor_ = 1.0f / (4.0f * frequency);
}

}  // namespace generators
}  // namespace openmini
