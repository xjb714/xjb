.PHONY: all_result_x64 all_result_arm64 clean check

BENCH_F_TO_STR = 1 # 0 : float/double to decimal , 1 : float/double to string

BENCH_DOUBLE = 1 # 0 : float, 1 : double

BENCH_FLOAT = 0 # 0 : float, 1 : double

GCC = g++
ICPX = icpx -no-ftz
CLANG = clang++


OPTD = -O3 -std=c++20 -march=native -DNDEBUG -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR)
OPTF = -O3 -std=c++20 -march=native -DNDEBUG -DBENCH_DOUBLE=$(BENCH_FLOAT) -DBENCH_STR=$(BENCH_F_TO_STR)

g:
	$(GCC) $(OPTD) main.cpp -o main_gcc_double
	$(GCC) $(OPTF) main.cpp -o main_gcc_float
	./main_gcc_double
	./main_gcc_float
i:
	$(ICPX) $(OPTD) main.cpp -o main_icpx_double
	$(ICPX) $(OPTF) main.cpp -o main_icpx_float
	./main_icpx_double
	./main_icpx_float
c:
	$(CLANG) $(OPTD) main.cpp -o main_clang_double
	$(CLANG) $(OPTF) main.cpp -o main_clang_float
	./main_clang_double
	./main_clang_float

if:
	$(ICPX) $(OPTF) main.cpp -o main_icpx_float
	./main_icpx_float
id:
	$(ICPX) $(OPTD) main.cpp -o main_icpx_double
	./main_icpx_double

cf:
	$(CLANG) $(OPTF) main.cpp -o main_clang_float
	./main_clang_float
cd:
	$(CLANG) $(OPTD) main.cpp -o main_clang_double
	./main_clang_double
gf:
	$(GCC) $(OPTF) main.cpp -o main_gcc_float
	./main_gcc_float
gd:
	$(GCC) $(OPTD) main.cpp -o main_gcc_double
	./main_gcc_double

# on x86-64
all_result_x64:
	echo "benchmarking float/double to string on x86-64, may cost a few minutes..."
	$(MAKE) g BENCH_DOUBLE=0 BENCH_F_TO_STR=1
	$(MAKE) i BENCH_DOUBLE=0 BENCH_F_TO_STR=1
	$(MAKE) c BENCH_DOUBLE=0 BENCH_F_TO_STR=1
	$(MAKE) g BENCH_DOUBLE=1 BENCH_F_TO_STR=1
	$(MAKE) i BENCH_DOUBLE=1 BENCH_F_TO_STR=1
	$(MAKE) c BENCH_DOUBLE=1 BENCH_F_TO_STR=1

# on apple M1
all_result_arm64:
	$(MAKE) g BENCH_DOUBLE=0 BENCH_F_TO_STR=1
	$(MAKE) g BENCH_DOUBLE=1 BENCH_F_TO_STR=1

check:
	$(CLANG) -O3 -std=c++20 -march=native check.cpp -o check
	./check

clean:
	rm -f main main_gcc main_clang main_icpx *.s *.o check main_gcc_double main_gcc_float main_clang_double main_clang_float main_icpx_double main_icpx_float
