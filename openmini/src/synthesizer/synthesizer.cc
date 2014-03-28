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

#include "openmini/src/samplingrate.h"
#include "openmini/src/synthesizer/parameters.h"
#include "openmini/src/synthesizer/synthesizer_common.h"

namespace openmini {
namespace synthesizer {

/// @brief Default (on startup) expected block size
static const unsigned int kDefaultBlockSize(512);

Synthesizer::Synthesizer(const float output_limit)
    : ParametersManager(Parameters::kParametersMeta),
      mixer_(),
      filter_(),
      modulator_(),
      limiter_(output_limit),
      buffer_() {
  // Nothing to do here for now
}

void Synthesizer::ProcessAudio(float* const output,
                               const unsigned int length) {
  OPENMINI_ASSERT(output != nullptr);
  OPENMINI_ASSERT(length > 0);

  // First, zeroing the output
  std::fill(&output[0], &output[length], 0.0f);
  ProcessParameters();

  // Resizing the buffer if need be
  buffer_.Reserve(length);

  while (buffer_.Size() < length) {
    buffer_.Push(limiter_(modulator_(filter_(mixer_()))));
  }
  buffer_.Pop(output, length);
}

void Synthesizer::NoteOn(const unsigned int note) {
  OPENMINI_ASSERT(note >= openmini::kMinKeyNote);
  OPENMINI_ASSERT(note <= openmini::kMaxKeyNote);

  // This has to be done BEFORE sending trigger messages;
  // Indeed things may have changed since last audio process!
  ProcessParameters();

  mixer_.NoteOn(note);
  modulator_.TriggerOn();
}

void Synthesizer::NoteOff(const unsigned int note) {
  OPENMINI_ASSERT(note >= openmini::kMinKeyNote);
  OPENMINI_ASSERT(note <= openmini::kMaxKeyNote);

  // This has to be done BEFORE sending trigger messages;
  // Indeed things may have changed since last audio process!
  ProcessParameters();

  mixer_.NoteOff(note);
  modulator_.TriggerOff();
}

void Synthesizer::SetOutputSamplingFrequency(const float freq) {
  SamplingRate::Instance().Set(freq);
  // Trigger changes to all parameters in order to take
  // sampling frequency change into account
  ParametersManager::ForceParametersProcess();
}

void Synthesizer::ProcessParameters(void) {
  if (ParametersChanged()) {
    UpdatedParametersIterator iter(*this);
    do {
      const int parameter_id(iter.GetID());
      switch (parameter_id) {
        case(Parameters::kOsc1Volume): {
          // Note that we feed here the "raw" (unnormalized) value
          // This is the same below
          mixer_.SetVolume(0, GetRawValue(Parameters::kOsc1Volume));
          break;
        }
        case(Parameters::kOsc2Volume): {
          mixer_.SetVolume(1, GetRawValue(Parameters::kOsc2Volume));
          break;
        }
        case(Parameters::kOsc3Volume): {
          mixer_.SetVolume(2, GetRawValue(Parameters::kOsc3Volume));
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
        case(Parameters::kFilterFreq): {
          filter_.SetFrequency(GetRawValue(Parameters::kFilterFreq));
          break;
        }
        case(Parameters::kFilterQ): {
          filter_.SetQFactor(GetRawValue(Parameters::kFilterQ));
          break;
        }
        case(Parameters::kAttackTime): {
          modulator_.SetAttack(
            GetDiscreteValue<unsigned int>(Parameters::kAttackTime));
          break;
        }
        case(Parameters::kDecayTime): {
          modulator_.SetDecay(
            GetDiscreteValue<unsigned int>(Parameters::kDecayTime));
          break;
        }
        case(Parameters::kSustainLevel): {
          modulator_.SetSustain(GetRawValue(Parameters::kSustainLevel));
          break;
        }
        default: {
          // Should never happen
          OPENMINI_ASSERT(false);
        }
      }
    } while (iter.Next());
    ParametersProcessed();
  }
}

}  // namespace synthesizer
}  // namespace openmini
