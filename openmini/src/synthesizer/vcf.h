/// @filename vcf.h
/// @brief "VCF": wrapper around an internal filter
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

#ifndef OPENMINI_SRC_SYNTHESIZER_VCF_H_
#define OPENMINI_SRC_SYNTHESIZER_VCF_H_

#include "openmini/src/common.h"
#include "openmini/src/maths.h"

#include "soundtailor/src/modulators/adsd.h"

// SoundTailor forward declarations
namespace soundtailor {
namespace filters {
class Filter_Base;
}  // namespace filters
}  // namespace soundtailor

namespace openmini {
namespace synthesizer {

/// @brief Vcf: wraps an internal filter and gives it
/// additional parameters as well as a more advanced parameters management.
///
/// It handles everything about asynchronous parameters update.
class Vcf {
 public:
  /// @brief Default constructor
  Vcf();
  /// @brief Default destructor
  ~Vcf();

  /// @brief Event for triggering the beginning of the contour envelop
  void TriggerOn(void);

  /// @brief Event for triggering the end of the contour envelop
  void TriggerOff(void);

  /// @brief Set the filter to the given frequency
  ///
  /// Frequency is not normalized here - the unit is Hz
  ///
  /// @param[in]    frequency         Frequency to set the filter to
  void SetFrequency(const float frequency);
  /// @brief Set the filter to the given resonance
  ///
  /// This is not normalized, resonance within [0.0f ; 100.0f]
  ///
  /// @param[in]  resonance   Resonance to set the filter to
  void SetResonance(const float resonance);
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
  /// @brief Set the given sustain level
  ///
  /// The parameter is normalized here, supposed to be within [0.0f ; 1.0f]
  ///
  /// @param[in]  sustain_level   Level when sustaining
  void SetSustain(const float sustain_level);
  /// @brief Set the amount, e.g. Dry/Wet setting
  ///
  /// The parameter is normalized here, supposed to be within [0.0f ; 1.0f]
  ///
  /// @param[in]  amount   Amount of filter contour: 0.0 -> null
  void SetAmount(const float amount);
  /// @brief Actual process function for one sample
  Sample operator()(SampleRead sample);
  /// @brief Update internal generator parameters
  ///
  /// Allows asynchronous updates; to be called within an update loop.
  void ProcessParameters(void);

 private:
  /// @brief Internal helper wrapper for filter contour computation
  ///
  /// @return the new filter contour (e.g. its new cutoff frequency)
  float ComputeContour(void);
  // No assignment operator for this class
  Vcf& operator=(const Vcf& right);

  // TODO(gm): polymorphism probably no longer useful here
  soundtailor::filters::Filter_Base* dry_filter_;  ///< Internal dry filter
  soundtailor::filters::Filter_Base* wet_filter_;  ///< Internal wet filter
  soundtailor::modulators::Adsd contour_gen_;  ///< Internal envelop generator

  unsigned int attack_; ///< Envelop attack time (due to asynchronous update,
                        ///< it may as well be the value to be applied soon
  unsigned int decay_; ///< Envelop decay time (due to asynchronous update,
                        ///< Same as above.
  float sustain_level_; ///< Envelop sustain level (normalized).
                        ///< Same as above.
  float frequency_; ///< Frequency of the filter (non-normalized, in Hz).
                    /// (due to asynchronous update,
                    ///< it may as well be the frequency to be applied soon)
  float resonance_; ///< Resonance of the filter
                  ///< Same as above.
  float amount_; ///< Dry/Wet tuning
  bool update_;  ///< True if any parameter was updated since the last call to
                 ///< ProcessParameters()
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_VCF_H_
