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

// std::min
#include <algorithm>

#include "soundtailor/src/filters/filter_base.h"
#include "soundtailor/src/filters/moog_lowaliasnonlinear.h"

#include "openmini/src/synthesizer/parameters.h"

namespace openmini {
namespace synthesizer {

Vcf::Vcf()
  : dry_filter_(new soundtailor::filters::MoogLowAliasNonLinear()),
    wet_filter_(new soundtailor::filters::MoogLowAliasNonLinear()),
    attack_(0),
    decay_(0),
    sustain_level_(0.0f),
    frequency_(0.0f),
    resonance_(0.0f),
    amount_(0.0f),
    update_(false) {
  OPENMINI_ASSERT(dry_filter_ != nullptr);
  OPENMINI_ASSERT(wet_filter_ != nullptr);
}

Vcf::~Vcf() {
  OPENMINI_ASSERT(dry_filter_ != nullptr);
  OPENMINI_ASSERT(wet_filter_ != nullptr);
  delete dry_filter_;
  delete wet_filter_;
}

void Vcf::TriggerOn(void) {
  ProcessParameters();
  contour_gen_.TriggerOn();
}

void Vcf::TriggerOff(void) {
  ProcessParameters();
  contour_gen_.TriggerOff();
}

void Vcf::SetFrequency(const float frequency) {
  OPENMINI_ASSERT(frequency >= soundtailor::filters::MoogLowAliasNonLinear::Meta().freq_min);
  OPENMINI_ASSERT(frequency <= soundtailor::filters::MoogLowAliasNonLinear::Meta().freq_max);

  // TODO(gm): find a way to do this generically
  if (frequency != frequency_) {
    frequency_ = frequency;
    update_ = true;
  }
}

void Vcf::SetResonance(const float resonance) {
  OPENMINI_ASSERT(resonance >= soundtailor::filters::MoogLowAliasNonLinear::Meta().res_min);
  OPENMINI_ASSERT(resonance <= soundtailor::filters::MoogLowAliasNonLinear::Meta().res_max);

  // TODO(gm): find a way to do this generically
  if (resonance != resonance_) {
    resonance_ = resonance;
    update_ = true;
  }
}

void Vcf::SetAttack(const unsigned int attack) {
  OPENMINI_ASSERT(attack <= kMaxTime);

  // TODO(gm): find a way to do this generically
  if (attack != attack_) {
    attack_ = attack;
    update_ = true;
  }
}

void Vcf::SetDecay(const unsigned int decay) {
  OPENMINI_ASSERT(decay <= kMaxTime);

  // TODO(gm): find a way to do this generically
  if (decay != decay_) {
    decay_ = decay;
    update_ = true;
  }
}

void Vcf::SetSustain(const float sustain_level) {
  OPENMINI_ASSERT(sustain_level <= 1.0f);
  OPENMINI_ASSERT(sustain_level >= 0.0f);

  // TODO(gm): find a way to do this generically
  if (sustain_level != sustain_level_) {
    sustain_level_ = sustain_level;
    update_ = true;
  }
}

void Vcf::SetAmount(const float amount) {
  OPENMINI_ASSERT(amount <= 1.0f);
  OPENMINI_ASSERT(amount >= 0.0f);

  // TODO(gm): find a way to do this generically
  if (amount != amount_) {
    amount_ = amount;
    update_ = true;
  }
}

Sample Vcf::operator()(SampleRead sample) {
  OPENMINI_ASSERT(dry_filter_ != nullptr);
  OPENMINI_ASSERT(wet_filter_ != nullptr);
  ProcessParameters();
  const Sample dry(MulConst((1.0f - amount_), (*dry_filter_)(sample)));
  const Sample wet(MulConst(amount_, (*wet_filter_)(sample)));
  // Update filter contour based on last envelop generator output
  wet_filter_->SetParameters(ComputeContour(), resonance_);
  return Add(dry, wet);
}

void Vcf::ProcessParameters(void) {
  OPENMINI_ASSERT(dry_filter_ != nullptr);
  OPENMINI_ASSERT(wet_filter_ != nullptr);
  if (update_) {
    dry_filter_->SetParameters(frequency_, resonance_);
    wet_filter_->SetParameters(frequency_, resonance_);
    contour_gen_.SetParameters(attack_, decay_, decay_, sustain_level_);
    update_ = false;
  }
}

float Vcf::ComputeContour(void) {
  // TODO(gm): Decide if accumulation is allowed for filter contour generator
  const float base_value(std::max(0.0f, std::min(1.0f, contour_gen_())));
  OPENMINI_ASSERT(base_value >= 0.0f);
  OPENMINI_ASSERT(base_value <= 1.0f);
  // Update modulation as much as the filter itself will be updated
  // TODO(gm): this should be simpler when the envelop generator outputs Sample
  for (unsigned int i(1); i < openmini::SampleSize; ++i) {
    contour_gen_();
  }
  // Adaptation from normalized range [0.0 ; 1.0]
  // into [frequency_ ; max allowed filter frequency]
  return base_value * (soundtailor::filters::MoogLowAliasNonLinear::Meta().freq_max - frequency_) + frequency_;
}

}  // namespace synthesizer
}  // namespace openmini
