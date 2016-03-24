/// @filename maths.h
/// @brief OpenMini common maths header
/// @author gm
/// @copyright gm 2016
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

#ifndef OPENMINI_SRC_MATHS_H_
#define OPENMINI_SRC_MATHS_H_

// We simply redirect to soundtailor math stuff, and expose its structures

#include "externals/soundtailor/soundtailor/src/maths.h"

using soundtailor::Math;
using soundtailor::VectorMath;

#endif  // OPENMINI_SRC_MATHS_H_
