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

#include "openmini/src/synthesizer/parameter_meta.h"

#include "openmini/src/common.h"

namespace openmini {
namespace synthesizer {

static const ParameterMeta kParameters[] = {
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                true,
                1,
                Waveform::kMaxCount,
                "Osc1 Waveform",
                "Waveform for oscillator 1"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                true,
                1,
                Waveform::kMaxCount,
                "Osc2 Waveform",
                "Waveform for oscillator 2"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                true,
                1,
                Waveform::kMaxCount,
                "Osc3 Waveform",
                "Waveform for oscillator 3"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                true,
                1,
                0,
                "Osc1 Volume",
                "Volume for oscillator 1"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                true,
                1,
                0,
                "Osc2 Volume",
                "Volume for oscillator 2"),
  ParameterMeta(0.0f,
                1.0f,
                0.0f,
                true,
                1,
                0,
                "Osc3 Volume",
                "Volume for oscillator 3")
};

static const int kParametersCount(sizeof(kParameters) / sizeof(ParameterMeta));

// (Using the "enum in its own namespace" trick)
/// @brief Allowed parameters
namespace Parameters {
enum Type {
  kOsc1Waveform = 0,
  kOsc2Waveform,
  kOsc3Waveform,
  kOsc1Volume,
  kOsc2Volume,
  kOsc3Volume,
  kMaxCount
};
}  // namespace Parameters

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_PARAMETERS_H_
