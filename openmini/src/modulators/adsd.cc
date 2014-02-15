/// @filename adsd.cc
/// @brief Envelop generator using Attack-Decay-Sustain-Decay (ADSD) model
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

#include "openmini/src/maths.h"

#include "openmini/src/modulators/adsd.h"

namespace openmini {
namespace modulators {

/// @brief Default max level for the apogee of the attack
static const float kMaxAmplitude(1.0f);

Adsd::Adsd()
    : current_increment_(0.0),
      current_section_(kZero),
      current_value_(0.0f),
      sustain_level_(0.0f),
      cursor_(0),
      attack_(0),
      decay_(0),
      actual_decay_(0),
      actual_release_(0) {
  // Nothing to do here for now
}
Adsd::~Adsd() {
  // Nothing to do here for now
}

void Adsd::TriggerOn(void) {
  cursor_ = 0;
  current_section_ = kAttack;
  current_increment_ = ComputeIncrement(kMaxAmplitude, attack_);
}

void Adsd::TriggerOff(void) {
  current_section_ = kRelease;
  current_increment_ = ComputeIncrement(-current_value_, decay_);
  actual_release_ = cursor_ + decay_;
}

float Adsd::operator()(void) {
  switch (current_section_) {
    case(kAttack): {
      current_value_ += static_cast<float>(current_increment_);
      if (cursor_ >= attack_) {
        current_section_ = GetNextSection(current_section_);
        current_increment_ = ComputeIncrement(sustain_level_ - current_value_,
                                              decay_);
      }
      break;
    }
    case(kDecay): {
      current_value_ += static_cast<float>(current_increment_);
      if (cursor_ >= actual_decay_) {
        current_section_ = GetNextSection(current_section_);
        current_value_ = sustain_level_;
      }
      break;
    }
    case(kSustain): {
      // Nothing to do here
      break;
    }
    case(kRelease): {
      if (cursor_ >= actual_release_) {
        current_section_ = GetNextSection(current_section_);
      } else {
        current_value_ += static_cast<float>(current_increment_);
      }
      break;
    }
    case(kZero): {
      return 0.0f;
    }
    default: {
      // Should never happen
      ASSERT(false);
    }
  }  // switch(current_section_)

  cursor_ += 1;

  return current_value_;
}

void Adsd::SetParameters(const unsigned int attack,
                         const unsigned int decay,
                         const unsigned int release,
                         const float sustain_level) {
  IGNORE(release);
  attack_ = attack;
  decay_ = decay;
  actual_decay_ = decay + attack;
  sustain_level_ = sustain_level;
  current_section_ = kZero;
}

double Adsd::ComputeIncrement(const float rise, const unsigned int run) {
  return static_cast<double>(rise) / run;
}

}  // namespace modulators
}  // namespace openmini
