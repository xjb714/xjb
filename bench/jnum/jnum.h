/*******************************************
* SPDX-License-Identifier: MIT             *
* Copyright (C) 2019-.... Jing Leng        *
* Contact: Jing Leng <lengjingzju@163.com> *
* URL: https://github.com/lengjingzju/json *
*******************************************/
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * json_type_t - the number value type
 * @description: The definition is consistent with the JSON object type in json.h
 */
/*
 * json_type_t - 数值类型
 * @description: 定义和json.h中的对象类型一致
 */
typedef enum {
    JNUM_NULL,
    JNUM_BOOL,
    JNUM_INT,
    JNUM_HEX,
    JNUM_LINT,
    JNUM_LHEX,
    JNUM_DOUBLE,
} jnum_type_t;

/*
 * json_type_t - the number value
 * @description: The definition is consistent with the JSON number object value in json.h
 */
/*
 * json_type_t - 数值
 * @description: 定义和json.h中的数值对象值一直
 */
typedef union {
    bool vbool;
    int32_t vint;
    uint32_t vhex;
    int64_t vlint;
    uint64_t vlhex;
    double vdbl;
} jnum_value_t;

/*
 * jnum_itoa - Converts signed integer to decimal string
 * jnum_ltoa - Converts signed long integer to decimal string
 * jnum_htoa - Converts unsigned integer to hexadecimal string (prefixed with 0x)
 * jnum_lhtoa - Converts unsigned long integer to hexadecimal string (prefixed with 0x)
 * jnum_dtoa - Converts double-precision float to shortest precise string representation
 * @num: IN, input number to convert
 * @buffer: OUT, buffer to store resulting string
 * @return: Length of converted string
 * @description: Buffer must have at least 27 characters capacity
 */
/*
 * jnum_itoa - 打印有符号整数成十进制字符串
 * jnum_ltoa - 打印有符号长整数成十进制字符串
 * jnum_htoa - 打印无符号整数成十六进制字符串，字符串中已含0x
 * jnum_lhtoa - 打印无符号长整数成十六进制字符串，字符串中已含0x
 * jnum_dtoa - 打印双精度浮点数成最短且精确的字符串
 * @num: IN, 被转换的数值
 * @buffer: OUT, 存储装换成的字符串
 * @return: 返回转换成的字符串长度
 * @description: buffer长度至少为27位
 */
int jnum_itoa(int32_t num, char *buffer);
int jnum_ltoa(int64_t num, char *buffer);
int jnum_htoa(uint32_t num, char *buffer);
int jnum_lhtoa(uint64_t num, char *buffer);
int jnum_dtoa(double num, char *buffer);

/*
 * jnum_parse_num - Parses string to numerical value
 * @num: IN, input string to parse
 * @type: OUT, stores detected numerical type
 * @value: OUT, stores parsed numerical value
 * @return: Number of characters consumed during parsing
 * @note: jnum_parse_num does not skip leading whitespace - use jnum_parse for whitespace skipping;
 *        Subsequent `ato?` functions internally call jnum_parse and convert value to requested type
 */
/*
 * jnum_parse_num - 解析字符串到数值
 * @num: IN, 被转换的字符串
 * @type: OUT, 存储数值的类型
 * @value: OUT, 存储数值的值
 * @return: 返回解析的字符串长度
 * @description: jnum_parse_num不会跳过首部空字符，需要跳过空字符时请使用jnum_parse；
 *               后面的 `ato?` 都是调用jnum_parse，并将数值转换成对应类型的数值
 */
int jnum_parse_num(const char *str, jnum_type_t *type, jnum_value_t *value);

static inline int jnum_parse(const char *str, jnum_type_t *type, jnum_value_t *value)
{
    const char *s = str;
    while (1) {
        switch (*s) {
        case '\b': case '\f': case '\n': case '\r': case '\t': case '\v': case ' ': ++s; break;
        default: goto next;
        }
    }
next:
    return (int)(jnum_parse_num(s, type, value) + (s - str));
}

#define jnum_to_func(rtype, fname)                      \
rtype fname(const char *str)                            \
{                                                       \
    jnum_type_t type;                                   \
    jnum_value_t value;                                 \
    rtype val = 0;                                      \
    jnum_parse(str, &type, &value);                     \
    switch (type) {                                     \
    case JNUM_BOOL:   val = (rtype)value.vbool;break;   \
    case JNUM_INT:    val = (rtype)value.vint; break;   \
    case JNUM_HEX:    val = (rtype)value.vhex; break;   \
    case JNUM_LINT:   val = (rtype)value.vlint;break;   \
    case JNUM_LHEX:   val = (rtype)value.vlhex;break;   \
    case JNUM_DOUBLE: val = (rtype)value.vdbl; break;   \
    default:          val = 0;                 break;   \
    }                                                   \
    return val;                                         \
}

#if defined(_MSC_VER)
/* MSVC's performance deteriorates after using inline */
int32_t jnum_atoi(const char *str);
int64_t jnum_atol(const char *str);
uint32_t jnum_atoh(const char *str);
uint64_t jnum_atolh(const char *str);
double jnum_atod(const char *str);
#else
static inline jnum_to_func(int32_t, jnum_atoi)
static inline jnum_to_func(int64_t, jnum_atol)
static inline jnum_to_func(uint32_t, jnum_atoh)
static inline jnum_to_func(uint64_t, jnum_atolh)
static inline jnum_to_func(double, jnum_atod)
#endif

#ifdef __cplusplus
}
#endif
