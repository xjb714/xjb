
# with no-PIC
# /////////////////////////////
io:
	icpx -O3  -march=native -c -fno-PIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	icpx -O3  -march=native main.cpp -o main_i -static -L. -lftoa -fno-PIC
	sudo perf stat -d ./main_i

go:
	g++ -O3  -march=native -c -fno-PIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	g++ -O3  -march=native main.cpp -o main_g -static -L. -lftoa -fno-PIC
	sudo perf stat -d ./main_g

co:
	clang++ -O3  -march=native -c -fno-PIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	clang++ -O3  -march=native main.cpp -o main_c -static -L. -lftoa -fno-PIC
	sudo perf stat -d ./main_c
# /////////////////////////////

# with PIC
# /////////////////////////////
ip:
	icpx -O3  -march=native -c -fPIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	icpx -O3  -march=native main.cpp -o main_i -static -L. -lftoa
	sudo perf stat -d ./main_i

gp:
	g++ -O3  -march=native -c -fPIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	g++ -O3  -march=native main.cpp -o main_g -static -L. -lftoa
	sudo perf stat -d ./main_g

cp:
	clang++ -O3  -march=native -c -fPIC ../float_to_string/ftoa.cpp -o ftoa.o
	ar rcs libftoa.a ftoa.o
	clang++ -O3  -march=native main.cpp -o main_c -static -L. -lftoa
	sudo perf stat -d ./main_c
# /////////////////////////////





# generate assembly code
# /////////////////////////////
s:
	icpx    -O3  -std=c++20 -fno-PIC -S ../float_to_string/ftoa.cpp -o ftoa_icpx_no_PIC.s -march=native
	clang++ -O3  -std=c++20 -fno-PIC -S ../float_to_string/ftoa.cpp -o ftoa_clang_no_PIC.s -march=native
	g++     -O3  -std=c++20 -fno-PIC -S ../float_to_string/ftoa.cpp -o ftoa_gcc_no_PIC.s -march=native

spic:
	icpx    -O3  -std=c++20 -fPIC -S ../float_to_string/ftoa.cpp -o ftoa_icpx_PIC.s -march=native
	clang++ -O3  -std=c++20 -fPIC -S ../float_to_string/ftoa.cpp -o ftoa_clang_PIC.s -march=native
	g++     -O3  -std=c++20 -fPIC -S ../float_to_string/ftoa.cpp -o ftoa_gcc_PIC.s -march=native
# /////////////////////////////



clean:
	rm -f ./*.s ./main_i ./main_g ./main_c ./libftoa.a ./ftoa.o