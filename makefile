example:example.cpp bench/xjb/dtoa_xjb_comp.cpp bench/xjb/dtoa_xjb64_xjb32.cpp
	g++ -march=native -o  example example.cpp
	./example

example2:example.cpp bench/xjb/dtoa_xjb_comp.cpp bench/xjb/dtoa_xjb64_xjb32.cpp
	g++ -o  example2 example.cpp
	./example2