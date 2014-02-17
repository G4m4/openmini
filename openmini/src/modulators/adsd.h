/// @filename adsd.h
/// @brief Envelop generator using Attack-Decay-Sustain-Decay (ADSD) model
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

#ifndef OPENMINI_SRC_MODULATORS_ADSD_H_
#define OPENMINI_SRC_MODULATORS_ADSD_H_

#include <array>

#include "openmini/src/common.h"
#include "openmini/src/modulators/envelopgenerator_base.h"
#include "openmini/src/modulators/modulators_common.h"

namespace openmini {
namespace modulators {

/// @brief 2nd order low pass filter
/// using the most simple (and computationally efficient) implementation
class ALIGN Adsd : public EnvelopGenerator_Base {
 public:
  Adsd();
  virtual ~Adsd();

  virtual void TriggerOn(void);

  virtual void TriggerOff(void);

  virtual float operator()(void);

  /// Note that the release here is not used since this is a ADSD:
  /// the decay setting also sets the release
  virtual void SetParameters(const unsigned int attack,
                             const unsigned int decay,
                             const unsigned int release,
                             const float sustain_level);

  Section GetCurrentSection(void) const;

 private:
  /// @brief Helper function for computing the increment at each increment,
  /// given the rise (vertical change) and run (horizontal change)
  double ComputeIncrement(const float rise, const unsigned int run);

  double current_increment_;  ///< Increment to use for the current slope
  double current_value_;  ///< Current amplitude
  Section current_section_;  ///< The current part of the generated envelop
  float sustain_level_;  ///< Amplitude to maintain while sustain is on
  unsigned int cursor_;  ///< Time cursor (e.g. samples since the last event)
  unsigned int attack_;  ///< Time setting for the attack
  unsigned int decay_;  ///< Time setting for the decay
  unsigned int actual_decay_;  ///< Actual decay time in local frame
  unsigned int actual_release_;  ///<  Actual release time in local frame
};

}  // namespace modulators
}  // namespace openmini

#endif  // OPENMINI_SRC_MODULATORS_ADSD_H_
