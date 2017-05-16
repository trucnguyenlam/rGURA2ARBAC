# Introduction
This directory contains source code for the translation from rGURA to ARBAC-URA

# Language Specification
Please refer to [examples/sample.txt](sample.txt)

# Installation
The tool has been successfully tested on Ubuntu 16.04 x64
and MacOS Sierra.
## Prerequisites
### Linux
- `uuid`, ```apt install uuid-dev```
- `cmake`, version 2.8+, ```apt install cmake```
- `g++` version 5.0+, ```apt install g++```

### MacOS
- `cmake`, version 2.8+, ```brew install cmake```
- `clang`, ```xcode-select --install```

## Compilation
```sh
mkdir build
cd build
cmake ..
make -j8    # assume that you want to build in parallel
```
The executable file `rGURAConverter` will be created in `build` directory.
*Note* for MacOS, please copy `build/locals/antl4cpp/lib/libantlr4-runtime.4.7.dylib` into `build` directory.

# Usage
Please launch `rGURAConverter` with `-h` option.

# Note
Please contact me on trucnguyenlam@gmail.com for any problem.

