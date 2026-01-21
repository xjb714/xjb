# xjb
xjb : a fast float to string algorithm.

This code is still being updated continuously, so it may not be the final version. Later, I will write documentation to explain in detail the function of each line of code and the proof process.

## algorithm correctness proof:  
xjb64、xjb64_comp:
f64 to string/decimal : please run `bench/main.cpp/check_double_to_string();` to check the correctness of the algorithm.  
The document explaining the working principle of the code is called `code_explain.ipynb`.The document is currently under preparation.  

xjb32、xjb32_comp:  
(1)f32 to string : please run `bench/main.cpp/check_all_float_number_to_string()` to check the correctness of the algorithm.  
(2)f32 to decimal : please run `bench/main.cpp/check_all_float_number_to_decimal()` to check the correctness of the algorithm.  
Test all positive single-precision floating-point numbers. The running time only takes about one minute.  

## note:
the code is still under development and may contain bugs. if you find any bugs, please let me know.  
**warning: The current code can only run on little-endian CPUs. The code for the big-endian CPU is currently under development. Support for the msvc compiler is under development.**

## src files:
(1)float/double to decimal algorithm  
&emsp;&emsp;   xjb32 : for float (IEEE754-binary32) ; `bench/xjb/xjb32_i.cpp`;  <!-- https://godbolt.org/z/eT57sW9q1   -->  
&emsp;&emsp;   xjb64 : for double(IEEE754-binary64) ; `bench/xjb/xjb64_i.cpp`;  <!-- https://godbolt.org/z/oG7eW9jEj   -->

(2)float/double to string algorithm  
&emsp;&emsp;   full lookup table : `bench/xjb/xjb.cpp` 
&emsp;&emsp;   compress lookup table : `bench/xjb/xjb_comp.cpp`

lookup table size:
<table border="1px">
    <tr>
        <td colspan="1"></td>
        <td colspan="1">full table:xjb.cpp</td>
        <td colspan="1">compress table:xjb_comp.cpp</td>
    </tr>
    <tr>
        <td>float</td>
        <td>1352 byte</td>
        <td>64 byte</td>
    </tr>
    <tr>
        <td>double</td>
        <td>15336 byte</td>
        <td>592 byte</td>
    </tr>
</table>

## examples:
Here are a few examples for double to string algorithm:
<table border="1px">
    <tr>
        <td colspan="1">double</td>
        <td colspan="1">print result</td>
    </tr>
    <tr>
        <td>123.45</td>
        <td>"123.45"</td>
    </tr>
    <tr>
        <td>1000</td>
        <td>"1000.0"</td>
    </tr>
    <tr>
        <td>123</td>
        <td>"123.0"</td>
    </tr>
    <tr>
        <td>123000</td>
        <td>"123000.0"</td>
    </tr>
    <tr>
        <td>1.2e23</td>
        <td>"1.2e23"</td>
    </tr>
    <tr>
        <td>1e100</td>
        <td>"1e100"</td>
    </tr>
    <tr>
        <td>0.0123</td>
        <td>"0.0123"</td>
    </tr>
    <tr>
        <td>0.001</td>
        <td>"0.001"</td>
    </tr>
    <tr>
        <td>0.000123</td>
        <td>"1.23e-04"</td>
    </tr>
    <tr>
        <td>0</td>
        <td>"0.0"</td>
    </tr>
    <tr>
        <td>NaN</td>
        <td>"nan"</td>
    </tr>
    <tr>
        <td>Inf</td>
        <td>"inf"</td>
    </tr>
</table>

<!-- demo link: https://onlinegdb.com/OPKdOpikG -->

## Algorithm characteristics:  
This algorithm is based on the schubfach algorithm.  
This algorithm is inspired by algorithms such as schubfach, yy, dragonbox, and grisu.
(1) Support double and float  
(2) Algorithm process optimization  
(3) Low branch prediction failure rate  
(4) Acceleration using SIMD instruction set  
(5) Low instruction dependency, high IPC  
(6) Fewer instructions  

