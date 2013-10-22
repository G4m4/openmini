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

// std::fill
#include <algorithm>

#include "openmini/src/common.h"
#include "openmini/src/synthesizer/mixer.h"
#include "openmini/src/synthesizer/synthesizer_common.h"
#include "openmini/src/synthesizer/vco.h"

namespace openmini {
namespace synthesizer {

//static const int kVCOsCount(3);

Mixer::Mixer() {
  for (int vco_idx(0); vco_idx < kVCOsCount; ++vco_idx) {
    // Default on Triangle generators
    vcos_[vco_idx] = new Vco(Waveform::kTriangle);
  }
  std::fill(&vcos_gain_[0], &vcos_gain_[kVCOsCount], 0.0f);
}

Mixer::~Mixer() {
  for (int vco_idx(0); vco_idx < kVCOsCount; ++vco_idx) {
    delete vcos_[vco_idx];
  }
}

void Mixer::ProcessAudio(float* const output, const int length) {
  float* current_sample(output);
  for (int sample_idx(0); sample_idx < length; ++sample_idx) {
    for (int vco_idx(0); vco_idx < kVCOsCount; ++vco_idx) {
      *current_sample += vcos_[vco_idx]->operator()() * vcos_gain_[vco_idx];
    }
    current_sample += 1;
  }
}

void Mixer::NoteOn(const int note) {
  for (int vco_idx(0); vco_idx < kVCOsCount; ++vco_idx) {
    vcos_[vco_idx]->SetFrequency(NoteToFrequency(note));
  }
  // TODO(gm): actual oscillators management
  std::fill(&vcos_gain_[0], &vcos_gain_[kVCOsCount], 0.2f);
}

void Mixer::NoteOff(const int note) {
  // TODO(gm): actual oscillators management
  std::fill(&vcos_gain_[0], &vcos_gain_[kVCOsCount], 0.0f);
}

}  // namespace synthesizer
}  // namespace openmini
