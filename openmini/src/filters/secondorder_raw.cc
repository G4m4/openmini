/// @filename secondorder_raw.cc
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

// std::sin, std::cos
#include <cmath>

#include "openmini/src/maths.h"

#include "openmini/src/filters/secondorder_raw.h"

namespace openmini {
namespace filters {

SecondOrderRaw::SecondOrderRaw()
    : Filter_Base(),
      gain_(0.0f) {
  // Nothing to do here for now
  coeffs_.fill(0.0f);
  history_.fill(0.0f);
}

Sample SecondOrderRaw::operator()(SampleRead sample) {
  // Direct Form 1 implementation:
  // the Direct Form 2, although usually more efficient, has issues with
  // time-varying parameters

  // Vector = (x_{n}, x_{n + 1}, x_{n + 2}, x_{n + 3})
  // previous = (x_{n - 1}, x_{n}, x_{n + 1}, x_{n + 2})
  // last = (x_{n - 2}, x_{n - 1}, x_{n}, x_{n + 1})
  const Sample previous(RotateOnRight(sample, history_[1]));
  const Sample last(RotateOnRight(previous, history_[0]));
  const Sample current(MulConst(gain_, sample));
  // previous *= b1
  const Sample previous_gain(MulConst(coeffs_[1], previous));
  // previous *= b2
  const Sample last_gain(MulConst(coeffs_[0], last));
  // All weighted inputs cumulated sum
  const Sample tmp_sum(Add(Add(current, previous_gain), last_gain));

#if (_USE_SSE)
  const float oldest_out(GetByIndex<0>(tmp_sum)
                         + history_[2] * coeffs_[2]
                         + history_[3] * coeffs_[3]);
  const float old_out(GetByIndex<1>(tmp_sum)
                      + history_[3] * coeffs_[2]
                      + oldest_out * coeffs_[3]);
  const float new_out(GetByIndex<2>(tmp_sum)
                      + oldest_out * coeffs_[2]
                      + old_out * coeffs_[3]);
  const float newest_out(GetByIndex<3>(tmp_sum)
                         + old_out * coeffs_[2]
                         + new_out * coeffs_[3]);
  const Sample out(Fill(newest_out, new_out, old_out, oldest_out));
  const Sample history(TakeEachRightHalf(out, sample));
  Store(history_.data(), history);
#else
  const float out(tmp_sum
                  + history_[2] * coeffs_[2]
                  + history_[3] * coeffs_[3]);
  history_[0] = history_[1];
  history_[1] = sample;
  history_[2] = history_[3];
  history_[3] = out;
#endif  // (_USE_SSE)

  return out;
}

void SecondOrderRaw::SetParameters(const float frequency,
                                   const float resonance) {
  // Based on Audio EQ Cookbook material
  ASSERT(resonance >= 0.0f);
  // Required due to imprecisions
  ASSERT(frequency >= kMinFilterFreq);
  ASSERT(frequency <= kMaxFilterFreq);

  // Computations done in double since precision is crucial here
  const double kFrequency(frequency);
  const double kResonance(resonance);

  const double kOmega(2.0 * Pi * kFrequency);
  const double kSinOmega(std::sin(kOmega));
  const double kCosOmega(std::cos(kOmega));
  const double kAlpha(kSinOmega / (2.0 * kResonance));

  // Actual coefficients
  const double b0 = (1.0 - kCosOmega) / 2.0;
  const double b1 = (1.0 - kCosOmega);
  const double b2 = (1.0 - kCosOmega) / 2.0;
  const double a0 = 1.0 + kAlpha;
  const double a1 = -2.0 * kCosOmega;
  const double a2 = 1.0 - kAlpha;

  // Assigning normalized coefficients
  gain_ = static_cast<float>(b0 / a0);
  coeffs_[0] = static_cast<float>(b2 / a0);
  coeffs_[1] = static_cast<float>(b1 / a0);
  coeffs_[2] = static_cast<float>(-a2 / a0);
  coeffs_[3] = static_cast<float>(-a1 / a0);
}

}  // namespace filters
}  // namespace openmini
