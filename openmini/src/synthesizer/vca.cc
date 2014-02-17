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

#include "openmini/src/modulators/envelopgenerator_base.h"
#include "openmini/src/modulators/adsd.h"
#include "openmini/src/modulators/factory.h"

namespace openmini {
namespace synthesizer {

Vca::Vca()
  : // Default on Triangle
    generator_(modulators::CreateEnvelopGenerator()),
    attack_(0),
    decay_(0),
    release_(0),
    sustain_level_(0.0f),
    update_(false) {
  ASSERT(generator_ != nullptr);
}

Vca::~Vca() {
  ASSERT(generator_ != nullptr);
  modulators::DestroyEnvelopGenerator(generator_);
}

void Vca::TriggerOn(void) {
  ASSERT(generator_ != nullptr);
  ProcessParameters();
  generator_->TriggerOn();
}

void Vca::TriggerOff(void) {
  ASSERT(generator_ != nullptr);
  ProcessParameters();
  generator_->TriggerOff();
}

Sample Vca::operator()(SampleRead input) {
  ASSERT(generator_ != nullptr);
  ProcessParameters();
  // TODO(gm): this cast should not be done:
  // the generator is actually known at compile-time,
  // so find an elegant way to statically do this
  return openmini::Mul(input,
    FillWithGenerator(*(static_cast<modulators::Adsd*>(generator_))));
}

void Vca::SetAttack(const unsigned int attack) {
  if (attack != attack_) {
    attack_ = attack;
    update_ = true;
  }
}

void Vca::SetDecay(const unsigned int decay) {
  if (decay != decay_) {
    decay_ = decay;
    update_ = true;
  }
}

void Vca::SetRelease(const unsigned int release) {
  if (release != release_) {
    release_ = release;
    update_ = true;
  }
}

void Vca::SetSustain(const float sustain_level) {
  ASSERT(sustain_level <= 1.0f);
  ASSERT(sustain_level >= 0.0f);

  if (sustain_level != sustain_level_) {
    sustain_level_ = sustain_level;
    update_ = true;
  }
}

void Vca::ProcessParameters(void) {
  ASSERT(generator_ != nullptr);
  if (update_) {
    generator_->SetParameters(attack_, decay_, release_, sustain_level_);
    update_ = false;
  }
}

modulators::Section Vca::GetCurrentSection(void) const {
  return generator_->GetCurrentSection();
}

}  // namespace synthesizer
}  // namespace openmini
