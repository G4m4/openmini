/// @filename vco.h
/// @brief "VCO", voltage-controlled oscillators:
/// Wrapper around a basic generator - definitions
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

#ifndef OPENMINI_SRC_SYNTHESIZER_VCO_H_
#define OPENMINI_SRC_SYNTHESIZER_VCO_H_

#include "openmini/src/common.h"

// Internal forward declarations
namespace openmini {
namespace generators {
class Generator_Base;
}  // namespace generators
}  // namespace openmini

namespace openmini {
namespace synthesizer {

/// @brief Simple implementation of a circular buffer
///
/// Resizable, FIFO-type container; its general philosophy is that,
/// if one operation could not be done (pushing too much data, etc.)
/// it asserts - there are no return values nor exceptions.
class Vco {
 public:
  /// @brief Default constructor, set the form this VCO is supposed to generate
  ///
  /// @param[in]   waveform      Signal waveform to be generated
  Vco(void);
  /// @brief Default destructor
  ~Vco();
  /// @brief Set the VCO to the given frequency
  ///
  /// Frequency is not normalized here - the unit is Hz
  ///
  /// @param[in]    frequency         Frequency to set the VCO to
  void SetFrequency(const float frequency);
  /// @brief Set the VCO to the given volume
  ///
  /// This is normalized! Volume within [0.0f ; 1.0f]
  ///
  /// @param[in]    volume         Volume to set the VCO to
  void SetVolume(const float volume);
  /// @brief Set the VCO to the given waveform
  ///
  /// @param[in]    value          Waveform type to set the VCO to
  void SetWaveform(const Waveform::Type value);
  /// @brief Actual process function for one sample
  float operator()(void);
  /// @brief Update internal generator parameters
  ///
  /// Allows asynchronous updates; to be called within an update loop.
  void ProcessParameters(void);

 private:
  // No assignment operator for this class
  Vco& operator=(const Vco& right);

  generators::Generator_Base* generator_;  ///< Internal generator
  float volume_; ///< Volume of the generator (due to asynchronous update,
                 ///< it may as well be the volume to be applied soon
  float frequency_; ///< Frequency of the generator. Same as above.
  Waveform::Type waveform_;  ///< Waveform of the generator. Same as above.
  bool update_;  ///< True if any parameter was updated since the last call to
                 ///< ProcessParameters()
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_VCO_H_
