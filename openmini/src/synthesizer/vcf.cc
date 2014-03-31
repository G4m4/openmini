/// @filename vcf.cc
/// @brief "VCF": wrapper around an internal filter - implementation
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

#include "openmini/src/synthesizer/vcf.h"

#include "soundtailor/src/filters/filter_base.h"
#include "soundtailor/src/filters/secondorder_raw.h"

#include "openmini/src/synthesizer/parameters.h"

namespace openmini {
namespace synthesizer {

Vcf::Vcf()
  : filter_(new soundtailor::filters::SecondOrderRaw()),
    frequency_(0.0f),
    resonance_(0.0f),
    update_(false) {
  OPENMINI_ASSERT(filter_ != nullptr);
}

Vcf::~Vcf() {
  OPENMINI_ASSERT(filter_ != nullptr);
  delete filter_;
}

void Vcf::SetFrequency(const float frequency) {
  OPENMINI_ASSERT(frequency >= kMinFilterFreq);
  OPENMINI_ASSERT(frequency <= kMaxFilterFreq);

  // TODO(gm): find a way to do this generically
  if (frequency != frequency_) {
    frequency_ = frequency;
    update_ = true;
  }
}

void Vcf::SetResonance(const float resonance) {
  OPENMINI_ASSERT(resonance <= 100.0f);
  OPENMINI_ASSERT(resonance >= 0.0f);

  // TODO(gm): find a way to do this generically
  if (resonance != resonance_) {
    resonance_ = resonance;
    update_ = true;
  }
}
    update_ = true;
  }
}

Sample Vcf::operator()(SampleRead sample) {
  OPENMINI_ASSERT(filter_ != nullptr);
  ProcessParameters();
  return (*filter_)(sample);
}

void Vcf::ProcessParameters(void) {
  OPENMINI_ASSERT(filter_ != nullptr);
  if (update_) {
    filter_->SetParameters(frequency_, qfactor_);
    update_ = false;
  }
}

}  // namespace synthesizer
}  // namespace openmini
