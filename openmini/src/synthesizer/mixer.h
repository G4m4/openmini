/// @filename mixer.h
/// @brief Mixer object declarations
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

#ifndef OPENMINI_SRC_SYNTHESIZER_MIXER_H_
#define OPENMINI_SRC_SYNTHESIZER_MIXER_H_

namespace openmini {
namespace synthesizer {

// Internal forward declarations
class Vco;

/// @brief: Number of VCOs a Mixer handles
static const int kVCOsCount(3);

/// @brief Mixer: VCOs manager
///
/// Mixer is responsible for managing multiple VCOs - including spawning them.
/// It routes their different parameters, and sum their signal
/// output into one mono signal.
///
/// The number of managed VCOs is fixed at compile-time.
class Mixer {
 public:
  /// @brief Default constructor
  Mixer();
  ~Mixer();

  /// @brief Process function for one buffer
  ///
  /// Note that the buffer size is fixed: @see kBlockSize
  ///
  /// @param[out]    output    Output buffer to write into
  void ProcessAudio(float* const output, const int length);

  /// @brief Trigger the given note ID on
  ///
  /// The note must be within the allowed range [kMinKeyNote ; kMaxKeyNote]
  ///
  /// @param[in]    note      Note to trig
  void NoteOn(const int note);

  /// @brief Stop the given note ID
  ///
  /// The note must be within the allowed range [kMinKeyNote ; kMaxKeyNote]
  ///
  /// @param[in]    note      Note to stop
  void NoteOff(const int note);

 private :
  Vco* vcos_[kVCOsCount];  ///< List of VCOs
  float vcos_gain_[kVCOsCount];  ///< List of VCOs gain
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_MIXER_H_
