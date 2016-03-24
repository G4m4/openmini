/// @filename vca.cc
/// @brief "VCA": Wrapper around a basic envelop generator - implementation
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

#include "openmini/src/synthesizer/vca.h"

#include "soundtailor/src/modulators/adsd.h"

#include "openmini/src/maths.h"

namespace openmini {
namespace synthesizer {

Vca::Vca()
  : // TODO(gm): use a factory?
    generator_(new soundtailor::modulators::Adsd()),
    attack_(0),
    decay_(0),
    sustain_level_(0.0f),
    update_(false) {
  OPENMINI_ASSERT(generator_ != nullptr);
}

Vca::~Vca() {
  OPENMINI_ASSERT(generator_ != nullptr);
  delete generator_;
}

void Vca::TriggerOn(void) {
  OPENMINI_ASSERT(generator_ != nullptr);
  ProcessParameters();
  generator_->TriggerOn();
}

void Vca::TriggerOff(void) {
  OPENMINI_ASSERT(generator_ != nullptr);
  ProcessParameters();
  generator_->TriggerOff();
}

Sample Vca::operator()(SampleRead input) {
  OPENMINI_ASSERT(generator_ != nullptr);
  ProcessParameters();
  // TODO(gm): this cast should not be done:
  // the generator is actually known at compile-time,
  // so find an elegant way to statically do this
  soundtailor::modulators::Adsd* static_generator_ptr
    = static_cast<soundtailor::modulators::Adsd*>(generator_);
  const Sample envelop((*static_generator_ptr)());

  return VectorMath::Mul(input, envelop);
}

void Vca::SetAttack(const unsigned int attack) {
  OPENMINI_ASSERT(attack <= kMaxTime);
  if (attack != attack_) {
    attack_ = attack;
    update_ = true;
  }
}

void Vca::SetDecay(const unsigned int decay) {
  OPENMINI_ASSERT(decay <= kMaxTime);

  if (decay != decay_) {
    decay_ = decay;
    update_ = true;
  }
}

void Vca::SetSustain(const float sustain_level) {
  OPENMINI_ASSERT(sustain_level <= 1.0f);
  OPENMINI_ASSERT(sustain_level >= 0.0f);

  if (sustain_level != sustain_level_) {
    sustain_level_ = sustain_level;
    update_ = true;
  }
}

void Vca::ProcessParameters(void) {
  OPENMINI_ASSERT(generator_ != nullptr);
  if (update_) {
    generator_->SetParameters(attack_, decay_, decay_, sustain_level_);
    update_ = false;
  }
}

}  // namespace synthesizer
}  // namespace openmini
