//  Copyright 2022 Google LLC.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_MATH_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_MATH_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

__BEGIN_DECLS

#define NAN (0.0f / 0.0f)
inline bool isnan(double x) { return x != x; }
#define signbit(x) ((x) >= 0.0 ? 1 : 0)

// Returns value with the magnitued of X and sign of Y.
inline float copysignf(float x, float y) { return y >= 0.0f ? x : -x; }
inline double copysign(double x, double y) { return y >= 0.0 ? x : -x; }
inline long double copysignl(long double x, long double y) {
  return y >= 0.0 ? x : -x;
}

inline float fabsf(float value) {
  return value >= 0.0f ? value : -value;
}
inline double fabs(double value) {
  return value >= 0.0 ? value : -value;
}

// These functions are stubbed out and not implemented.
float ceilf(float value);
double ceil(double value);
float floorf(float value);
double floor(double value);
float truncf(float value);
double trunc(double value);
float sqrtf(float value);
double sqrt(double value);
float rintf(float value);
double rint(double value);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_MATH_H_
