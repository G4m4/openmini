/// @filename vca.h
/// @brief "VCA", voltage-controlled amplifier
/// Wrapper around a basic envelop generator - definitions
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

#ifndef OPENMINI_SRC_SYNTHESIZER_VCA_H_
#define OPENMINI_SRC_SYNTHESIZER_VCA_H_

#include "openmini/src/common.h"
#include "openmini/src/maths.h"

// Internal forward declarations
namespace openmini {
namespace modulators {
class EnvelopGenerator_Base;
}  // namespace modulators
}  // namespace openmini

namespace openmini {
namespace synthesizer {

/// @brief "Voltage controlled amplifier": wraps an envelop generator,
/// and allow to apply the envelop as a modulation to an input signal.
///
/// It handles everything about asynchronous parameters update.
class Vca {
 public:
  /// @brief Default constructor
  Vca();
  /// @brief Default destructor
  ~Vca();

  /// @brief Event for triggering the beginning of the envelop
  ///
  /// Note that the amplitude start value is the one applied when triggering.
  /// e.g. repeated calls to this function will lead to accumulated envelops.
  void TriggerOn(void);

  /// @brief Event for triggering the end of the envelop
  ///
  /// Note that this does not mean that the envelop stops right after this
  /// function was called, depending on the parameters currently in use
  void TriggerOff(void);
  /// @brief Set the given attack time
  ///
  /// The parameter is not normalized here - the unit is "samples"
  ///
  /// @param[in]  attack    Attack time for the envelop
  void SetAttack(const unsigned int attack);
  /// @brief Set the given decay time
  ///
  /// The parameter is not normalized here - the unit is "samples"
  ///
  /// @param[in]  decay    Decay time for the envelop
  void SetDecay(const unsigned int decay);
  /// @brief Set the given release time
  ///
  /// The parameter is not normalized here - the unit is "samples"
  ///
  /// @param[in]  attack    Release time for the envelop
  void SetRelease(const unsigned int release);
  /// @brief Set the given sustain level
  ///
  /// The parameter is normalized here, supposed to be within [0.0f ; 1.0f]
  ///
  /// @param[in]  sustain_level   Level when sustaining
  void SetSustain(const float sustain_level);
  /// @brief Actual process function for one sample
  Sample operator()(SampleRead input);
  /// @brief Update internal generator parameters
  ///
  /// Allows asynchronous updates; to be called within an update loop.
  void ProcessParameters(void);

 private:
  // No assignment operator for this class
  Vca& operator=(const Vca& right);

  modulators::EnvelopGenerator_Base* generator_;  ///< Envelop generator
  unsigned int attack_; ///< Envelop attack time (due to asynchronous update,
                        ///< it may as well be the value to be applied soon
  unsigned int decay_; ///< Envelop decay time (due to asynchronous update,
                        ///< Same as above.
  unsigned int release_; ///< Envelop release time (due to asynchronous update,
                        ///< Same as above.
  float sustain_level_; ///< Envelop sustain level (normalized).
                        ///< Same as above.
  bool update_;  ///< True if any parameter was updated since the last call to
                 ///< ProcessParameters()
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_VCA_H_
