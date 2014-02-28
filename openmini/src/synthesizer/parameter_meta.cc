/// @filename parameter_meta.cc
/// @brief Parameter "Meta" class implementation
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

#include "openmini/src/synthesizer/parameter_meta.h"

namespace openmini {
namespace synthesizer {

ParameterMeta::ParameterMeta(float min,
                             float max,
                             float default_value,
                             int sig_figs,
                             int cardinality,
                             const std::string& name,
                             const std::string& description)
    : min_(min),
      max_(max),
      default_value_(default_value),
      sig_figs_(sig_figs),
      cardinality_(cardinality),
      name_(name),
      description_(description) {
  // Nothing to do here for now
}

float ParameterMeta::min(void) const {
  return min_;
}
float ParameterMeta::max(void) const {
  return max_;
}
float ParameterMeta::default_value(void) const {
  return default_value_;
}
int ParameterMeta::sig_figs(void) const {
  return sig_figs_;
}
int ParameterMeta::cardinality(void) const {
  return cardinality_;
}
const std::string& ParameterMeta::name(void) const {
  return name_;
}
const std::string& ParameterMeta::description(void) const {
  return description_;
}

}  // namespace synthesizer
}  // namespace openmini
