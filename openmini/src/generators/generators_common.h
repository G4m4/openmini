/// @filename generators_common.h
/// @brief OpenMini common stuff for all generators - declarations
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

#ifndef OPENMINI_SRC_GENERATORS_GENERATORS_COMMON_H_
#define OPENMINI_SRC_GENERATORS_GENERATORS_COMMON_H_

// std::min, std::max
#include <algorithm>

#include "openmini/src/common.h"
#include "openmini/src/generators/generator_base.h"

namespace openmini {
namespace generators {

/// @brief Basic sawtooth signal generator
/// Generates a lot of aliasing, not to be used straight to audio
class ALIGN PhaseAccumulator : public Generator_Base {
 public:
  explicit PhaseAccumulator(const float phase = 0.0f);
  virtual Sample operator()(void);
  virtual void SetPhase(const float phase);
  virtual void SetFrequency(const float frequency);
  virtual float Phase(void) const;

 protected:
  Sample phase_;  ///< Instantaneous phase of the generator

 private:
  Sample increment_;
};

/// @brief Basic differentiator
/// implementing a simple 1st-order differentiator, unitary gain
class Differentiator {
 public:
  explicit Differentiator(const float last = 0.0f);
  Sample operator()(SampleRead sample);

 private:
  float last_;  ///< Last synthesized sample value
};

/// @brief One pole signal filter class
class OnePoleFilter {
 public:
  /// Default constructor: user has to provide the filter coefficients
  ///
  /// @param[in]   b0_     Gain value
  /// @param[in]   b1_     Pole value
  OnePoleFilter(const double b0_, const double b1_);
  void init(void);
  float operator()(const float input);

 private:
  // No assignment operator for this class
  OnePoleFilter& operator=(const OnePoleFilter& right);

  const double b0_;  ///< Gain value
  const double b1_;  ///< Pole value
  float last_;  ///< Last processed sample value
};

// Miscellaneous

/// @brief Helper function: increment the input and wraps it into [-1.0 ; 1.0[
///
/// @param[in]  input         Input to be wrapped - supposed not to be < 1.0
/// @param[in]  increment     Increment to add to the input
///@return the incremented output in [-1.0 ; 1.0[
Sample IncrementAndWrap(SampleRead input, SampleRead increment);

/// @brief Helper function: Clamp input into [min ; max]
template <typename TypeInput>
void Clamp(TypeInput* input, const TypeInput min, const TypeInput max) {
    *input = std::min(std::max(*input, min), max);
}

/// @brief Helper function: Sign of the input (branchless implementation)
template <typename TypeInput>
int GetSign(const TypeInput input) {
  const TypeInput zero = static_cast<TypeInput>(0);
  return (zero < input) - (input < zero);
}

/// @brief Simple linear interpolation
///
/// --------|--------------x--|-----------
///       left              right
///         |<    ratio   >|
///
/// @param[in]  left     Previous value
/// @param[in]  right    Next value
/// @param[out] ratio    Location of the required interpolated value
///
/// @return The interpolated value
float LinearInterpolation(
    const float left,
    const float right,
    const float ratio);

}  // namespace generators
}  // namespace openmini

#endif  // OPENMINI_SRC_GENERATORS_GENERATORS_COMMON_H_
