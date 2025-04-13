# Chip 8 Emulator
This is a Chip 8 Emulator. This project uses a CMake template originally created by Matt Morse, licensed under the MIT License.

### Prerequisites
* CMake >=3.1
* A C/C++ compiler (tested with gcc 7.5.0)
* Catch (v2.11.3 is included in `tests/`; overwrite with a more updated version)

### Compiling, installing and running your code
To compile the project, run the following in the project root:
```
    mkdir build
    cd build
    cmake ..
    make
```