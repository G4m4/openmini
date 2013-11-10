/// @filename ringbuffer.cc
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

void Mixer::ProcessAudio(float* const output, const int length) {
  if (active_) {
    float* current_sample(output);
    const float* end_sample(output + length - 1);
    while(current_sample < end_sample) {
      VcoIterator iter(this);
      Sample temp(openmini::Fill(0.0f));
      do {
        temp = Add(temp, iter.GetVco()());
      } while (iter.Next());
      Store(current_sample, temp);
      current_sample += openmini::SampleSize;
    }
  }
}

void Mixer::NoteOn(const int note) {
  const float frequency(NoteToFrequency(note));
  VcoIterator iter(this);
  do {
    iter.GetVco().SetFrequency(frequency);
  } while (iter.Next());
  active_ = true;
}

void Mixer::NoteOff(const int note) {
  active_ = false;
}

void Mixer::SetVolume(const int vco_id, const float value) {
  // TODO(gm): actual parameters management
  ASSERT(vco_id >= 0);
  ASSERT(vco_id < kVCOsCount);

  vcos_[vco_id].SetVolume(value);
}

void Mixer::SetWaveform(const int vco_id, const Waveform::Type value) {
  // TODO(gm): actual parameters management
  ASSERT(vco_id >= 0);
  ASSERT(vco_id < kVCOsCount);

  vcos_[vco_id].SetWaveform(value);
}

Mixer::VcoIterator::VcoIterator(Mixer* mixer_to_iterate)
    : mixer_(mixer_to_iterate),
      index_(0) {
  // Nothing to do here
}

bool Mixer::VcoIterator::Next() {
  if (index_ >= kVCOsCount - 1) {
    return false;
  } else {
    index_ += 1;
    return true;
  }
}

Vco& Mixer::VcoIterator::GetVco() const {
  return mixer_->vcos_[index_];
}

}  // namespace synthesizer
}  // namespace openmini
