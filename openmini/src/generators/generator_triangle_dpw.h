/// @filename generator_triangle_dpw.h
/// @brief Triangle signal generator using DPW algorithm
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

#ifndef OPENMINI_SRC_GENERATORS_GENERATOR_TRIANGLE_DPW_H_
#define OPENMINI_SRC_GENERATORS_GENERATOR_TRIANGLE_DPW_H_

#include "openmini/src/generators/generators_common.h"

namespace openmini {
namespace generators {

/// @brief Triangle signal generator
/// using Differentiated Parabolic Wave (DPW) algorithm
class TriangleDPW {
 public:
  TriangleDPW();
  virtual float operator()(void);
  virtual void SetPhase(const float phase);
  virtual void SetFrequency(const float frequency);

 protected:
  virtual void ProcessParameters(void);

 private:
  PhaseAccumulator sawtooth_gen_;  //< Internal basic sawtooth signal generator
  Differentiator differentiator_;  //< Internal basic differentiator
  float normalization_factor_;  //< To be applied on the signal after synthesis
  float frequency_;  //< Frequency to be set, allows asynchronous update
  bool update_;  //< True if internal values have to be changed
};

}  // namespace generators
}  // namespace openmini

#endif  // OPENMINI_SRC_GENERATORS_GENERATOR_TRIANGLE_DPW_H_
