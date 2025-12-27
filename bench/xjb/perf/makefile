g:
	g++ main.cpp -o main_g -O3 -march=native 
	sudo perf stat -d ./main_g
i:
	icpx main.cpp -o main_i -O3 -march=native
	icpx main.cpp -o main_icpx.s -O3 -S -march=native
	sudo perf stat -d ./main_i
c:
	clang++ main.cpp -o main_c -O3 -march=native 
	sudo perf stat -d ./main_c