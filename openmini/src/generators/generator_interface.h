/// @filename generator_interface.h
/// @brief OpenMini generators interface declaration
///
/// Base interface for all generators
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

#ifndef OPENMINI_SRC_GENERATORS_GENERATOR_INTERFACE_H_
#define OPENMINI_SRC_GENERATORS_GENERATOR_INTERFACE_H_

#include "openmini/src/common.h"

namespace openmini {
namespace generators {

/// @brief Interface defining common methods
/// to be implemented in all generators
///
/// All generators are supposed to work at a fixed sampling rate
/// and buffer length - these may be changed in compile-time in configuration.h
class Generator_Interface {
 public:
  virtual ~Generator_Interface() {
    // Nothing to do here for now
  };
  /// @brief Actual process function for one sample
  ///
  /// Process is done per-sample for flexibility purpose, beware of not having
  /// one of this called per-sample! Check in the final code that it actually
  /// gets inlined
  virtual float operator()(void) = 0;
  /// @brief Reset the instance to the given phase - nothing else gets changed
  ///
  /// Phase is normalized - the input value should be in [-1.0f ; 1.0f]
  ///
  /// @param[in]    phase     Phase to set the generator to
  virtual void SetPhase(const float phase) = 0;
  /// @brief Set the generator to the given output frequency
  ///
  /// Frequency is not normalized here - the unit is Hz
  ///
  /// @param[in]    frequency         Frequency to set the generator to
  virtual void SetFrequency(const float frequency) = 0;

 protected:
  /// @brief Update internal generator variables with lastly set parameters
  ///
  /// When set, parameters are not immediately used - they must be processed
  /// into generator internal variables
  ///
  /// This function allows to make this update asynchronously
  /// it should probably be called from within a dedicated "update" loop
  virtual void ProcessParameters(void) = 0;
};

}  // namespace generators
}  // namespace openmini

#endif  // OPENMINI_SRC_GENERATORS_GENERATOR_INTERFACE_H_
