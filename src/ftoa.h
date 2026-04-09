// author : xjb
// src : github.com/xjb714/xjb
// date : 2026.4.9

#ifndef XJB_FTOA_H
#define XJB_FTOA_H

/**
 * Convert a float to string
 * @param v The float value to convert
 * @param buf The buffer to write the result to (must be at least 17 bytes)
 * @return A pointer to the end of the written string (including the null terminator)
 */
char* xjb_ftoa(float v, char *buf);

/**
 * Convert a double to string
 * @param v The double value to convert
 * @param buf The buffer to write the result to (must be at least 33 bytes)
 * @return A pointer to the end of the written string (including the null terminator)
 */
char* xjb_ftoa(double v, char *buf);

#endif // XJB_FTOA_H
