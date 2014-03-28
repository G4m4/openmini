/// @filename generator_factory.h
/// @brief Generators factory - declaration
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

#ifndef OPENMINI_SRC_SYNTHESIZER_GENERATOR_FACTORY_H_
#define OPENMINI_SRC_SYNTHESIZER_GENERATOR_FACTORY_H_

#include "openmini/src/common.h"

// SoundTailor forward declarations
namespace soundtailor {
namespace generators {
class Generator_Base;
}  // namespace generators
}  // namespace soundtailor

namespace openmini {
  // TODO(gm): is this namespace actually useful?
namespace generators {

/// @brief Create a generator based on the input enum value
///
/// The user is responsible for the destruction of the created object
///
/// @param[in]  waveform    Waveform of the signal generator to be created
/// @param[in]  phase   Phase to initialize the new generator to
///
/// @return a pointer to the created generator
soundtailor::generators::Generator_Base* CreateGenerator(
  const Waveform::Type waveform,
  const float phase = 0.0f);

void DestroyGenerator(soundtailor::generators::Generator_Base* generator);

}  // namespace generators
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_GENERATOR_FACTORY_H_
