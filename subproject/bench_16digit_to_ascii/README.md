bolg : https://dougallj.wordpress.com/2022/04/01/converting-integers-to-fixed-width-strings-faster-with-neon-simd-on-the-apple-m1/  

src : https://gist.github.com/dougallj/b4f600ab30ef79bb6789bc3f86cd597a#file-convert-neon-cpp-L144-L169

This folder is designed for testing the performance comparison of converting unsigned integers within the range of $[0, 1e16)$ to ASCII values. 

Execute the `make` command to obtain the benchmark results.

date : 2025.12.18