/*
 * Copyright (c) 2018 YaoYuan <ibireme@gmail.com>.
 * Released under the MIT license (MIT).
 */

#include "benchmark_helper.cpp"
#include "yybench/src/yybench.h"
#include <inttypes.h>

#ifndef BENCHMARK_DATA_PATH
const char *benchmark_get_data_path(void){return "util";}
#define BENCHMARK_DATA_PATH benchmark_get_data_path()
#endif

/*----------------------------------------------------------------------------*/
/**
 A function prototype to convert double to string.
 Null-terminator is required (since we cannot change some lib's source).
 @param val A double number.
 @param buf A string buffer as least 32 bytes
 @return The ending of this string.
 */
typedef char *(*dtoa_func)(double val, char *buf);
typedef char *(*ftoa_func)(float val, char *buf);



/*----------------------------------------------------------------------------*/

typedef union {
    f64 f; u64 u;
} f64_uni;

typedef union {
    f32 f; u32 u;
} f32_uni;

static yy_inline u64 f64_to_u64_raw(f64 f) {
    f64_uni uni;
    uni.f = f;
    return uni.u;
}

static yy_inline f64 f64_from_u64_raw(u64 u) {
    f64_uni uni;
    uni.u = u;
    return uni.f;
}

static yy_inline f32 f32_from_u32_raw(u32 u) {
    f32_uni uni;
    uni.u = u;
    return uni.f;
}

/** Get the number of significant digit from a valid floating number string. */
static yy_inline int f64_str_get_digits(const char *str) {
    const char *hdr = str;
    if (*hdr == '-' || *hdr == '+') hdr++;

    const char *cur = hdr;
    const char *dot = NULL;
    bool has_frac = 0;
    while('0' <= *cur && *cur <= '9') cur++;
    if (*cur == '.') {
        dot = cur;
        cur++;
        while('0' <= *cur && *cur <= '9') {
            has_frac |= (*cur != '0');
            cur++;
        }
    }
    const char *end = cur;

    while (hdr < end && (*hdr == '0' || *hdr == '.')) hdr++;
    if (!has_frac) {
        while (hdr < end && ((*(end - 1) == '0') || *(end - 1) == '.')) end--;
    }
    return (int)(end - hdr) - (hdr < dot && dot < end);
}

/*----------------------------------------------------------------------------*/

typedef f64 (*fill_func)(void);
typedef f64 (*fill_len_func)(int len);

typedef f32 (*fill_func_f32)(void);
typedef f32 (*fill_len_func_f32)(int len);

/** Get random double. */
static yy_inline f64 rand_f64(void) {
    while (true) {
        u64 u = yy_random64();
        f64 f = f64_from_u64_raw(u);
        if (isfinite(f)) return f;
    };
}
static yy_inline f32 rand_f32(void) {
    while (true) {
        u32 u = yy_random32();
        f32 f = f32_from_u32_raw(u);
        if (isfinite(f)) return f;
    };
}

/** Get random float number as double. */
static yy_inline f64 rand_f64_from_f32(void) {
    while (true) {
        u32 u = yy_random32();
        f32 f = f32_from_u32_raw(u);
        if (isfinite(f)) return (f64)f;
    };
}

/** Get random positive double with specified number of digit (in range 1 to 17) */
static yy_inline f64 rand_f64_len(int len) {
    if (len < 1 || len > 17) return 0.0;
    char buf[32];
    int processed = 0;
    while (true) {
        // get random double
        u64 u = yy_random64();
        f64 f = f64_from_u64_raw(u);
        if (!isfinite(f)) continue;
        if (f < 0) f = -f;

        // print with precision
        google_double_to_string_prec(f, len, buf);
        f = google_string_to_double(buf, &processed);
        processed = google_double_to_string(f, buf);
        if (processed == 0 || f64_str_get_digits(buf) != len) continue;

        // read as double
        f = google_string_to_double(buf, &processed);
        if (!isfinite(f)) continue;
        if (processed == 0) continue;

        return f;
    }
}

