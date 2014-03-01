/// @filename tests_parameters.cc
/// @brief Parameters specific tests
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

#include "openmini/src/synthesizer/parameter_meta.h"
#include "openmini/src/synthesizer/parameters.h"
#include "openmini/src/synthesizer/parameters_manager.h"

// Using declarations for tested class
using openmini::synthesizer::ParameterMeta;
using openmini::synthesizer::ParametersManager;

// Using declarations for parameters
using openmini::synthesizer::Parameters::kParametersMeta;
using openmini::synthesizer::Parameters::kCount;

/// @brief Test parameters count
static const unsigned int kTestParamsCount(kCount);

/// @bref Test class, since ParametersManager is abstract
class TestParametersManager : public ParametersManager {
 public:
  TestParametersManager(const std::array<ParameterMeta, kCount>& params)
      : ParametersManager(params) {
    // Nothing to do here
  }

  void ProcessParameters(void) {
    // Nothing to do here! overloaded in order to have a concrete class
  }

  /// @brief For testing purpose only: exposing raw value getter
  virtual float GetRawValue(const int param_id) const {
    return ParametersManager::GetRawValue(param_id);
  }
};

/// @brief Parameters bounds random distribution
static std::uniform_real_distribution<float> kBoundsDistribution(
                                                     -10000.0f, 10000.0f);

/// @brief Due to intermediate computation the parameters value set-and-get
/// roundtrip may not be perfect. Here is the allowzed inaccuracy upper bound.
/// This is a relative epsilon!
static const float kRoundTripEpsilon(1e-6f);

/// @brief Set all parameters to random values with normalized bounds,
/// retrieve them, check for no value change
TEST(Parameters, SetGetNormalized) {
  std::array<float, kTestParamsCount> values;
  // Fill random values for inputs
  std::generate(values.begin(),
                values.end(),
                std::bind(kNormPosDistribution, kRandomGenerator));

  const std::array<ParameterMeta, kTestParamsCount> TestParametersMeta{{}};
  TestParametersManager manager(TestParametersMeta);

  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    manager.SetValue(param_id, values[param_id]);
  }
  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    EXPECT_NEAR(values[param_id],
                manager.GetValue(param_id),
                kRoundTripEpsilon * values[param_id]);
  }
}

/// @brief Check setting min through normalization
TEST(Parameters, SetGetMin) {
  // This time using random bounds
  const std::array<ParameterMeta, kTestParamsCount> TestParametersMeta
    = {{ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator))
      }};
  TestParametersManager manager(TestParametersMeta);

  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    manager.SetValue(param_id, 0.0f);
  }
  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    EXPECT_NEAR(
      TestParametersMeta[param_id].min(),
      manager.GetRawValue(param_id),
      kRoundTripEpsilon * std::fabs(TestParametersMeta[param_id].min()));
  }
}

/// @brief Check setting max through normalization
TEST(Parameters, SetGetMax) {
  const std::array<ParameterMeta, kTestParamsCount> TestParametersMeta
    = {{ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator))
      }};
  TestParametersManager manager(TestParametersMeta);

  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    manager.SetValue(param_id, 1.0f);
  }
  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    EXPECT_NEAR(
      TestParametersMeta[param_id].max(),
      manager.GetRawValue(param_id),
      kRoundTripEpsilon * std::fabs(TestParametersMeta[param_id].max()));
  }
}

/// @brief Set all parameters to random values with normalized bounds,
/// retrieve them, check for no value change
TEST(Parameters, SetGetUnnormalized) {
  std::array<float, kTestParamsCount> values;
  std::generate(values.begin(),
              values.end(),
              std::bind(kNormPosDistribution, kRandomGenerator));

  const std::array<ParameterMeta, kTestParamsCount> TestParametersMeta
    = {{ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator)),
        ParameterMeta(kBoundsDistribution(kRandomGenerator),
                      kBoundsDistribution(kRandomGenerator))
      }};
  TestParametersManager manager(TestParametersMeta);
  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    manager.SetValue(param_id, values[param_id]);
  }
  for (unsigned int param_id(0); param_id < kTestParamsCount; ++param_id) {
    EXPECT_NEAR(values[param_id],
                manager.GetValue(param_id),
                kRoundTripEpsilon * std::fabs(values[param_id]));
  }
}
