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
  /// @brief Set the filter to the given frequency
  ///
  /// Frequency is not normalized here - the unit is Hz
  ///
  /// @param[in]    frequency         Frequency to set the filter to
  void SetFrequency(const float frequency);
  /// @brief Set the filter to the given q factor
  ///
  /// This is not normalized, Q factor within [0.0f ; 100.0f]
  ///
  /// @param[in]    qfactor         Q factor to set the filter to
  void SetQFactor(const float qfactor);
  /// @brief Actual process function for one sample
  Sample operator()(SampleRead sample);
  /// @brief Update internal generator parameters
  ///
  /// Allows asynchronous updates; to be called within an update loop.
  void ProcessParameters(void);

 private:
  // No assignment operator for this class
  Vcf& operator=(const Vcf& right);

  // TODO(gm): polymorphism probably no longer useful here
  soundtailor::filters::Filter_Base* filter_;  ///< Internal filter
  float frequency_; ///< Frequency of the filter (non-normalized, in Hz).
                    /// (due to asynchronous update,
                    ///< it may as well be the frequency to be applied soon)
  float qfactor_; ///< Q factor of the filter
                  ///< Same as above.
  bool update_;  ///< True if any parameter was updated since the last call to
                 ///< ProcessParameters()
};

}  // namespace synthesizer
}  // namespace openmini

#endif  // OPENMINI_SRC_SYNTHESIZER_VCF_H_