/** Get random normalized double (in range 0.0 to 1.0). */
static yy_inline f64 rand_f64_normalize(void) {
    char buf[32];
    int processed = 0;

    // write a random double number in range 0.0 to 1.0
    buf[0] = '0';
    buf[1] = '.';
    yy_uint_to_string(yy_random64(), buf + 2);

    // read as double
    return google_string_to_double(buf, &processed);
}

/** Get random normalized double (in range 0.0 to 1.0)
    with specified number of digit (in range 1 to 17) */
static yy_inline f64 rand_f64_normalize_len(int len) {
    if (len < 1 || len > 17) return 0.0;
    char buf[32];
    int processed = 0;
    while (true) {
        // write a random double number in range 0.0 to 1.0
        buf[0] = '0';
        buf[1] = '.';
        yy_uint_to_string(yy_random64_uniform(100000000000000000ULL), buf + 2);
        buf[2 + len] = '\0';

        // read as double
        f64 f = google_string_to_double(buf, &processed);
        if (processed == 0) continue;

        // print to shortest string
        processed = google_double_to_string(f, buf);
        if (processed == 0) continue;
        if (f64_str_get_digits(buf) != len) continue;

        return f;
    }
}

/** Get random double which can convert to integer exactly. */
static yy_inline f64 rand_f64_integer(void) {
    char buf[32];
    int processed = 0;

    // write a random integer number which can fit in double
    yy_uint_to_string(yy_random64_range(1, (u64)1 << 53), buf);

    // read as double
    return google_string_to_double(buf, &processed);
}

/** Get random double which can convert to integer exactly,
    with specified number of digit (in range 1 to 17) */
static yy_inline f64 rand_f64_integer_len(int len) {
    if (len < 1 || len > 17) return 0.0;
    char buf[32];
    int processed = 0;
    while (true) {
        // write a random integer number with length
        yy_uint_to_string(yy_random64_range(100'000'000'000'000'000ULL,
                                            999999999999999999ULL), buf);
        buf[len] = '\0';

        // read as double
        f64 f = google_string_to_double(buf, &processed);
        if (processed == 0) continue;

        // print to shortest string
        processed = google_double_to_string(f, buf);
        if (processed == 0) continue;
        if (f64_str_get_digits(buf) != len) continue;

        return f;
    }
}

/** Get random subnormal double. */
static yy_inline f64 rand_f64_subnormal(void) {
    u64 u = yy_random64_range(0x0000000000001ULL,
                              0xFFFFFFFFFFFFFULL);
    f64 f = f64_from_u64_raw(u);
    return f;
}

/*----------------------------------------------------------------------------*/
//double to string functions
static int func_count = 0;
static dtoa_func func_arr[128];
static char *func_name_arr[128];
static int func_name_max = 0;

//float to string functions
static int func_count_float = 0;
static ftoa_func func_arr_float[128];
static char *func_name_arr_float[128];
static int func_name_max_float = 0;

// returns the function name aligned (inner memory)
static char *get_name_aligned(const char *name, bool align_right) {
    static char buf[64];
    int len = (int)strlen(name);
    int space = func_name_max - len;
    if (align_right) {
        for (int i = 0; i < space; i++) buf[i] = ' ';
        memcpy(buf + space, name, len);
    } else {
        memcpy(buf, name, len);
        for (int i = 0; i < space; i++) buf[len + i] = ' ';
    }
    buf[func_name_max] = '\0';
    return buf;
}



/*----------------------------------------------------------------------------*/

