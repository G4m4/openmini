/// @filename modulators_common.cc
/// @brief OpenMini common stuff for all modulators - implementation
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

#include "openmini/src/modulators/modulators_common.h"

namespace openmini {
namespace modulators {

Section GetNextSection(const Section enum_value) {
  return static_cast<Section>(static_cast<int>(enum_value) + 1);
}

}  // namespace modulators
}  // namespace openmini
