// Copyright 2026 xjb714 and contributors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef XJB_FTOA_H
#define XJB_FTOA_H

/**
 * Convert a float to string
 * @param v The float value to convert
 * @param buf The buffer to write the result to (must be at least 17 bytes)
 * @return A pointer to the end of the written string (including the null terminator)
 */
char* xjb_ftoa(float v, char* buf);

/**
 * Convert a double to string
 * @param v The double value to convert
 * @param buf The buffer to write the result to (must be at least 33 bytes)
 * @return A pointer to the end of the written string (including the null terminator)
 */
char* xjb_ftoa(double v, char* buf);

#endif  // XJB_FTOA_H
