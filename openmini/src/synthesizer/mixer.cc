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
  Sample output(Fill(0.0f));
  if (active_) {
    VcoIterator iter(this);
    do {
      output = Add(output, iter.GetVco()());
    } while (iter.Next());
  }
  return output;
}

void Mixer::NoteOn(const unsigned int note) {
  ASSERT(note >= openmini::kMinKeyNote);
  ASSERT(note <= openmini::kMaxKeyNote);

  const float frequency(NoteToFrequency(note));
  VcoIterator iter(this);
  do {
    iter.GetVco().SetFrequency(frequency);
  } while (iter.Next());
  active_ = true;
}

void Mixer::NoteOff(const unsigned int note) {
  ASSERT(note >= openmini::kMinKeyNote);
  ASSERT(note <= openmini::kMaxKeyNote);

  active_ = false;
}

void Mixer::SetVolume(const int vco_id, const float value) {
  // TODO(gm): actual parameters management
  ASSERT(vco_id >= 0);
  ASSERT(vco_id < kVCOsCount);

  // TODO(gm): actual oscillators volume management
  const float actual_value(value / static_cast<float>(kVCOsCount));
  vcos_[vco_id].SetVolume(actual_value);
}

void Mixer::SetWaveform(const int vco_id, const Waveform::Type value) {
  // TODO(gm): actual parameters management
  ASSERT(vco_id >= 0);
  ASSERT(vco_id < kVCOsCount);

  vcos_[vco_id].SetWaveform(value);
}

Mixer::VcoIterator::VcoIterator(Mixer* mixer_to_iterate)
    : mixer_(mixer_to_iterate),
      iterator_(mixer_to_iterate->vcos_.begin()) {
  ASSERT(mixer_ != nullptr);
}

bool Mixer::VcoIterator::Next() {
  ++iterator_;
  if (iterator_ != mixer_->vcos_.end()) {
    return true;
  } else {
    return false;
  }
}

Vco& Mixer::VcoIterator::GetVco() const {
  return *iterator_;
}

}  // namespace synthesizer
}  // namespace openmini
