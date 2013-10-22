/// @filename vco.cc
/// @brief "VCO": wrapper around a basic generator - implementation
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

#include "openmini/src/synthesizer/vco.h"
#include "openmini/src/generators/generators_factory.h"
#include "openmini/src/generators/generator_interface.h"

namespace openmini {
namespace synthesizer {

Vco::Vco(const Waveform::Type waveform)
    : generator_(generators::CreateGenerator(waveform)) {
  // Nothing to do here for now
}

Vco::~Vco() {
  delete &generator_;
}

void Vco::SetFrequency(const float frequency) {
  generator_.SetFrequency(frequency);
}

float Vco::operator()(void) {
  return generator_();
}

}  // namespace synthesizer
}  // namespace openmini
