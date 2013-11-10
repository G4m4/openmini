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

extern "C"
{
#include <emmintrin.h>
#include <mmintrin.h>
}

#include "openmini/src/common.h"

namespace openmini {
  
typedef union {
  Sample sample_v;
  __m128i integer_v;
} ConverterFloatIntVector;

typedef union {
  Sample sample_v;
  float sample[4];
} ConverterFloatScalarVector;

static inline Sample Fill(const float value) {
  return _mm_set1_ps(value);
}

static inline Sample Fill(const float a,
                          const float b,
                          const float c,
                          const float d) {
  return _mm_set_ps(a, b, c, d);
}

static inline Sample FillIncremental(const float base,
                                     const float increment) {
  return _mm_set_ps(base + increment * 3.0f,
                    base + increment * 2.0f,
                    base + increment,
                    base);
}

template<unsigned i>
float GetByIndex(const Sample value) {
  ConverterFloatScalarVector converter;
  converter.sample_v = value;
  return converter.sample[i];
}

static inline Sample Add(const Sample left, const Sample right) {
  return _mm_add_ps(left, right);
}

static inline float AddHorizontal(const Sample value) {
  return GetByIndex<0>(value)
         + GetByIndex<1>(value)
         + GetByIndex<2>(value)
         + GetByIndex<3>(value);
}

static inline Sample Sub(const Sample left, const Sample right) {
  return _mm_sub_ps(left, right);
}

static inline Sample Mul(const Sample left, const Sample right) {
  return _mm_mul_ps(left, right);
}

static inline Sample MulConst(const float constant, const Sample right) {
  return _mm_mul_ps(Fill(constant), right);
}

static inline Sample Abs(const Sample value) {
  return _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), value), value);
}

static inline Sample RotateOnRight(const Sample vector,
                                   const float value) {
  ConverterFloatIntVector converter;
  converter.sample_v = vector;
  const __m128i tmp(converter.integer_v);
  const __m128i rotated(_mm_slli_si128(tmp, 4));
  ConverterFloatIntVector converter_back;
  converter_back.integer_v = rotated;
  return Fill(GetByIndex<3>(converter_back.sample_v),
              GetByIndex<2>(converter_back.sample_v),
              GetByIndex<1>(converter_back.sample_v),
              value);
}

static inline Sample Sgn(const Sample value) {
  const Sample kZero(_mm_setzero_ps());
  const Sample kOne(Fill(1.0f));
  const Sample kMinus(Fill(-1.0f));
  const Sample kPlusMask(_mm_and_ps(_mm_cmpgt_ps(value, kZero), kOne));
  const Sample kMinusMask(_mm_and_ps(_mm_cmplt_ps(value, kZero), kMinus));
  return Add(kPlusMask, kMinusMask);
}

static inline void Store(float* const buffer, const Sample value) {
  _mm_storeu_ps(buffer, value);
}

}  // namespace openmini

#endif  // OPENMINI_SRC_MATHS_H_
