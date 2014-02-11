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

#include "openmini/src/filters/factory.h"
#include "openmini/src/filters/filter_base.h"
#include "openmini/src/synthesizer/parameters.h"

namespace openmini {
namespace synthesizer {

Vcf::Vcf()
  : filter_(filters::CreateFilter()),
    frequency_(0.0f),
    qfactor_(0.0f),
    update_(false) {
  ASSERT(filter_ != nullptr);
}

Vcf::~Vcf() {
  ASSERT(filter_ != nullptr);
  filters::DestroyFilter(filter_);
}

void Vcf::SetFrequency(const float frequency) {
  ASSERT(frequency >= 0.0f);
  ASSERT(frequency <= 1.0f);

  // De-normalization
  // TODO(gm): find a way to do this generically
  const float actual_freq(frequency * (Parameters::kParametersMeta[Parameters::kFilterFreq].max()
                                       - Parameters::kParametersMeta[Parameters::kFilterFreq].min())
                          + Parameters::kParametersMeta[Parameters::kFilterFreq].min());
  ASSERT(actual_freq >= kMinFilterFreq);
  ASSERT(actual_freq <= kMaxFilterFreq);
  // TODO(gm): find a way to do this generically
  if (actual_freq != frequency_) {
    frequency_ = actual_freq;
    update_ = true;
  }
}

void Vcf::SetQFactor(const float qfactor) {
  ASSERT(qfactor <= 100.0f);
  ASSERT(qfactor >= 0.0f);

  // TODO(gm): find a way to do this generically
  if (qfactor != qfactor_) {
    qfactor_ = qfactor;
    update_ = true;
  }
}

Sample Vcf::operator()(SampleRead sample) {
  ASSERT(filter_ != nullptr);
  ProcessParameters();
  return (*filter_)(sample);
}

void Vcf::ProcessParameters(void) {
  ASSERT(filter_ != nullptr);
  if (update_) {
    filter_->SetParameters(frequency_, qfactor_);
    update_ = false;
  }
}

}  // namespace synthesizer
}  // namespace openmini