static void dtoa_func_benchmark_all(const char *output_path) {
    printf("initialize...\n");
    yy_cpu_setup_priority();
    yy_cpu_spin(1.0);
    yy_cpu_measure_freq();


    const int num_per_case = 1 << 17; // 131072
    const int meansure_count = 1;

    const int loop_unroll = 2;

    //bool recovery_from_file = true;         // read from file or not
    //std::string file_path = std::string(BENCHMARK_DATA_PATH) + "/dtoa_func_benchmark.txt";

    typedef struct {
        const char *name;
        const char *desc;
        void *fill_func;
        bool func_has_len;
        bool use_random_len;
    } dataset_t;

    dataset_t dataset_arr[64];
    int dataset_num = 0;

    dataset_arr[dataset_num++] = (dataset_t) {
        "random",
        "random double number in all binary range, ignore nan and inf",
        (void*)rand_f64, false, false
    };
    dataset_arr[dataset_num++] = (dataset_t) {
        "random length",
        "random double number with random significant digit count",
        (void*)rand_f64_len, true, true
    };
    dataset_arr[dataset_num++] = (dataset_t) {
        "fixed length",
        "random double number with fixed significant digit count",
        (void*)rand_f64_len, true, false
    };


    dataset_arr[dataset_num++] = (dataset_t) {
        "nomalized",
        "random double number in range 0.0 to 1.0",
        (void*)rand_f64_normalize, false, false
    };
    dataset_arr[dataset_num++] = (dataset_t) {
        "nomalized (random len)",
        "random double number in range 0.0 to 1.0, with random significant digit count",
        (void*)rand_f64_normalize_len, true, true
    };
    dataset_arr[dataset_num++] = (dataset_t) {
        "nomalized (fixed len)",
        "random double number in range 0.0 to 1.0, with fixed significant digit count",
        (void*)rand_f64_normalize_len, true, false
    };

    dataset_arr[dataset_num++] = (dataset_t) {
        "integer",
        "random double number from integer",
        (void*)rand_f64_integer, false, false
    };
    dataset_arr[dataset_num++] = (dataset_t) {
        "integer (random len)",
        "random double number from integer, with random digit count",
        (void*)rand_f64_integer_len, true, true
    };
    dataset_arr[dataset_num++] = (dataset_t) {
        "integer (fixed len)",
        "random double number from integer, with fixed digit count",
        (void*)rand_f64_integer_len, true, false
    };

    dataset_arr[dataset_num++] = (dataset_t) {
        "subnormal",
        "random subnormal double number",
        (void*)rand_f64_subnormal, false, false
    };
    dataset_arr[dataset_num++] = (dataset_t) {
        "float",
        "random float number",
        (void*)rand_f64_from_f32, false, false
    };

    char buf[64];
    //f64 *vals = (f64 *)malloc(num_per_case * sizeof(f64));
    int fixed_len_count = 3; // func_has_len = true; use_random_len=false;
    int all_vals_number = 17 * num_per_case * fixed_len_count + num_per_case * (dataset_num - fixed_len_count);
    std::vector<f64> all_vals;
    all_vals.resize(all_vals_number);
    int all_vals_begin_pos = 0;
    for(int d = 0; d < dataset_num; ++d){
        dataset_t dataset = dataset_arr[d];
        if(dataset.func_has_len && !dataset.use_random_len){// fixed_len_count = 3
            // fixed_len_count ++;
            for(int len = 1; len <= 17; len++){
                yy_random_reset();
                for(int v = 0; v < num_per_case; ++v){
                    double val = ((fill_len_func)dataset.fill_func)(len);
                    all_vals[v + all_vals_begin_pos + (len - 1) * num_per_case] = val;
                }
            }
            all_vals_begin_pos += 17 * num_per_case;
        }else{// dataset_num - fixed_len_count = 11 - 3 = 8
            yy_random_reset();
            if (dataset.func_has_len) {
                for (int v = 0; v < num_per_case; v++) {
                    double val = ((fill_len_func)dataset.fill_func)((int)yy_random32_range(1, 17));
                    all_vals[v + all_vals_begin_pos] = val;
                }
            } else {
                for (int v = 0; v < num_per_case; v++) {
                    double val =  ((fill_func)dataset.fill_func)();
                    all_vals[v + all_vals_begin_pos] = val;
                }
            }
            all_vals_begin_pos += num_per_case;
        }
    }
    yy_report *report = yy_report_new();
    yy_report_add_env_info(report);
    assert(all_vals_begin_pos == all_vals_number);
    all_vals_begin_pos = 0;
    for (int d = 0; d < dataset_num; d++) {
        dataset_t dataset = dataset_arr[d];
        printf("run benchmark %s...", dataset.name);

        yy_chart *chart = yy_chart_new();
        yy_chart_options op;
        yy_chart_options_init(&op);
        op.title = dataset.name;
        op.subtitle = dataset.desc;

        if (dataset.func_has_len && !dataset.use_random_len) {
            op.type = YY_CHART_LINE;
            op.v_axis.title = "average CPU cycles";
            op.v_axis.logarithmic = true;
            op.h_axis.title = "digit count";
            op.h_axis.tick_interval = 1;
            op.plot.point_start = 1;
            op.tooltip.value_decimals = 2;
            op.tooltip.shared = true;
            op.tooltip.crosshairs = true;
            op.width = 800;
            op.height = 540;

            for (int f = 0; f < func_count; f++) {
                const char *func_name = func_name_arr[f];
                dtoa_func func = func_arr[f];
                yy_chart_item_begin(chart, func_name);

                for (int len = 1; len <= 17; len++) {
                    yy_random_reset();
                    for (int i = 0; i < num_per_case; i++) {
                        //vals[i] = ((fill_len_func)dataset.fill_func)(len);
                        //all_vals.push_back(vals[i]);
                    }

                    u64 ticks_min = UINT64_MAX;
                    for (int r = 0; r < meansure_count; r++) {
                        f64* data = (f64*)&all_vals[all_vals_begin_pos + (len - 1) * num_per_case];
                        u64 t1 = yy_time_get_ticks();
                        //const u64 loop_unroll = 4;
                        for (int v = 0; v < num_per_case; v+=loop_unroll) {
                            //f64 val = vals[v];
                            //f64 val = all_vals[v + all_vals_begin_pos + (len - 1) * num_per_case];
                            // f64 val = data[v];
                            // func(val, buf);
                            for(int u=0;u<loop_unroll;u++)func(data[v+u], buf);
                        }
                        u64 t2 = yy_time_get_ticks();
                        u64 t = t2 - t1;
                        if (t < ticks_min) ticks_min = t;
                    }
                    f64 cycle = (f64)ticks_min / (f64)num_per_case * yy_cpu_get_cycle_per_tick();
                    yy_chart_item_add_float(chart, (f32)cycle);
                    
                }

                yy_chart_item_end(chart);
            }
            all_vals_begin_pos += num_per_case * 17;
        } else { // bar chart
            op.type = YY_CHART_BAR;
            op.h_axis.title = "CPU cycles";
            op.plot.value_labels_enabled = true;
            op.plot.value_labels_decimals = 2;
            op.plot.color_by_point = true;
            op.plot.group_padding = 0.0f;
            op.plot.point_padding = 0.1f;
            op.plot.border_width = 0.0f;
            op.legend.enabled = false;
            op.tooltip.value_decimals = 2;
            op.width = 640;
            op.height = 420;

            yy_random_reset();
            if (dataset.func_has_len) {
                for (int i = 0; i < num_per_case; i++) {
                    //vals[i] = ((fill_len_func)dataset.fill_func)((int)yy_random32_range(1, 17));
                    //all_vals.push_back(vals[i]);
                }
            } else {
                for (int i = 0; i < num_per_case; i++) {
                    //vals[i] = ((fill_func)dataset.fill_func)();
                    //all_vals.push_back(vals[i]);
                }
            }

            for (int f = 0; f < func_count; f++) {
                const char *func_name = func_name_arr[f];
                dtoa_func func = func_arr[f];
                u64 ticks_min = UINT64_MAX;
                for (int r = 0; r < meansure_count; r++) {
                    f64* data = (f64*)&all_vals[all_vals_begin_pos];
                    u64 t1 = yy_time_get_ticks();
                    //const u64 loop_unroll = 4;
                    for (int v = 0; v < num_per_case; v+=loop_unroll) {
                        //f64 val = vals[v];
                        //f64 val = all_vals[v + all_vals_begin_pos];
                        // f64 val = data[v];
                        // func(val, buf);
                        for(int u=0;u<loop_unroll;u++)func(data[v+u], buf);
                    }
                    u64 t2 = yy_time_get_ticks();
                    u64 t = t2 - t1;
                    if (t < ticks_min) ticks_min = t;
                }
                f64 cycle = (f64)ticks_min / (f64)num_per_case * yy_cpu_get_cycle_per_tick();
                yy_chart_item_with_float(chart, func_name, (f32)cycle);
            }
            all_vals_begin_pos += num_per_case;
        }
        yy_chart_sort_items_with_value(chart, false);
        yy_chart_set_options(chart, &op);
        yy_report_add_chart(report, chart);
        yy_chart_free(chart);

        printf("[OK]\n");
    }
    
    // export report to html
    bool suc = yy_report_write_html_file(report, output_path);
    if (!suc) {
        printf("write report file failed: %s\n", output_path);
    }
    yy_report_free(report);
    //free(vals);
    //all_vals.clear();
}

