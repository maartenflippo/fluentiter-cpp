[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/MacOS/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Windows/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Ubuntu/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Style/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Install/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)

# FluentIter CPP

A fluent API around iterators, inspired by [Rust's iterators](https://doc.rust-lang.org/std/iter/trait.Iterator.html).
This repository gives a good project to work with C++ templates, C++20 concepts, and explore compare some performance 
differences between ranged loops, the STL algorithms and this implementation.

## Benchmarks
Here we can see some benchmarks of this style of iterator. I intend to expand upon them in the future. These results
are obtained using the `CMAKE_BUILD_TYPE=Release` build type.

```
Run on (8 X 2592 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 0.21, 0.06, 0.02
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_map_ranged_for_loop       1074 ns         1074 ns       642052
BM_Map_std_transform         1026 ns         1026 ns       719025
BM_Map_fluent_iter           1971 ns         1971 ns       357823
```

## Contributing

### Cloning the repository
Clone the repository by running:
```sh
git clone https://github.com/maartenflippo/fluentiter-cpp
```

To ensure a consistent code style, the [CMakeLists](./CMakeLists.txt) file defines a `fix-format` target. Run the
following command to format the code according to the style before committing changes:
```sh
cp hooks/pre-commit .git/hooks/pre-commit
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# or simply call the executable: 
./build/test/FluentIterTests
```

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -S test -B build/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details.

### Build the documentation
```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
# view the docs
open build/doc/doxygen/html/index.html
```

To build the documentation locally, you will need Doxygen, jinja2 and Pygments on installed your system.

### Build everything at once

The project also includes an `all` directory that allows building all targets at the same time.
This is useful during development, as it exposes all subprojects to your IDE and avoids redundant builds of the library.

```bash
cmake -S all -B build
cmake --build build

# run tests
./build/test/FluentIterTests
# format code
cmake --build build --target fix-format
# build docs
cmake --build build --target GenerateDocs
```

## License
This project is licensed under MIT.