/// @filename synthesizer.cc
/// @brief Main Synthesizer object implementation
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

#include "openmini/src/synthesizer/synthesizer.h"

#include "openmini/src/synthesizer/mixer.h"
#include "openmini/src/synthesizer/parameters.h"

namespace openmini {
namespace synthesizer {

Synthesizer::Synthesizer()
    : ParametersManager() {
  // Nothing to do here for now
}

void Synthesizer::ProcessAudio(float* const output,
                               const unsigned int length) {
  ASSERT(output != nullptr);
  ASSERT(length > 0);

  // First, zeroing the output
  std::fill(&output[0], &output[length], 0.0f);
  ProcessParameters();
  mixer_.ProcessAudio(output, length);
}

void Synthesizer::NoteOn(const unsigned int note) {
  ASSERT(note >= openmini::kMinKeyNote);
  ASSERT(note <= openmini::kMaxKeyNote);

  mixer_.NoteOn(note);
}

void Synthesizer::NoteOff(const unsigned int note) {
  ASSERT(note >= openmini::kMinKeyNote);
  ASSERT(note <= openmini::kMaxKeyNote);

  mixer_.NoteOff(note);
}

void Synthesizer::ProcessParameters(void) {
  if (ParametersChanged()) {
    UpdatedParametersIterator iter(*this);
    do {
      const int parameter_id(iter.GetID());
      switch (parameter_id) {
        case(Parameters::kOsc1Volume): {
          mixer_.SetVolume(0, GetValue(Parameters::kOsc1Volume));
          break;
        }
        case(Parameters::kOsc2Volume): {
          mixer_.SetVolume(1, GetValue(Parameters::kOsc2Volume));
          break;
        }
        case(Parameters::kOsc3Volume): {
          mixer_.SetVolume(2, GetValue(Parameters::kOsc3Volume));
          break;
        }
        case(Parameters::kOsc1Waveform): {
          mixer_.SetWaveform(0,
            GetDiscreteValue<Waveform::Type>(Parameters::kOsc1Waveform));
          break;
        }
        case(Parameters::kOsc2Waveform): {
          mixer_.SetWaveform(1,
            GetDiscreteValue<Waveform::Type>(Parameters::kOsc2Waveform));
          break;
        }
        case(Parameters::kOsc3Waveform): {
          mixer_.SetWaveform(2,
            GetDiscreteValue<Waveform::Type>(Parameters::kOsc3Waveform));
          break;
        }
        default: {
          // Should never happen
          ASSERT(false);
        }
      }
    } while (iter.Next());
    ParametersProcessed();
  }
}

}  // namespace synthesizer
}  // namespace openmini
