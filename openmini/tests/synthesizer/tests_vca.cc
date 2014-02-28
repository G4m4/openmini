/// @filename tests_vca.cc
/// @brief VCA specific tests
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

#include "openmini/tests/tests.h"

#include "openmini/src/generators/generators_common.h"
#include "openmini/src/synthesizer/vca.h"

// Using declarations for tested class
using openmini::synthesizer::Vca;
// Using declaration for differentiator
using openmini::generators::Differentiator;

/// @brief Time parameters allowed max
static const unsigned int kMaxTime(kDataTestSetSize / 4);
/// @brief Time parameters random generator
static std::uniform_int_distribution<unsigned int> kTimeDistribution(0,
                                                                     kMaxTime);

/// @brief Length of the tail to check after each envelop
static const unsigned int kTail(256);

/// @brief Modulate a sinus, check for its range (must be within [-1.0 ; 1.0])
TEST(Vca, Range) {
  const float kFrequency(1000.0f);
  SinusGenerator input_signal(kFrequency, SamplingRate::Instance().Get());
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random parameters
    const unsigned int kAttack(kTimeDistribution(kRandomGenerator));
    const unsigned int kDecay(kTimeDistribution(kRandomGenerator));
    const unsigned int kSustain(kTimeDistribution(kRandomGenerator));
    const float kSustainLevel(kNormPosDistribution(kRandomGenerator));

    Vca modulator;
    modulator.SetAttack(kAttack);
    modulator.SetDecay(kDecay);
    modulator.SetSustain(kSustainLevel);

    const float kEpsilon(1e-4f);

    modulator.TriggerOn();
    unsigned int i(0);
    while (i < kAttack + kDecay * 2 + kTail) {
      if ((i >= kAttack + kDecay + kSustain)
          && ((openmini::modulators::kSustain == modulator.GetCurrentSection())
          || (openmini::modulators::kDecay == modulator.GetCurrentSection()))
          ) {
        modulator.TriggerOff();
      }
      const Sample input(FillWithFloatGenerator(input_signal));
      const Sample amplitude(Abs(modulator(input)));
      EXPECT_TRUE(LessEqual(0.0f - kEpsilon, amplitude));
      EXPECT_TRUE(GreaterEqual(1.0f + kEpsilon, amplitude));
      i += openmini::SampleSize;
    }
  }  // iterations?
}

/// @brief Used in the following test.
/// TODO(gm): get rid of this by using a more robust system (std::functional)
struct VcaFunctor {
  VcaFunctor(Vca* vca,
             SampleRead constant)
    : vca_(vca),
      constant_(constant),
      differentiator_() {
    // Nothing to do here
  }

  Sample operator()(void) {
    return differentiator_(vca_->operator()(constant_));
  }

 private:
  // No assignment operator for this class
  VcaFunctor& operator=(const VcaFunctor& right);

  Vca* vca_;
  const Sample constant_;
  Differentiator differentiator_;
};

/// @brief Modulates a constant, check for its proper slopes:
/// - when in attack samples should be in a continuous upward slope
/// - when in decay samples should be in a continuous downward slope
/// - when in sustain samples should all be equal
/// - when in release samples should be in a continuous downward slope
TEST(Vca, Timings) {
  const Sample kConstant(Fill(0.5f));
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    // Random parameters
    // Although using multiple of the sample size is easier in order to get
    // "right" sections (e.f. ones beginning/ending on samples)
    const unsigned int kAttack(GetNextMultiple(
      kTimeDistribution(kRandomGenerator),
      openmini::SampleSize));
    const unsigned int kDecay(GetNextMultiple(
      kTimeDistribution(kRandomGenerator),
      openmini::SampleSize));
    const unsigned int kSustain(GetNextMultiple(
      kTimeDistribution(kRandomGenerator),
      openmini::SampleSize));
    const float kSustainLevel(kNormPosDistribution(kRandomGenerator));

    Vca modulator;
    modulator.SetAttack(kAttack);
    modulator.SetDecay(kDecay);
    modulator.SetSustain(kSustainLevel);

    modulator.TriggerOn();
    std::vector<unsigned int> zero_crossing_indexes;

    // TODO(gm): get rid of that
    VcaFunctor vca_functor(&modulator, kConstant);
    ZeroCrossing<VcaFunctor> zero_crossing(vca_functor);
    unsigned int kTriggerOnLength(kAttack + kDecay + kSustain);
    unsigned int kTotalLength(kTriggerOnLength + kDecay + kTail);
    // 2-sample difference occurs due to differentiation and trigger unevenness
    unsigned int kEpsilon(2);
    unsigned int zero_crossing_idx(
      zero_crossing.GetNextZeroCrossing(kTriggerOnLength));
    while (zero_crossing_idx < kTriggerOnLength) {
      zero_crossing_indexes.push_back(zero_crossing_idx);
      zero_crossing_idx = zero_crossing.GetNextZeroCrossing(kTriggerOnLength);
    }
    modulator.TriggerOff();
    while (zero_crossing_idx < kTotalLength) {
      zero_crossing_idx = zero_crossing.GetNextZeroCrossing(kTotalLength);
      zero_crossing_indexes.push_back(zero_crossing_idx);
    }
    // Remove too close indexes
    // TODO(gm): this should not have to be done,
    // improve zero crossing detection
    RemoveClose(&zero_crossing_indexes,
                4);
    EXPECT_NEAR(kAttack, zero_crossing_indexes[1], kEpsilon);
    EXPECT_NEAR(kAttack + kDecay, zero_crossing_indexes[2], kEpsilon);
    EXPECT_NEAR(kTriggerOnLength, zero_crossing_indexes[3], kEpsilon);
    EXPECT_NEAR(kTriggerOnLength + kDecay, zero_crossing_indexes[4], kEpsilon);
  }  // iterations?
}

/// @brief Modulates using a "click envelop" - with both timing parameters null
TEST(Vca, ClickEnvelop) {
  const float kFrequency(1000.0f);
  SinusGenerator input_signal(kFrequency, SamplingRate::Instance().Get());
  for (unsigned int iterations(0); iterations < kIterations; ++iterations) {
    IGNORE(iterations);

    const unsigned int kAttack(0);
    const unsigned int kDecay(0);
    const unsigned int kSustain(kTimeDistribution(kRandomGenerator));
    const float kSustainLevel(kNormPosDistribution(kRandomGenerator));

    Vca modulator;
    modulator.SetAttack(kAttack);
    modulator.SetDecay(kDecay);
    modulator.SetSustain(kSustainLevel);

    modulator.TriggerOn();
    // Since vectorized we have to ignore (the length worst case) samples
    unsigned int i(0);
    while (i < 4) {
      const Sample input(FillWithFloatGenerator(input_signal));
      IGNORE(modulator(input));
      i += 1;
    }
    while (i <= kSustain) {
      const Sample input(FillWithFloatGenerator(input_signal));
      const Sample sample(modulator(input));
      EXPECT_TRUE(Equal(MulConst(kSustainLevel, input), sample));
      i += 1;
    }
    modulator.TriggerOff();
    // Same here, ignoring (the length worst case) samples, due to the release
    const Sample input(FillWithFloatGenerator(input_signal));
    IGNORE(modulator(input));
    while (i <= kSustain + kTail) {
      const Sample input(FillWithFloatGenerator(input_signal));
      const Sample sample(modulator(input));
      EXPECT_TRUE(Equal(Fill(0.0f), sample));
      i += 1;
    }
  }  // iterations?
}
