icpx:
	icpx -O3 -march=native -S ftoa.cpp -o ftoa_icpx.s
clang:
	clang++ -O3 -march=native -S ftoa.cpp -o ftoa_clang.s
gcc: 
	g++ -O3 -march=native -S ftoa.cpp -o ftoa_gcc.s

all: icpx clang gcc

clean:
	rm -f ftoa_icpx.s ftoa_clang.s ftoa_gcc.s