# Install

The library needs no installing: put `include` on your include path and build with C++23.

To build and test it here:

1. Install mingw g++ 13 or newer, CMake 3.28 or newer, Ninja, and LLVM for `clang-tidy` and `clang-format`.
2. Clone this repository.
3. Run `.\recipe.ps1 test`. The first run downloads GoogleTest, whose archive `CMakeLists.txt` pins by hash.

`recipe.ps1` looks for g++ in `C:\ProgramData\mingw64\mingw64\bin` and for LLVM in `C:\Program Files\LLVM\bin`, and adds those folders to its own PATH.

`.\recipe.ps1 build` compiles the demo and the tests into `build\`. `.\recipe.ps1 install` does nothing: there is no installer.

## Two standard headers it can do without

g++ 13 has neither `<print>` nor `<flat_map>`. Each is used when the compiler has it:

- `src/main.cpp` falls back on a `print` of its own, built on `std::format`. No header needs `<print>`.
- `include/flat_map_compat.hpp` declares `bookdb::flat_map`: an alias of `std::flat_map` where that exists, and a sorted vector of pairs where it does not.

`.devcontainer`, `.vscode`, `CMakePresets.json`, `conanfile.py`, `run_clang_format.sh` and `scripts/build_windows.ps1` are what the course and the old GitHub route left. No gate runs them.