## benchmark:
The latest benchmark results on Apple M1 and amd R7-7840h CPU:
<!-- ![ramdom float apple M1](bench_result/random_float_m1.svg)
![ramdom double apple M1](bench_result/random_double_m1.svg)
![ramdom float amd 7840h](bench_result/random_float_7840h.svg)
![ramdom double amd 7840h](bench_result/random_double_7840h.svg) -->
<table>
    <tr>
        <td ><center><img src="bench_result/random_float_m1.svg" >ramdom float on Apple M1 </br> compiler: apple clang 17.0.0</center></td>
        <td ><center><img src="bench_result/random_double_m1.svg"  >ramdom double on Apple M1</br> compiler: apple clang 17.0.0</center></td>
    </tr>
    <tr>
        <td><center><img src="bench_result/random_float_7840h.svg" >ramdom float on AMD R7-7840H </br> compiler: icpx 2025.0.4</center></td>
        <td ><center><img src="bench_result/random_double_7840h.svg"  >ramdom double on AMD R7-7840H</br> compiler: icpx 2025.0.4</center> </td>
    </tr>
</table>

please refer to `bench` directory for more details.  

## related algorithm

algorithm     | Description
--------------|-----------
[schubfach](https://github.com/abolz/Drachennest) | Raffaello Giulietti's algorithm.Origin source code from https://github.com/c4f7fcce9cb06515/Schubfach
schubfach_xjb  | It is improved by schubfach and has the same output result.
[ryu](https://github.com/ulfjack/ryu) | Ulf Adams's [Ryū algorithm](https://dl.acm.org/citation.cfm?id=3192369).
[dragonbox](https://github.com/jk-jeon/dragonbox) | Junekey Jeon's CPP implementation.
[fmt](https://github.com/fmtlib/fmt) | Victor Zverovich.https://github.com/fmtlib/fmt. version:12.1.0
[yy_double](https://github.com/ibireme/c_numconv_benchmark) | yy's algorithm ,  origin link : https://github.com/ibireme/c_numconv_benchmark/blob/master/vendor/yy_double/yy_double.c
[yy_json](https://github.com/ibireme/yyjson) | yy's algorithm. https://github.com/ibireme/yyjson
[teju](https://github.com/cassioneri/teju_jagua) | youtube  link : https://www.youtube.com/watch?v=fPZ1ZdA7Iwc . An academic paper will be written to provide proof of correctness.https://github.com/cassioneri/teju_jagua
[xjb](https://github.com/xjb714/xjb) | XiangJunBo's algorithm. https://github.com/xjb714/xjb
[schubfach_vitaut](https://github.com/vitaut/schubfach) | Vitaut's algorithm. https://github.com/vitaut/schubfach
[zmij](https://github.com/vitaut/zmij) | Vitaut's algorithm. https://github.com/vitaut/zmij
[jnum](https://github.com/lengjingzju/json) | Jing Leng's algorithm. https://github.com/lengjingzju/json

## acknowledgement:
Thanks to the following authors:

* Yaoyuan Guo ([@ibireme](https://github.com/ibireme)) - Author of the yyjson and yy_double algorithm , provide benchmark data and test code. The code inspiration for this project comes from the yy algorithm.

* Dougall Johnson ([@dougallj](https://github.com/dougallj)) - Authored the NEON implementation, which is used in the xjb algorithm.

* Daniel Lemire ([@lemire](https://github.com/lerime)) - Authored the AVX512IFMA implementation(Convert 8/16 digit integer to decimal strings), which is used in the xjb algorithm. [code](https://lemire.me/blog/2022/03/28/converting-integers-to-decimal-strings-faster-with-avx-512/)

* Raffaello Giulietti ([@rgiulietti](https://github.com/rgiulietti)) - Author of the Schubfach algorithm, whose work forms a foundational basis for xjb.

<!-- https://github.com/xjb714/f2dec_bench
![alt text](image-1.png)
![alt text](image-2.png) -->


<!-- 
**Outdated experimental results.**

https://github.com/xjb714/f2dec_bench benchmark result:  
Generate 1e7 random numbers without 0, NaN and Inf. Measure the time spent converting a floating-point number to a decimal result. We conducted tests on the following two processors.  
(1)x86-64  
![alt text](result/amd7840h/image.png)  
AMD R7 7840H(zen4) ; Ubuntu 24.04.3 LTS ; 24x2=48GB ddr5 5600MHZ;  
clang++: clang 18.1.3  
icpx : icpx 2025.0.4  
g++ : gcc 13.3  

**float:**  
1. g++ -O3 -march=native  
![alt text](result/amd7840h/image-1.png)  
1. icpx -O3 -march=native  
![alt text](result/amd7840h/image-2.png)  
1. clang++ -O3  -march=native  
![alt text](result/amd7840h/image-3.png)  
Under this condition, the xjb32 algorithm will be compiled into AVX-512 code.  
1. clang++ -O3  -march=native -mno-avx512f  
![alt text](result/amd7840h/image-4.png)  
Under this condition, the xjb32 algorithm will be compiled into AVX-2 code.  
1. clang++ -O3  -march=native -mno-avx512f -mno-avx2  
![alt text](result/amd7840h/image-5.png)  
Under this condition, the xjb32 algorithm will be compiled into normal code.  

**double:**  
1. g++ -O3 -march=native  
![alt text](result/amd7840h/image-6.png)  
2. icpx -O3 -march=native  
![alt text](result/amd7840h/image-7.png)  
3. clang++ -O3  -march=native  
![alt text](result/amd7840h/image-8.png)  
Under this condition, the xjb64 algorithm will be compiled into AVX-512 code.  
4. clang++ -O3  -march=native -mno-avx512f  
![alt text](result/amd7840h/image-9.png)  
Under this condition, the xjb64 algorithm will be compiled into AVX-2 code.  
5. clang++ -O3  -march=native -mno-avx512f -mno-avx2  
![alt text](result/amd7840h/image-10.png)  
Under this condition, the xjb64 algorithm will be compiled into normal code.  

(2)apple M1  
![alt text](result/m1/mmexport1761290732997.jpg)  
clang++ : Apple clang 17.0.0  
**float:**  
   clang++ -O3 -march=native  
   ![alt text](result/m1/mmexport1761290101583.jpg)  
**double:**  
   clang++ -O3 -march=native  
   ![alt text](result/m1/mmexport1761290105081.jpg)  
    -->


<!-- On a little-endian machine, the BCD code y of x, which ranges from 0 to 99, can be calculated as follows. On the big-endian machine, y should be equal to $a  \cdot  256 + b = x - 254 \cdot (x/10)$.
$$
\begin{aligned}
a \cdot 10+b &= x\\
a+b \cdot 256 &= y = a + (x - a \cdot 10)\cdot 256 = x \cdot 256 - (10 \cdot 256 - 1)  \cdot  (x/10)
\end{aligned}
$$


abcdefgh = x
y = a + b<<8 + c<<16 + d<<24 + e<<32 + f<<40 + g<<48 + h<<56

$$
\begin{aligned}
x &= abcdefgh \\
y &= a + (b<<8) + (c<<16) + (d<<24) + (e<<32) + (f<<40) + (g<<48) + (h<<56) \\
y &= a + (b<<8) + (c<<16) + (d<<24) + ((e + (f<<8) + (g<<16) + (h<<24))<<32) \\ 
y &= a + (b<<8) + ((c + (d<<8))<<16) + ((e + (f<<8) + ((g + (h<<8)) << 16))<<32) \\
\end{aligned}
$$

`u64 aabb_ccdd_merge = (x << 32) - ((10000ull<<32) - 1) * ((x * 109951163) >> 40);`
aabb_ccdd_merge = (efgh<<32)+abcd;

`u64 aa_bb_cc_dd_merge = (aabb_ccdd_merge << 16) - ((100ull<<16) - 1) * (((aabb_ccdd_merge * 10486) >> 20) & ((0x7FULL << 32) | 0x7FULL));`
aa_bb_cc_dd_merge = (((gh<<16)+ef)<<32) + ((cd<<16)+ab);

`u64 aabbccdd_BCD = (aa_bb_cc_dd_merge << 8) - ((10ull<<8) - 1) * (((aa_bb_cc_dd_merge * 103) >> 10) & ((0xFULL << 48) | (0xFULL << 32) | (0xFULL << 16) | 0xFULL));`
aabbccdd_BCD = y = a + (b<<8) + ((c + (d<<8))<<16) + ((e + (f<<8) + ((g + (h<<8)) << 16))<<32); -->