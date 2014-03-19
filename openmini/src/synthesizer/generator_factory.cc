/// @filename generator_factory.cc
/// @brief Generators factory - implementation
/// @author gm
/// @copyright gm 2014
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

#include "openmini/src/synthesizer/generator_factory.h"

#include <new>

#include "soundtailor/src/generators/generator_base.h"
#include "soundtailor/src/generators/sawtooth_dpw.h"
#include "soundtailor/src/generators/triangle_dpw.h"

namespace openmini {
namespace generators {

soundtailor::generators::Generator_Base* CreateGenerator(
  const Waveform::Type waveform,
  const soundtailor::generators::Generator_Base* previous) {
  float phase(0.0f);
  if (previous) {
    phase = previous->Phase();
    ASSERT(phase <= 1.0f);
    ASSERT(phase >= -1.0f);
  }
  switch (waveform) {
    case(Waveform::kTriangle): {
      void* ptr(Allocate(sizeof(soundtailor::generators::TriangleDPW)));
      ASSERT(ptr != nullptr);
      return new (ptr) soundtailor::generators::TriangleDPW(phase);
    }
    case(Waveform::kSawtooth): {
      void* ptr(Allocate(sizeof(soundtailor::generators::SawtoothDPW)));
      ASSERT(ptr != nullptr);
      return new (ptr) soundtailor::generators::SawtoothDPW(phase);
    }
    default: {
      // Should never happen
      ASSERT(false);
    }
  }
}

void DestroyGenerator(soundtailor::generators::Generator_Base* generator) {
  ASSERT(generator != nullptr);

  // Beware, this is not safe! (explicit call to base destructor
  // -> possible leaks in the child!)
  // We should use smart pointers anyway
  generator->~Generator_Base();
  Deallocate(generator);
}

}  // namespace generators
}  // namespace openmini
