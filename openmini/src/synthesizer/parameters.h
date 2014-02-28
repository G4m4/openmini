/// @filename parameters.h
/// @brief List of parameters to be used by the synthesizer
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

#ifndef OPENMINI_SRC_SYNTHESIZER_PARAMETERS_H_
#define OPENMINI_SRC_SYNTHESIZER_PARAMETERS_H_

#include <array>

#include "openmini/src/common.h"
#include "openmini/src/synthesizer/parameter_meta.h"

namespace openmini {
namespace synthesizer {

/// @brief Allowed parameters
// Implementation detail: ordered from the most probable to the least
namespace Parameters {
enum Type {
  kOsc1Volume = 0,
  kOsc2Volume,
  kOsc3Volume,
  kOsc1Waveform,
  kOsc2Waveform,
  kOsc3Waveform,
  kFilterFreq,
  kFilterQ,
  kAttackTime,
  kDecayTime,
  kSustainLevel,
  kCount
};

/// @brief Parameters metadata
// Implementation detail: ordered from the most probable to the least
const std::array<ParameterMeta, Parameters::kCount> kParametersMeta = {{
  ParameterMeta(0.0f,
                1.0f,
                1.0f,
                1,
                0,
                "Osc1 Volume",
                "Volume for oscillator 1"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                1,
                0,
                "Osc2 Volume",
                "Volume for oscillator 2"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                1,
                0,
                "Osc3 Volume",
                "Volume for oscillator 3"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                1,
                Waveform::kCount,
                "Osc1 Waveform",
                "Waveform for oscillator 1"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                1,
                Waveform::kCount,
                "Osc2 Waveform",
                "Waveform for oscillator 2"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                1,
                Waveform::kCount,
                "Osc3 Waveform",
                "Waveform for oscillator 3"),
  ParameterMeta(kMinFilterFreq,
                kMaxFilterFreq,
                1.0f,  // "almost" passthrough
                1,
                0,
                "Filter Freq",
                "Cutoff Frequency for the filter"),
  ParameterMeta(1e-5f,
                10.0f,
                0.7f,
                1,
                0,
                "Filter Q",
                "Q Factor for the filter"),
  ParameterMeta(static_cast<float>(kMinTime),
                static_cast<float>(kMaxTime),
                0.5f,
                1,
                kMaxTime - kMinTime,
                "Attack Time",
                "Envelop generator attack time"),
  ParameterMeta(static_cast<float>(kMinTime),
                static_cast<float>(kMaxTime),
                0.5f,
                1,
                kMaxTime - kMinTime,
                "Decay Time",
                "Envelop generator decay time"),
  ParameterMeta(0.0f,
                1.0f,
                0.5f,
                1,
                0,
                "Sustain Level",
                "Envelop generator sustain level")
}};

}  // namespace Parameters
}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_PARAMETERS_H_
