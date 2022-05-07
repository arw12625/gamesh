This is an example project using CMake.

The requirements are:

* CMake 3.17+ highly recommended.
* A C compiler
* Git
* OpenGL
* Doxygen (optional)

To configure:

```bash
cmake -S . -B build
```
For use with MinGW use the corresponding generators `-G "MinGW Makefiles"`

To build:

```bash
cmake --build build
```

To test (`--target` can be written as `-t` in CMake 3.15+):

```bash
cmake --build build --target test
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

