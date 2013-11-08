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

#include "openmini/src/generators/generator_base.h"
#include "openmini/src/generators/generators_factory.h"

namespace openmini {
namespace synthesizer {

Vco::Vco(void)
  : // Default on Triangle
    generator_(generators::CreateGenerator(Waveform::kTriangle)),
    volume_(1.0f),
    waveform_(Waveform::kTriangle) {
  // Nothing to do here for now
}

Vco::~Vco() {
  delete generator_;
}

void Vco::SetFrequency(const float frequency) {
  if (frequency != frequency_) {
    frequency_ = frequency;
    update_ = true;
  }
}

void Vco::SetVolume(const float volume) {
  ASSERT(volume <= 1.0f);
  ASSERT(volume >= 0.0f);

  // Volume should be managed as any other parameter (e.g. as frequency)
  volume_ = volume;
}

void Vco::SetWaveform(const Waveform::Type value) {
  // This is temporary
  if (value != waveform_) {
    generators::Generator_Base* temp = generators::CreateGenerator(value,
                                                                   generator_);
    delete generator_;
    generator_ = temp;
    waveform_ = value;
    // Process parameters!
    update_ = true;
  }
}

float Vco::operator()(void) {
  ProcessParameters();
  return volume_ * (*generator_)();
}

void Vco::ProcessParameters(void) {
  if (update_) {
    generator_->SetFrequency(frequency_);
    update_ = false;
  }
}

}  // namespace synthesizer
}  // namespace openmini
