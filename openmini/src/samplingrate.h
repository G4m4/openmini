/// @filename samplingrate.h
/// @brief SamplingRate singleton - simple holder for the sampling rate
/// @author gm
/// @copyright gm 2014
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

#ifndef OPENMINI_SRC_SAMPLINGRATE_H_
#define OPENMINI_SRC_SAMPLINGRATE_H_

namespace openmini {

/// @brief Unique instance of the sampling rate
///
/// Implemented as a singleton in order to make it accessible from everywhere,
/// with only a single instance of it.
//TODO(gm): Make it thread-safe
class SamplingRate {
 public:
  /// @brief Instance getter
  ///
  /// @return the current (and unique) class instance
  static SamplingRate& Instance(void);

  /// @brief Getter
  ///
  /// @return the current sampling rate
  float Get(void) const;
/// @brief Getter
  ///
  /// @return half the current sampling rate
  float GetHalf(void) const;
  /// @brief Setter
  ///
  /// @param[in]  sampling_rate    New sampling rate to set (in Hertz)
  void Set(const float sampling_rate);

 private:
  /// @brief Default constructor (not accessible from ext.)
  SamplingRate();
  SamplingRate(const SamplingRate&);
  SamplingRate& operator=(const SamplingRate&);

  float sampling_rate_;  ///< Actual sampling rate value in Hertz
};

}  // namespace openmini

#endif  // OPENMINI_SRC_SAMPLINGRATE_H_
