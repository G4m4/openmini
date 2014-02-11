/// @filename synthesizer.h
/// @brief Main header: main Synthesizer object definitions
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

#ifndef OPENMINI_SRC_SYNTHESIZER_SYNTHESIZER_H_
#define OPENMINI_SRC_SYNTHESIZER_SYNTHESIZER_H_

#include <array>

#include "openmini/src/synthesizer/mixer.h"
#include "openmini/src/synthesizer/parameter_manager.h"
#include "openmini/src/synthesizer/resampler.h"
#include "openmini/src/synthesizer/vcf.h"

namespace openmini {
namespace synthesizer {

/// @brief Synthesizer: main object, everything lives within it
class Synthesizer : public ParametersManager {
 public:
  /// @brief Default constructor
  Synthesizer();

  /// @brief Process function for one buffer
  ///
  /// @param[out]   output      Output buffer to write into
  /// @param[in]    length      Output buffer length
  void ProcessAudio(float* const output, const unsigned int length);

  /// @brief Trigger the given note ID on
  ///
  /// The note must be within the allowed range [kMinKeyNote ; kMaxKeyNote]
  ///
  /// @param[in]    note      Note to trig
  void NoteOn(const unsigned int note);

  /// @brief Stop the given note ID
  ///
  /// The note must be within the allowed range [kMinKeyNote ; kMaxKeyNote]
  ///
  /// @param[in]    note      Note to stop
  void NoteOff(const unsigned int note);

  /// @brief Set the output sampling frequency
  ///
  /// @param[in]  freq    Output sampling frequency
  void SetOutputSamplingFrequency(const float freq);

 protected:
  /// @brief Asynchronous parameters update
  ///
  /// (overriden from inherited class)
  void ProcessParameters(void);

 private:
  Mixer mixer_;  ///< Mixer object for VCOs management
  Vcf filter_;  ///< Filter object
  Resampler buffer_;  ///< Adapter object for output audio stream matching

  std::array<Sample, openmini::kBlockSize / SampleSize> internal_buf_;  ///< Internal memory
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_SYNTHESIZER_H_
