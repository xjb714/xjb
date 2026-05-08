# all:
# 	gcc -O3 -o gmp_demo gmp_demo.cpp -lgmp

LINK = -lgmpxx -lgmp -I/opt/homebrew/Cellar/gmp/6.3.0/include -L/opt/homebrew/Cellar/gmp/6.3.0/lib
verify:
	clang++ -O3 -std=c++20 -fopenmp -o generic_verify generic_verify.cpp $(LINK)
	./generic_verify