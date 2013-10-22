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

namespace openmini {
namespace synthesizer {

Synthesizer::Synthesizer() {
  // Nothing to do here for now
}

void Synthesizer::ProcessAudio(float* const output,
                               const unsigned int length) {
  // First, zeroing the output
  std::fill(&output[0], &output[length], 0.0f);
  mixer_.ProcessAudio(output, length);
}

void Synthesizer::NoteOn(const int note) {
  mixer_.NoteOn(note);
}

void Synthesizer::NoteOff(const int note) {
  mixer_.NoteOff(note);
}

}  // namespace synthesizer
}  // namespace openmini
