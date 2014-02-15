/// @filename modulator_base.h
/// @brief OpenMini modulators base class declaration
///
/// Base interface and common methods for all modulators
///
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

#ifndef OPENMINI_SRC_MODULATORS_ENVELOPGENERATOR_BASE_H_
#define OPENMINI_SRC_MODULATORS_ENVELOPGENERATOR_BASE_H_

#include "openmini/src/common.h"

namespace openmini {
namespace modulators {

/// @brief Base class, defining common methods
/// to be implemented in all envelop generators
///
/// All envelop generators are supposed to work at a fixed sampling rate
/// which may be changed in compile-time in configuration.h
///
/// All envelop generators are "dumb" - they do not do any fancy
/// parameter management.
/// It means that any asynchronous parameter update has to be managed upstream,
/// because the setters here are "instantaneous".
class ALIGN EnvelopGenerator_Base {
 public:
  /// @brief Default constructor
  EnvelopGenerator_Base(void) {
    // Nothing to do here for now
  }
  virtual ~EnvelopGenerator_Base() {
    // Nothing to do here for now
  }

  /// @brief Event for triggering the beginning of the envelop
  ///
  /// Note that the amplitude start value is the one applied when triggering.
  /// e.g. repeated calls to this function will lead to accumulated envelops.
  virtual void TriggerOn(void) = 0;

  /// @brief Event for triggering the end of the envelop
  ///
  /// Note that this does not mean that the envelop stops right after this
  /// function was called, depending on the parameters currently in use
  virtual void TriggerOff(void) = 0;

  /// @brief Actual process function for one sample:
  /// output one sample of the envelop
  ///
  /// Process is done per-sample for flexibility purpose, beware of not having
  /// one of this called per-sample! Check in the final code that it actually
  /// gets inlined if needed
  virtual float operator()(void) = 0;

  /// @brief Set the envelop generator parameters,
  /// common to all generators implementations
  ///
  /// @param[in]  attack    Length of the attack (absolute value in samples)
  /// @param[in]  decay    Length of the decay (absolute value in samples)
  /// @param[in]  release    Length of the release (absolute value in samples)
  /// @param[in]  sustain_level    Sustain amplitude (absolute normalized value)
  virtual void SetParameters(const unsigned int attack,
                             const unsigned int decay,
                             const unsigned int release,
                             const float sustain_level) = 0;
};

}  // namespace modulators
}  // namespace openmini

#endif  // OPENMINI_SRC_MODULATORS_ENVELOPGENERATOR_BASE_H_
