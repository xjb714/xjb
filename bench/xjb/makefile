asm:
	icpx -O3 -S -march=native -o xjb_icpx.s xjb.cpp
	clang++ -O3 -S  -march=native -o xjb_clang.s xjb.cpp
	g++ -O3 -S -march=native -o xjb_gcc.s xjb.cpp
arm_asm:
	clang++ -O3 -S  -march=native -o xjb_arm_clang.s xjb.cpp
	clang++ -O3 -S  -march=native -o xjb_comp_arm_clang.s xjb_comp.cpp

clean:
	rm -f xjb_icpx.s xjb_clang.s xjb_gcc.s xjb_arm_clang.s xjb_arm_gcc.s xjb_comp_arm_clang.s