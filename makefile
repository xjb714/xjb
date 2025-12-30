example:
	g++ -o  example example.cpp
	./example
test:
	g++ -std=c++11 -march=native -o  test test.cpp -lgtest -Lbench/util/gtest/build
	./test