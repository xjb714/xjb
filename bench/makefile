.PHONY: all_result_x64 all_result_arm64 clean check

BENCH_DOUBLE = 1 # 0 : float, 1 : double

BENCH_F_TO_STR = 1 # 0 : float/double to decimal , 1 : float/double to string

BENCH_FLOAT = 0 # 0 : float, 1 : double


# default compiler : gcc
# g:
# 	g++ -O3 -std=c++20 main.cpp util/yybench/src/yybench.cpp  -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_gcc
# 	./main_gcc
# i:
# 	icpx -O3 -std=c++20  main.cpp util/yybench/src/yybench.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_icpx
# 	./main_icpx
# c:
# 	clang++ -O3 -std=c++20  main.cpp util/yybench/src/yybench.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_clang
# 	./main_clang

# with -march=native for better performance
g:
	g++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_gcc_double
	g++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_FLOAT) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_gcc_float
	./main_gcc_double
	./main_gcc_float
i:
	icpx -O3 -std=c++20 -march=native -no-ftz main.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_icpx_double
	icpx -O3 -std=c++20 -march=native -no-ftz main.cpp -DBENCH_DOUBLE=$(BENCH_FLOAT) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_icpx_float
	./main_icpx_double
	./main_icpx_float
c:
	clang++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_clang_double
	clang++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_FLOAT) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_clang_float
	./main_clang_double
	./main_clang_float

if:
	icpx -O3 -std=c++20 -march=native -no-ftz main.cpp -DBENCH_DOUBLE=$(BENCH_FLOAT) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_icpx_float
	./main_icpx_float
id:
	icpx -O3 -std=c++20 -march=native -no-ftz main.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_icpx_double
	./main_icpx_double

cf:
	clang++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_FLOAT) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_clang_float
	./main_clang_float
cd:
	clang++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_clang_double
	./main_clang_double
gf:
	g++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_FLOAT) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_gcc_float
	./main_gcc_float
gd:
	g++ -O3 -std=c++20 -march=native main.cpp -DBENCH_DOUBLE=$(BENCH_DOUBLE) -DBENCH_STR=$(BENCH_F_TO_STR) -o main_gcc_double
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
	g++ -O3 -std=c++20 check.cpp -o check
	./check

clean:
	rm -f main main_gcc main_clang main_icpx *.s *.o check main_gcc_double main_gcc_float main_clang_double main_clang_float main_icpx_double main_icpx_float
