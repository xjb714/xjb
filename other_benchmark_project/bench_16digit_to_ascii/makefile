all:
	clang++ -O3 -march=native -Wall -Wextra -o main convert-neon.cpp
	./main
verify:verify.cpp
	g++ -O3 -march=native -o verify verify.cpp
	./verify