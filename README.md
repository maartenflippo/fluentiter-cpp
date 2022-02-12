[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/MacOS/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Windows/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Ubuntu/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Style/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)
[![Actions Status](https://github.com/maartenflippo/fluentiter-cpp/workflows/Install/badge.svg)](https://github.com/maartenflippo/fluentiter-cpp/actions)

# FluentIter CPP

A fluent API around iterators, inspired by [Rust's iterators](https://doc.rust-lang.org/std/iter/trait.Iterator.html).

## Contributing

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

The documentation is automatically built and [published](https://thelartians.github.io/ModernCppStarter) whenever a [GitHub Release](https://help.github.com/en/github/administering-a-repository/managing-releases-in-a-repository) is created.
To manually build documentation, call the following command.

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
./build/test/GreeterTests
# format code
cmake --build build --target fix-format
# run standalone
./build/standalone/Greeter --help
# build docs
cmake --build build --target GenerateDocs
```

## License
This project is licensed under MIT.