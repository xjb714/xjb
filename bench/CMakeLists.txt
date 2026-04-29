cmake_minimum_required(VERSION 3.16)
project(bench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 选项变量
option(BENCH_DOUBLE "Use double instead of float" ON)
option(BENCH_F_TO_STR "Benchmark float/double to string" ON)

# 源文件
set(SRC main.cpp)

# 编译选项
add_compile_options(-O3 -march=native -DNDEBUG)

# 生成不同配置的可执行文件
add_executable(main_gcc_double ${SRC})
target_compile_definitions(main_gcc_double PRIVATE BENCH_DOUBLE=1 BENCH_STR=1)

add_executable(main_gcc_float ${SRC})
target_compile_definitions(main_gcc_float PRIVATE BENCH_DOUBLE=0 BENCH_STR=1)

add_executable(main_clang_double ${SRC})
target_compile_definitions(main_clang_double PRIVATE BENCH_DOUBLE=1 BENCH_STR=1)

add_executable(main_clang_float ${SRC})
target_compile_definitions(main_clang_float PRIVATE BENCH_DOUBLE=0 BENCH_STR=1)

# icpx 相关（如需支持icpx，可取消注释）
# add_executable(main_icpx_double ${SRC})
# target_compile_definitions(main_icpx_double PRIVATE BENCH_DOUBLE=1 BENCH_STR=1)
# target_compile_options(main_icpx_double PRIVATE -no-ftz)
# add_executable(main_icpx_float ${SRC})
# target_compile_definitions(main_icpx_float PRIVATE BENCH_DOUBLE=0 BENCH_STR=1)
# target_compile_options(main_icpx_float PRIVATE -no-ftz)

# check 测试
add_executable(check check.cpp)

# 清理目标
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "main main_gcc main_clang main_icpx *.s *.o check main_gcc_double main_gcc_float main_clang_double main_clang_float main_icpx_double main_icpx_float")

# 说明：
# 1. 运行不同可执行文件以获得不同配置的基准测试结果。
# 2. 可通过 `cmake -DBENCH_DOUBLE=ON/OFF -DBENCH_F_TO_STR=ON/OFF ..` 控制选项。
# 3. 如需支持icpx，请取消相关注释并确保环境有icpx。
