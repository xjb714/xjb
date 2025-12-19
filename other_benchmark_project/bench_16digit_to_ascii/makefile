all:
	clang++ -O3 -march=native -Wall -Wextra -o main convert-neon.cpp
	./main
verify:verify.cpp
	g++ -march=native -o verify verify.cpp
	./verify
asm:
	clang++ -O3 -march=native -Wall -Wextra -S -o convert-neon.s convert-neon.cpp
	
clean:
	rm -f main verify