/// @filename mixer.cc
/// @brief Mixer object implementation
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

#include "openmini/src/common.h"
#include "openmini/src/maths.h"
#include "openmini/src/synthesizer/mixer.h"
#include "openmini/src/synthesizer/synthesizer_common.h"
#include "openmini/src/synthesizer/vco.h"

namespace openmini {
namespace synthesizer {

Mixer::Mixer()
    : active_(false) {
  // Nothing to do here for now
}

Mixer::~Mixer() {
  // Nothing to do here for now
}

Sample Mixer::operator()(void) {
  Sample output(VectorMath::Fill(0.0f));
  if (active_) {
    for (auto& vco : vcos_) {
      output = VectorMath::Add(output, vco());
    }
  }
  return output;
}

void Mixer::NoteOn(const unsigned int note) {
  OPENMINI_ASSERT(note >= openmini::kMinKeyNote);
  OPENMINI_ASSERT(note <= openmini::kMaxKeyNote);

  const float frequency(NoteToFrequency(note));
  for (auto& vco : vcos_) {
    vco.SetFrequency(frequency);
  }
  active_ = true;
}

void Mixer::NoteOff(const unsigned int note) {
  OPENMINI_ASSERT(note >= openmini::kMinKeyNote);
  OPENMINI_ASSERT(note <= openmini::kMaxKeyNote);
  // Nothing to do here for now
}

void Mixer::SetVolume(const int vco_id, const float value) {
  // TODO(gm): actual parameters management
  OPENMINI_ASSERT(vco_id >= 0);
  OPENMINI_ASSERT(vco_id < kVCOsCount);

  // TODO(gm): actual oscillators volume management
  const float actual_value(value / static_cast<float>(kVCOsCount));
  vcos_[vco_id].SetVolume(actual_value);
}

void Mixer::SetWaveform(const int vco_id, const Waveform::Type value) {
  // TODO(gm): actual parameters management
  OPENMINI_ASSERT(vco_id >= 0);
  OPENMINI_ASSERT(vco_id < kVCOsCount);

  vcos_[vco_id].SetWaveform(value);
}

}  // namespace synthesizer
}  // namespace openmini
