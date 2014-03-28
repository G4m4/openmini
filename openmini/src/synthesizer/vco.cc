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

#include "soundtailor/src/generators/generator_base.h"

#include "openmini/src/synthesizer/generator_factory.h"

namespace openmini {
namespace synthesizer {

Vco::Vco()
  : // Default on Triangle
    generator_(generators::CreateGenerator(Waveform::kTriangle)),
    volume_(1.0f),
    frequency_(0.0f),
    last_(Fill(0.0f)),
    waveform_(Waveform::kTriangle),
    update_(false) {
  OPENMINI_ASSERT(generator_ != nullptr);
}

Vco::~Vco() {
  OPENMINI_ASSERT(generator_ != nullptr);
  generators::DestroyGenerator(generator_);
}

void Vco::SetFrequency(const float frequency) {
  OPENMINI_ASSERT(frequency > 0.0f);
  OPENMINI_ASSERT(frequency < SamplingRate::Instance().GetHalf());

  if (frequency != frequency_) {
    frequency_ = frequency;
    update_ = true;
  }
}

void Vco::SetVolume(const float volume) {
  OPENMINI_ASSERT(volume <= 1.0f);
  OPENMINI_ASSERT(volume >= 0.0f);

  // Volume should be managed as any other parameter (e.g. as frequency)
  volume_ = volume;
}

void Vco::SetWaveform(const Waveform::Type value) {
  // This is temporary
  if (value != waveform_) {
    ::soundtailor::generators::Generator_Base* temp
      = generators::CreateGenerator(value, GetLast(last_));
    OPENMINI_ASSERT(temp != nullptr);
    generators::DestroyGenerator(generator_);
    generator_ = temp;
    waveform_ = value;
    // Force parameters processing!
    update_ = true;
    ProcessParameters();
    // Explicit call to generators "take last change into account"
    generator_->ProcessParameters();
  }
}

Sample Vco::operator()(void) {
  OPENMINI_ASSERT(generator_ != nullptr);
  ProcessParameters();
  last_ = openmini::MulConst(volume_, (*generator_)());
  return last_;
}

void Vco::ProcessParameters(void) {
  OPENMINI_ASSERT(generator_ != nullptr);
  if (update_) {
    const float normalized_freq(frequency_ / SamplingRate::Instance().Get());
    generator_->SetFrequency(normalized_freq);
    update_ = false;
  }
}

}  // namespace synthesizer
}  // namespace openmini
