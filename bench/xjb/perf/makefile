GCC = g++
ICPX = icpx
CLANG = clang++


# with no-PIC
# /////////////////////////////
io:
	$(ICPX) -O3  -march=native -c -fno-PIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	$(ICPX) -O3  -march=native main.cpp -o main_i -static -L. -lftoa -fno-PIC
	sudo perf stat -d ./main_i

go:
	$(GCC) -O3  -march=native -c -fno-PIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	$(GCC) -O3  -march=native main.cpp -o main_g -static -L. -lftoa -fno-PIC
	sudo perf stat -d ./main_g

co:
	$(CLANG) -O3  -march=native -c -fno-PIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	$(CLANG) -O3  -march=native main.cpp -o main_c -static -L. -lftoa -fno-PIC
	sudo perf stat -d ./main_c
# /////////////////////////////

# with PIC
# /////////////////////////////
ip:
	$(ICPX) -O3  -march=native -c -fPIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	$(ICPX) -O3  -march=native main.cpp -o main_i -static -L. -lftoa
	sudo perf stat -d ./main_i

gp:
	$(GCC) -O3  -march=native -c -fPIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	$(GCC) -O3  -march=native main.cpp -o main_g -static -L. -lftoa
	sudo perf stat -d ./main_g

cp:
	$(CLANG) -O3  -march=native -c -fPIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	$(CLANG) -O3  -march=native main.cpp -o main_c -static -L. -lftoa
	sudo perf stat -d ./main_c
# /////////////////////////////

mca: # machine code analysis
	clang++ -O3 -std=c++20 ../float_to_string/ftoa.cpp -S -o ftoa.s -march=native
	llvm-mca -march=aarch64 --all-stats ./ftoa.s > mca.txt --iterations=1000 --timeline --bottleneck-analysis

# generate assembly code
# /////////////////////////////
s:
	$(ICPX)    -O3  -std=c++20 -fno-PIC -S ../float_to_string/ftoa.cpp -o ftoa_icpx_no_PIC.s -march=native
	$(CLANG) -O3  -std=c++20 -fno-PIC -S ../float_to_string/ftoa.cpp -o ftoa_clang_no_PIC.s -march=native
	$(GCC)     -O3  -std=c++20 -fno-PIC -S ../float_to_string/ftoa.cpp -o ftoa_gcc_no_PIC.s -march=native

spic:
	$(ICPX)    -O3  -std=c++20 -fPIC -S ../float_to_string/ftoa.cpp -o ftoa_icpx_PIC.s -march=native
	$(CLANG) -O3  -std=c++20 -fPIC -S ../float_to_string/ftoa.cpp -o ftoa_clang_PIC.s -march=native
	$(GCC)     -O3  -std=c++20 -fPIC -S ../float_to_string/ftoa.cpp -o ftoa_gcc_PIC.s -march=native
# /////////////////////////////



clean:
	rm -f ./*.s ./main_i ./main_g ./main_c ./libftoa.a ./ftoa.o