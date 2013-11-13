/// @filename generators_factory.cc
/// @brief Generators factory - implementation
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

#include "openmini/src/generators/factory.h"

#include "openmini/src/generators/generator_base.h"
#include "openmini/src/generators/sawtooth_dpw.h"
#include "openmini/src/generators/triangle_dpw.h"

namespace openmini {
namespace generators {

Generator_Base* CreateGenerator(const Waveform::Type waveform,
                                const Generator_Base* previous) {
  float phase(0.0f);
  if (previous) {
    phase = previous->Phase();
    ASSERT(phase <= 1.0f);
    ASSERT(phase >= -1.0f);
  }
  switch (waveform) {
    case(Waveform::kTriangle): {
      void* ptr(Allocate(sizeof(TriangleDPW)));
      ASSERT(ptr != nullptr);
      return new (ptr) TriangleDPW(phase);
    }
    case(Waveform::kSawtooth): {
      void* ptr(Allocate(sizeof(SawtoothDPW)));
      ASSERT(ptr != nullptr);
      return new (ptr) SawtoothDPW(phase);
    }
    default: {
      // Should never happen
      ASSERT(false);
    }
  }
}

void DestroyGenerator(Generator_Base* generator) {
  ASSERT(generator != nullptr);

  generator->~Generator_Base();
  Deallocate(generator);
}

}  // namespace generators
}  // namespace openmini
