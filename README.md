# GaMesh

This a project where I hope to play around with procedurally generating MESHes for GAmes (GAMESH).

---

## Building
CMake is used to build the project.
The project structure and cmake scripts were modeled after [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/).


### Dependencies
The following are used for building.
* CMake (3.17+ highly recommended)
* A compiler for C99 (tested with MinGW)
* Git
* OpenGL
* Doxygen (optional)

The following libraries are automatically installed when building.
* [GLFW](https://www.glfw.org/)
* [glad](https://github.com/Dav1dde/glad)
* [Nuklear GUI](https://github.com/Immediate-Mode-UI/Nuklear)
* [STB library](https://github.com/nothings/stb)
* [log.c](https://github.com/rxi/log.c)
* ptheads

### Building with CMake

To configure building in the directory `build`:

```bash
cmake -S . -B build
```
For use with MinGW use the corresponding generators `-G "MinGW Makefiles"`

To build:

```bash
cmake --build build
```

To build docs (requires Doxygen, output in `build/docs/html`):

```bash
cmake --build build --target docs
```

To use an IDE, such as Xcode:

```bash
cmake -S . -B xbuild -GXcode
cmake --open xbuild
```

## Testing

CTest is used for testing.
Tests are located in the `tests` directory.
Tests can be run directly with cmake with 

```bash
cmake --build build --target test
```

Alternatively to use CTest to run all tests`
```bash
CTest --test-directory tests
```

A subset of tests whose labels match a regex can be run using the `-L` option.
For example to run base tests
```bash
CTest --test-directory tests -L base
```

Or to run interactive tests
```bash
CTest --test-directory tests -L interactive
```