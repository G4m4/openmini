/// @filename filter_base.h
/// @brief OpenMini filters base class declaration
///
/// Base interface and common methods for all filters
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

#ifndef OPENMINI_SRC_FILTERS_FILTER_BASE_H_
#define OPENMINI_SRC_FILTERS_FILTER_BASE_H_

#include "openmini/src/common.h"

namespace openmini {
namespace filters {

/// @brief Base class, defining common methods
/// to be implemented in all filters
///
/// All filters are supposed to work at a fixed sampling rate
/// which may be changed in compile-time in configuration.h
///
/// All filters are "dumb" - they do not do any fancy parameter management.
/// It means that any asynchronous parameter update has to be managed upstream,
/// because the setters here are "instantaneous".
class ALIGN Filter_Base {
 public:
  /// @brief Default constructor
  Filter_Base(void) {
    // Nothing to do here for now
  }
  virtual ~Filter_Base() {
    // Nothing to do here for now
  };
  /// @brief Actual process function for one sample
  ///
  /// Process is done per-sample for flexibility purpose, beware of not having
  /// one of this called per-sample! Check in the final code that it actually
  /// gets inlined if needed
  virtual Sample operator()(const Sample& sample) = 0;
  /// @brief Reset the instance to the given qualitfy factor - "resonance"
  ///
  /// Resonance is not normalized
  ///
  /// @param[in]    resonance     Resonance to set the filter to
  virtual void SetResonance(const float resonance) = 0;
  /// @brief Set the filter to the given cutoff frequency
  ///
  /// Frequency is normalized - the input value should be in [0.0f ; 0.5f]
  /// 0.5 because:
  /// f_{n} = \frac{f}{f_{s}}
  /// and:
  /// f_{max} = \frac{f_{s}}{2}
  ///
  /// @param[in]    frequency         Cutoff frequency to set the filter to
  virtual void SetCutoffFrequency(const float frequency) = 0;
};

}  // namespace filters
}  // namespace openmini

#endif  // OPENMINI_SRC_FILTERS_FILTER_BASE_H_
