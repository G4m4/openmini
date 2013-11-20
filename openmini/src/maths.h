/// @filename maths.h
/// @brief OpenMini common maths header
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

#ifndef OPENMINI_SRC_MATHS_H_
#define OPENMINI_SRC_MATHS_H_

#include "openmini/src/configuration.h"

#if (_USE_SSE)
extern "C" {
#include <emmintrin.h>
#include <mmintrin.h>
#include <pmmintrin.h>
}
#endif  // (_USE_SSE)

#include <cmath>

#include "openmini/src/common.h"

namespace openmini {

#if (_USE_SSE)
typedef union {
  Sample sample_v;
  __m128i integer_v;
} ConverterFloatIntVector;

typedef union {
  Sample sample_v;
  float sample[4];
} ConverterFloatScalarVector;
#endif  // (_USE_SSE)

static inline Sample Fill(const float value) {
#if (_USE_SSE)
  return _mm_set1_ps(value);
#else
  return value;
#endif  // (_USE_SSE)
}

static inline Sample Fill(const float* value) {
#if (_USE_SSE)
  return _mm_loadu_ps(value);
#else
  return *value;
#endif  // (_USE_SSE)
}

static inline Sample Fill(const float a,
                          const float b,
                          const float c,
                          const float d) {
#if (_USE_SSE)
  return _mm_set_ps(a, b, c, d);
#else
  IGNORE(a);
  IGNORE(b);
  IGNORE(c);
  return d;
#endif  // (_USE_SSE)
}

static inline Sample FillIncremental(const float base,
                                     const float increment) {
  return Fill(base + increment * 3.0f,
              base + increment * 2.0f,
              base + increment,
              base);
}

static inline Sample FillOnLength(const float value) {
  const float actual_value(value * openmini::SampleSize);
  return Fill(actual_value);
}

#if (_USE_SSE)
template<unsigned i>
float GetByIndex(SampleRead value) {
  ConverterFloatScalarVector converter;
  converter.sample_v = value;
  return converter.sample[i];
}
#else
template<unsigned i>
float GetByIndex(SampleRead value) {
  return value;
}
#endif  // (_USE_SSE)

#if (_USE_SSE)
static inline float GetByIndex(SampleRead value, const unsigned i) {
  ConverterFloatScalarVector converter;
  converter.sample_v = value;
  return converter.sample[i];
}
#else
static inline float GetByIndex(SampleRead value, const unsigned i) {
  IGNORE(i);
  return value;
}
#endif  // (_USE_SSE)

static inline Sample Add(SampleRead left, SampleRead right) {
#if (_USE_SSE)
  return _mm_add_ps(left, right);
#else
  return left + right;
#endif  // (_USE_SSE)
}

static inline float AddHorizontal(SampleRead value) {
#if (_USE_SSE)
  const Sample first_add(_mm_hadd_ps(value, value));
  const Sample shuffled(_mm_shuffle_ps(first_add, first_add,
                                       _MM_SHUFFLE(0, 1, 0, 1)));
  return GetByIndex<0>(_mm_hadd_ps(shuffled, first_add));
#else
  return value;
#endif  // (_USE_SSE)
}

static inline Sample Sub(SampleRead left, SampleRead right) {
#if (_USE_SSE)
  return _mm_sub_ps(left, right);
#else
  return left - right;
#endif  // (_USE_SSE)
}

static inline Sample Mul(SampleRead left, SampleRead right) {
#if (_USE_SSE)
  return _mm_mul_ps(left, right);
#else
  return left * right;
#endif  // (_USE_SSE)
}

static inline Sample MulConst(const float constant, SampleRead right) {
#if (_USE_SSE)
  return Mul(Fill(constant), right);
#else
  return constant * right;
#endif  // (_USE_SSE)
}

static inline Sample Abs(SampleRead value) {
#if (_USE_SSE)
  return _mm_max_ps(Sub(_mm_setzero_ps(), value), value);
#else
  return std::fabs(value);
#endif  // (_USE_SSE)
}

static inline Sample RotateOnRight(SampleRead vector,
                                   const float value) {
#if (_USE_SSE)
  const Sample rotated(_mm_castsi128_ps(
                       _mm_slli_si128(_mm_castps_si128(vector), 4)));
  return Add(_mm_set_ss(value), rotated);
#else
  IGNORE(vector);
  return value;
#endif  // (_USE_SSE)
}

static inline Sample Sgn(SampleRead value) {
#if (_USE_SSE)
  const Sample kZero(_mm_setzero_ps());
  const Sample kOne(Fill(1.0f));
  const Sample kMinus(Fill(-1.0f));
  const Sample kPlusMask(_mm_and_ps(_mm_cmpgt_ps(value, kZero), kOne));
  const Sample kMinusMask(_mm_and_ps(_mm_cmplt_ps(value, kZero), kMinus));
  return Add(kPlusMask, kMinusMask);
#else
  return Sub((0.0f < value), (value < 0.0f));
#endif  // (_USE_SSE)
}

static inline void Store(float* const buffer, SampleRead value) {
#if (_USE_SSE)
  _mm_storeu_ps(buffer, value);
#else
  *buffer = value;
#endif  // (_USE_SSE)
}

#if (_USE_SSE)
/// @brief Get each right half of the two given vectors
///
/// Given left = (x0, x1, x2, x3) and right = (y0, y1, y2, y3)
/// it will return (x2, x3, y2, y3)
static inline Sample TakeEachRightHalf(SampleRead left,
                                       SampleRead right) {
  return _mm_shuffle_ps(right, left, _MM_SHUFFLE(3, 2, 3, 2));
}
#endif  // (_USE_SSE)

/// @brief Revert the givne vector values order
///
/// Given value = (x0, x1, x2, x3)
/// it will return (x3, x2, x1, x0)
static inline Sample Revert(SampleRead value) {
#if (_USE_SSE)
  return _mm_shuffle_ps(value, value, _MM_SHUFFLE(0, 1, 2, 3));
#else
  return value;
#endif  // (_USE_SSE)
}

}  // namespace openmini

#endif  // OPENMINI_SRC_MATHS_H_
