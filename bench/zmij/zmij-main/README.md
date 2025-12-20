# Żmij

A double-to-string conversion algorithm based on [Schubfach](https://fmt.dev/papers/Schubfach4.pdf)
and [yy](https://github.com/ibireme/c_numconv_benchmark/blob/master/vendor/yy_double/yy_double.c)
with implementations in C and C++

## Features

* Round trip guarantee
* Shortest decimal representation
* Correct rounding
* High [performance](#performance)
* Fast [compile time](#compile-time)
* Negative zero dependencies
* Small, clean codebase consisting of one [source file](
  https://github.com/vitaut/zmij/blob/main/zmij.cc) and one [header](https://github.com/vitaut/zmij/blob/main/zmij.h)
* Permissive [license](https://github.com/vitaut/zmij/blob/main/LICENSE)

## Usage

```c++
#include "zmij.h"
#include <stdio.h>

int main() {
  char buf[zmij::buffer_size];
  zmij::dtoa(6.62607015e-34, buf);
  puts(buf);
}
```

## Performance

More than 3x faster than [Ryu](https://github.com/ulfjack/ryu) used by multiple
C++ standard library implementations and ~2x faster than
[Schubfach](https://github.com/vitaut/schubfach)
on [dtoa-benchmark](https://github.com/fmtlib/dtoa-benchmark) run on Apple M1.

| Function            | Time (ns) | Speedup |
|---------------------|----------:|--------:|
| ostringstream       | 871.982   | 1.00x   |
| sprintf             | 737.510   | 1.18x   |
| double-conversion   | 84.304    | 10.34x  |
| to_chars            | 42.786    | 20.38x  |
| ryu                 | 37.081    | 23.52x  |
| schubfach           | 24.885    | 35.04x  |
| fmt                 | 22.274    | 39.15x  |
| dragonbox           | 20.701    | 42.12x  |
| yy                  | 13.974    | 62.40x  |
| zmij                | 12.271    | 71.06x  |
| null                | 0.930     | 937.62x |

<img width="812" height="355" alt="image" src="https://github.com/user-attachments/assets/333f0575-8631-421b-8620-22b8a4ac9e0a" />

<img width="830" height="661" alt="image" src="https://github.com/user-attachments/assets/0e657107-fdc5-4575-bc04-4fd4ef4b0740" />

## Compile time

Compile time is ~60ms by default and ~68ms with optimizations enabled as measured by

```
% time c++ -c -std=c++20 zmij.cc [-O2]
```

taking the best of 3 runs.

## Other languages

* Rust: https://docs.rs/zmij/latest/zmij/index.html

## Differences from Schubfach

* 1 instead of 3 multiplications by powers of 10 in the common case
* Faster logarithm approximations
* Faster division and modulo
* Fewer conditional branches
* More efficient significand and exponent output
* Improved storage of powers of 10

## Name

Żmij (pronounced roughly zhmeey or more precisely /ʐmij/) is a Polish word that refers
to a mythical dragon- or serpent-like creature. This continues the dragon theme [started
by Steele and White](https://fmt.dev/papers/p372-steele.pdf). Nice feature of this name
is that it has a floating point in the first letter.
