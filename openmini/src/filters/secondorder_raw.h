/// @filename secondorder_raw.h
/// @brief Low Pass filter using a simple ("raw") 2nd order implementation
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

#ifndef OPENMINI_SRC_FILTERS_SECONDORDER_RAW_H_
#define OPENMINI_SRC_FILTERS_SECONDORDER_RAW_H_

#include "openmini/src/common.h"
#include "openmini/src/filters/filter_base.h"

namespace openmini {
namespace filters {

/// @brief 2nd order low pass filter
/// using the most simple (and computationally efficient) implementation
class ALIGN SecondOrderRaw : public Filter_Base {
 public:
  SecondOrderRaw();
  virtual ~SecondOrderRaw() {
    // Nothing to do here for now
  };
  virtual Sample operator()(const Sample& sample);
  virtual void SetResonance(const float resonance);
  virtual void SetCutoffFrequency(const float frequency);

 protected:
  float zeros_;  //< Filter "zeros" coefficients (a0-an)
  float poles_;  //< Filter "poles" coefficients (b0-bn)
};

}  // namespace filters
}  // namespace openmini

#endif  // OPENMINI_SRC_FILTERS_SECONDORDER_RAW_H_
