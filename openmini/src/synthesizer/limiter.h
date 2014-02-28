/// @filename limiter.h
/// @brief Limiter object declarations
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

#ifndef OPENMINI_SRC_SYNTHESIZER_LIMITER_H_
#define OPENMINI_SRC_SYNTHESIZER_LIMITER_H_

#include "openmini/src/common.h"

namespace openmini {
namespace synthesizer {

/// @brief Limiter: simple hard limiter
///
/// Clips everything above the given value, to insure good signal behaviour
class Limiter {
 public:
  /// @brief Default constructor
  explicit Limiter(const float threshold = 1.0f);
  ~Limiter();

  /// @brief Process function for one sample
  ///
  /// @param[in]  input   Input sample
  Sample operator()(SampleRead input);

 private:
  // No assignment operator for this class
  Limiter& operator=(const Limiter& right);

  const Sample threshold_neg_;
  const Sample threshold_pos_;
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_LIMITER_H_