static void dtoa_func_verify_all(void) {

    printf("\nIf the compiler is an intel compiler, there may be errors in the verification process. Please ignore them.\n");
    printf("The google_double_to_string function is not working properly on icpx compiler. example : 3.3156184e-316\n");
    double test_value = 3.3156184e-316;
    char buf_test[64];
    google_double_to_string(test_value, buf_test);
    printf("test value: %.16le buf = %s\n", test_value, buf_test);
    printf("if buf content is '3.3156184e-316' , the test is passed.\n");
    printf("if buf content is '0' , the test is failed.\n");
    printf("if test is failed, please add \"-no-ftz\" compile option for intel compiler.\n\n");

    for (int i = 1; i < func_count; i++) { // skip null func
        const char *name = func_name_arr[i];
        dtoa_func func = func_arr[i];
        int func_ulp_err = 0;
        int func_len_err = 0;

        printf("verify %s ", get_name_aligned(name, false));

        // read test data dir
        char data_path[YY_MAX_PATH];
        yy_path_combine(data_path, BENCHMARK_DATA_PATH, "data", NULL);

        int file_count = 0;
        char **file_names = yy_dir_read(data_path, &file_count);
        if (file_count == 0) {
            printf("cannot read test data!\n");
            return;
        }

        // read each test file
        for (int f = 0; f < file_count; f++) {
            char *file_name = file_names[f];
            if (!yy_str_has_prefix(file_name, "real_pass") &&
                !yy_str_has_prefix(file_name, "sint_") &&
                !yy_str_has_prefix(file_name, "uint_")) continue;
            char file_path[YY_MAX_PATH];
            yy_path_combine(file_path, data_path, file_name, NULL);
            yy_dat dat;
            if (!yy_dat_init_with_file(&dat, file_path)) {
                printf("cannot read test file: %s\n", file_path);
                return;
            }

            // read each line
            usize line_len;
            char *line;
            while ((line = yy_dat_copy_line(&dat, &line_len))) {
                if (line_len == 0 || line[0] == '#') continue;
                char str[64]; // some func (such as schubfach) need more space

                // read test string with google
                int processed = 0;
                f64 val1 = google_string_to_double(line, &processed);
                if (!processed) continue;
                if (!isfinite(val1)) continue;
                if (val1 == -0.0) val1 = 0.0; /* some func may write -0.0 as 0.0 */

                // write double to string with google to get shortest digit num
                google_double_to_string(val1, str);
                int min_dig_num1 = f64_str_get_digits(str);

                // write test number with dtoa func
                usize write_len = func(val1, str) - str;
                int min_dig_num2 = f64_str_get_digits(str);

                // read output string with google
                f64 val2 = google_string_to_double(str, &processed);

                // ensure the double value is same
                u64 uval1 = f64_to_u64_raw(val1);
                u64 uval2 = f64_to_u64_raw(val2);
                bool is_same = (uval1 == uval2) && ((int)write_len == processed);

                // ensure the string is shortest
                bool is_shortest = (min_dig_num1 == min_dig_num2);

                bool print_err = 0;
                if (!is_same) {
                    if (print_err) {
                        printf("  func %s error:\n", name);
                        printf("    output: %s\n", str);
                        google_double_to_string(val1, str);
                        printf("    expect: %s\n", str);
                    }
                    func_ulp_err++;
                } else if (!is_shortest) {

                    if (print_err) {
                        printf("  func %s error:\n", name);
                        printf("    output: %s\n", str);
                        google_double_to_string(val1, str);
                        printf("    expect: %s\n", str);
                    }
                    func_len_err++;
                }

                free(line);
            }
            yy_dat_release(&dat);
        }
        yy_dir_free(file_names);

        if (!func_len_err && !func_ulp_err) printf(" [OK]\n");
        else {
            if (func_len_err) printf(" [not_shortest:%d]", func_len_err);
            if (func_ulp_err) printf(" [not_match:%d]", func_ulp_err);
            printf("\n");
        }
    }
}

