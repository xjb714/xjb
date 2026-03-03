g:
	g++ -O3 -march=native -Wall -Wextra -o main_gcc convert-neon.cpp
	./main_gcc
c:
	clang++ -O3 -march=native -Wall -Wextra -o main_clang convert-neon.cpp
	./main_clang
i:
	icpx -O3 -march=native -no-ftz -Wall -Wextra -o main_icpx convert-neon.cpp
	./main_icpx

# for intel compiler : to_string_sse4_1_v2 need add "-no-ftz" compile option



verify:verify.cpp
	g++ -march=native -o verify verify.cpp
	./verify
asm:
	clang++ -O3 -march=native -Wall -Wextra -S -o convert-neon.s convert-neon.cpp
	
clean:
	rm -f main verify