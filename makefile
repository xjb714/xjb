example:
	g++ -march=native -o  example example.cpp
	./example

# test: # only on apple m1 can run this test
# 	g++ -std=c++11 -march=native -o  test test.cpp -lgtest -Lbench/util/gtest/build
# 	./test