static void dtoa_func_cleanup(void) {
    func_count = 0;
    func_name_max = 0;
}
static void ftoa_func_cleanup(void) {
    func_count_float = 0;
    func_name_max_float = 0;
}

static void ftoa_func_benchmark_all(const char *output_path) {
    printf("initialize...\n");
    yy_cpu_setup_priority();
    yy_cpu_spin(1.0);
    yy_cpu_measure_freq();


    int num_per_case = (1<<18); //131072
    int meansure_count = 2;

    typedef struct {
        const char *name;
        const char *desc;
        void *fill_func_f32;
        bool func_has_len;
        bool use_random_len;
    } dataset_t;

    dataset_t dataset_arr[64];
    int dataset_num = 0;

    dataset_arr[dataset_num++] = (dataset_t) {
        "random",
        "random float number in all binary range, ignore nan and inf",
        (void*)rand_f32, false, false
    };
    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "random length",
    //     "random double number with random significant digit count",
    //     (void*)rand_f64_len, true, true
    // };
    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "fixed length",
    //     "random double number with fixed significant digit count",
    //     (void*)rand_f64_len, true, false
    // };


    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "nomalized",
    //     "random double number in range 0.0 to 1.0",
    //     (void*)rand_f64_normalize, false
    // };
    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "nomalized (random len)",
    //     "random double number in range 0.0 to 1.0, with random significant digit count",
    //     (void*)rand_f64_normalize_len, true, true
    // };
    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "nomalized (fixed len)",
    //     "random double number in range 0.0 to 1.0, with fixed significant digit count",
    //     (void*)rand_f64_normalize_len, true, false
    // };

    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "integer",
    //     "random double number from integer",
    //     (void*)rand_f64_integer, false
    // };
    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "integer (random len)",
    //     "random double number from integer, with random digit count",
    //     (void*)rand_f64_integer_len, true, true
    // };
    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "integer (fixed len)",
    //     "random double number from integer, with fixed digit count",
    //     (void*)rand_f64_integer_len, true, false
    // };

    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "subnormal",
    //     "random subnormal double number",
    //     (void*)rand_f64_subnormal, false
    // };
    // dataset_arr[dataset_num++] = (dataset_t) {
    //     "float",
    //     "random float number",
    //     (void*)rand_f64_from_f32, false
    // };
    char buf[64];
    f32 *vals = (f32 *)malloc(num_per_case * sizeof(f32));
    yy_report *report = yy_report_new();
    yy_report_add_env_info(report);
    for (int d = 0; d < dataset_num; d++) {
        dataset_t dataset = dataset_arr[d];
        printf("run benchmark %s...", dataset.name);

        yy_chart *chart = yy_chart_new();
        yy_chart_options op;
        yy_chart_options_init(&op);
        op.title = dataset.name;
        op.subtitle = dataset.desc;

        // if (dataset.func_has_len && !dataset.use_random_len) {
        //     op.type = YY_CHART_LINE;
        //     op.v_axis.title = "average CPU cycles";
        //     op.v_axis.logarithmic = true;
        //     op.h_axis.title = "digit count";
        //     op.h_axis.tick_interval = 1;
        //     op.plot.point_start = 1;
        //     op.tooltip.value_decimals = 2;
        //     op.tooltip.shared = true;
        //     op.tooltip.crosshairs = true;
        //     op.width = 800;
        //     op.height = 540;

        //     for (int f = 0; f < func_count; f++) {
        //         const char *func_name = func_name_arr[f];
        //         dtoa_func func = func_arr[f];
        //         yy_chart_item_begin(chart, func_name);

        //         for (int len = 1; len <= 17; len++) {
        //             yy_random_reset();
        //             for (int i = 0; i < num_per_case; i++) {
        //                 vals[i] = ((fill_len_func)dataset.fill_func)(len);
        //             }

        //             u64 ticks_min = UINT64_MAX;
        //             for (int r = 0; r < meansure_count; r++) {
        //                 u64 t1 = yy_time_get_ticks();
        //                 for (int v = 0; v < num_per_case; v++) {
        //                     f32 val = vals[v];
        //                     func(val, buf);
        //                 }
        //                 u64 t2 = yy_time_get_ticks();
        //                 u64 t = t2 - t1;
        //                 if (t < ticks_min) ticks_min = t;
        //             }
        //             f64 cycle = (f64)ticks_min / (f64)num_per_case * yy_cpu_get_cycle_per_tick();
        //             yy_chart_item_add_float(chart, (f32)cycle);
        //         }

        //         yy_chart_item_end(chart);
        //     }

        // }
        // else
        { // bar chart
            op.type = YY_CHART_BAR;
            op.h_axis.title = "CPU cycles";
            op.plot.value_labels_enabled = true;
            op.plot.value_labels_decimals = 2;
            op.plot.color_by_point = true;
            op.plot.group_padding = 0.0f;
            op.plot.point_padding = 0.1f;
            op.plot.border_width = 0.0f;
            op.legend.enabled = false;
            op.tooltip.value_decimals = 2;
            op.width = 640;
            op.height = 420;
            yy_random_reset();
            if (dataset.func_has_len) {
                for (int i = 0; i < num_per_case; i++) {
                    vals[i] = ((fill_len_func_f32)dataset.fill_func_f32)((int)yy_random32_range(1, 17));
                }
            } else {
                for (int i = 0; i < num_per_case; i++) {
                    vals[i] = ((fill_func_f32)dataset.fill_func_f32)();
                }
            }
            for (int f = 0; f < func_count_float; f++) {
                const char *func_name = func_name_arr_float[f];
                ftoa_func func = func_arr_float[f];
                u64 ticks_min = UINT64_MAX;
                for (int r = 0; r < meansure_count; r++) {
                    u64 t1 = yy_time_get_ticks();
                    const u64 loop_unroll = 2;
                    for (u64 v = 0; v < num_per_case; v+=loop_unroll) {
                        for( u64 u = 0; u < loop_unroll; u++)
                        {
                            //f32 val = vals[v+u];
                            func(vals[v+u], buf);
                        }
                    }
                    u64 t2 = yy_time_get_ticks();
                    u64 t = t2 - t1;
                    if (t < ticks_min) ticks_min = t;
                }
                f64 cycle = (f64)ticks_min * (1.0 / (f64)num_per_case) * yy_cpu_get_cycle_per_tick();
                printf("\n%s: %.4lf cycles, %llu ticks", func_name, cycle, (unsigned long long)ticks_min);
                yy_chart_item_with_float(chart, func_name, (f32)cycle);

            }
        }
        yy_chart_sort_items_with_value(chart, false);
        yy_chart_set_options(chart, &op);
        yy_report_add_chart(report, chart);
        yy_chart_free(chart);

        printf("[OK]\n");
    }

    // export report to html
    bool suc = yy_report_write_html_file(report, output_path);
    if (!suc) {
        printf("write report file failed: %s\n", output_path);
    }
    yy_report_free(report);
    free(vals);
}

