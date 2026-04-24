# HELLO, WORLD?

A tiny C++ terminal game about surviving interstellar first contact by translating some version of `hello world` for increasingly strange civilizations.

## What it is

You inspect each culture's protocol rules, assemble a transmission from phrase fragments, and try to avoid diplomatic disasters. The prototype is intentionally small and text-based, but the structure is ready to grow into a larger narrative or puzzle game.

## Files

- `main.cpp`: the playable prototype
- `CMakeLists.txt`: build file for CMake-compatible toolchains

## Build

### With CMake

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Then run:

```powershell
.\build\Release\hello_world_game.exe
```

Depending on your generator, the executable may also land at:

```powershell
.\build\hello_world_game.exe
```

### With MSVC

```powershell
cl /EHsc /std:c++17 main.cpp /Fe:hello_world_game.exe
```

### With MinGW g++

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o hello_world_game.exe
```

## Current status

Verified with Visual Studio 2022 (`cl`) on this PC. The prototype compiled and completed a scripted playthrough successfully.
