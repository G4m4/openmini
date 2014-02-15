/// @filename modulators_common.h
/// @brief OpenMini common stuff for all modulators
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

#ifndef OPENMINI_SRC_MODULATORS_MODULATORS_COMMON_H_
#define OPENMINI_SRC_MODULATORS_MODULATORS_COMMON_H_

#include "openmini/src/common.h"

namespace openmini {
namespace modulators {

/// @brief Available parts of an envelop
enum Section {
  kAttack = 0,
  kDecay,
  kSustain,
  kRelease,
  kZero
};

/// @brief Get the section after the given one
Section GetNextSection(const Section enum_value);

}  // namespace modulators
}  // namespace openmini

#endif  // OPENMINI_SRC_MODULATORS_MODULATORS_COMMON_H_
