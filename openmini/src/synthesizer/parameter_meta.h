/// @filename parameter_meta.h
/// @brief Parameter "Meta" class: handle parameter metadata
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

#ifndef OPENMINI_SRC_SYNTHESIZER_PARAMETER_META_H_
#define OPENMINI_SRC_SYNTHESIZER_PARAMETER_META_H_

#include <string>

namespace openmini {
namespace synthesizer {

/// @brief Parameter metadata: holds a parameter descriptor, e.g. everything
/// an external user needs to know in order to use and modify a parameter.
///
/// A parameter is always a floating point value, normalized if need be,
/// and its descriptor cannot change!
class ParameterMeta {
 public:
  /// @brief Default constructor, allowing members initialisation
  explicit ParameterMeta(float min,
                         float max,
                         float default_value,
                         bool is_normalized,
                         int sig_figs,
                         int cardinality,
                         const std::string& name,
                         const std::string& description);

  /// Getters

  float min(void) const;
  float max(void) const;
  float default_value(void) const;
  int sig_figs(void) const;
  int cardinality(void) const;
  bool is_normalized(void) const;
  const std::string& name(void) const;
  const std::string& description(void) const;

 private :
  // No assignment operator for this class
  ParameterMeta& operator=(const ParameterMeta& right);

  const float min_;  ///< Parameter lower bound
  const float max_;  ///< Parameter higher bound
  const float default_value_;  ///< Default (initialization) value
  const bool is_normalized_;  ///< True if the user can only interact with the
                              /// parameter using values within [0.0f ; 1.0f]
  const int sig_figs_;  ///< Digits used to represent the parameter
  const int cardinality_;  ///< Number of elements the parameter can take,
                           /// 0 if continuous
  const std::string name_;  ///< Parameter name - can be used in a maximized UI
  const std::string description_;  ///< Thorough information about what
                             /// the parameter actually does
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_PARAMETER_META_H_