static char * null_f64_to_str(double val, char *buf) {return buf;}
static char * null_f32_to_str(float val, char *buf) {return buf;}

/*
 This benchmark is somewhat unfair, because different algorithm use different
 format to print floating point number.

 For example:
 david_gay:     1.1e-06     11
 google:        0.0000011   11
 swift:         1.1e-06     11.0
 fmt:           1.1e-06     11.0
 ryu:           1.1E-6      1.1E1
 dragonbox:     1.1E-6      1.1E1

 Maybe we should benchmark the "binary to decimal" and "decimal to string"
 separately for some algorithms.
 */
static void dtoa_func_register_all(void) {
#define dtoa_func_register(name) \
    extern char * name##_f64_to_str(double val, char *buf); \
    func_arr[func_count] = name##_f64_to_str; \
    func_name_arr[func_count] = (char*)(#name); \
    func_count++; \
    if ((int)strlen(#name) > func_name_max) func_name_max = (int)strlen(#name);

    dtoa_func_register(null) // no need to verify

    dtoa_func_register(dragonbox_comp)
    dtoa_func_register(dragonbox_full)
    dtoa_func_register(fmt_comp)
    dtoa_func_register(fmt_full)
    dtoa_func_register(ryu)
    dtoa_func_register(schubfach)
    dtoa_func_register(schubfach_xjb)
    dtoa_func_register(yy_double)
    dtoa_func_register(yyjson)
    dtoa_func_register(xjb64)
    dtoa_func_register(xjb64_comp)
    //dtoa_func_register(schubfach_vitaut)
    dtoa_func_register(zmij)
    dtoa_func_register(jnum)
    //dtoa_func_register(d2e_xjb)
    dtoa_func_register(uscalec)



    // dtoa_func_register(null) /* used to meansure the benchmark overhead */
    // dtoa_func_register(david_gay)
    // dtoa_func_register(google)
    // dtoa_func_register(swift)
    // dtoa_func_register(fmtlib)
    // dtoa_func_register(fpconv)
    // dtoa_func_register(grisu3)
    // dtoa_func_register(schubfach)
    // dtoa_func_register(erthink)
    // dtoa_func_register(grisu_exact)
    // dtoa_func_register(dragonbox)
    // dtoa_func_register(ryu_mod)
    // dtoa_func_register(yy)
    // dtoa_func_register(xjb64)

// #ifndef _MSC_VER
//     dtoa_func_register(milo)
//     dtoa_func_register(emyg)
//     dtoa_func_register(ryu)
// #endif

// #ifdef HAVE_SSE2
//     dtoa_func_register(xjb_sse)
// #endif

// #ifdef HAVE_AVX512
//     dtoa_func_register(xjb_avx512)
// #endif

    // dtoa_func_register(printf) // not shortest, too slow
}

static void ftoa_func_register_all(void) {
    #define ftoa_func_register(name) \
        extern char * name##_f32_to_str(float val, char *buf); \
        func_arr_float[func_count_float] = name##_f32_to_str; \
        func_name_arr_float[func_count_float] = (char*)(#name); \
        func_count_float++; \
        if ((int)strlen(#name) > func_name_max_float) func_name_max_float = (int)strlen(#name);

        ftoa_func_register(null) // no need to verify

        ftoa_func_register(ryu)
        ftoa_func_register(schubfach)
        ftoa_func_register(schubfach_xjb)
        ftoa_func_register(xjb32)
        ftoa_func_register(xjb32_comp)
        ftoa_func_register(yyjson)
        ftoa_func_register(dragonbox_comp)
        ftoa_func_register(dragonbox_full)
        ftoa_func_register(fmt_comp)
        ftoa_func_register(fmt_full)
        ftoa_func_register(zmij)
        ftoa_func_register(jnum)
        //ftoa_func_register(f2e_xjb)

    }

void benchmark_double(const char *output_file_path) {
    dtoa_func_register_all();
    printf("------[verify]---------\n");
    dtoa_func_verify_all();
    printf("------[benchmark]------\n");
    dtoa_func_benchmark_all(output_file_path);
    printf("------[finish]---------\n");
    dtoa_func_cleanup();
    return;
}

void benchmark_float(const char *output_file_path) {
    ftoa_func_register_all();
    printf("------[verify]---------\n");
    printf("To be completed.\n");
    // ftoa_func_verify_all();
    printf("------[benchmark]------\n");
    ftoa_func_benchmark_all(output_file_path);
    printf("------[finish]---------\n");
    ftoa_func_cleanup();
    return;
}
