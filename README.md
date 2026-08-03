---
type: guide
---

# mini-lsm

A simple LSM storage engine.

## Toolchain

| Tool | Purpose |
| --- | --- |
| CMake 3.20 or newer | Configure and build the project |
| GCC 11 or newer | Compile, run sanitizers, and collect coverage |
| clang-format 14 or newer | Format C++ sources |
| GDB | Debug the executable from VS Code |
| Git | Download GoogleTest and Google Benchmark through CMake |
| gcovr | Generate the coverage report |

GoogleTest and Google Benchmark are downloaded automatically during CMake configuration.

Install the local dependencies in Ubuntu or WSL:

```bash
sudo apt update
sudo apt install -y cmake g++ clang clang-format gdb git gcovr
```

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/mini_lsm
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Format

Apply the repository's Google-based clang-format rules:

```bash
cmake --build build --target format
```

Check formatting without changing files:

```bash
cmake --build build --target format-check
```

## Benchmark

```bash
./build/mini_lsm_bench --benchmark_min_time=0.01s
```

## Sanitizers

Run AddressSanitizer and UndefinedBehaviorSanitizer:

```bash
cmake -S . -B build-asan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DMINI_LSM_BUILD_BENCHMARKS=OFF \
  -DMINI_LSM_ENABLE_ASAN_UBSAN=ON
cmake --build build-asan --target mini_lsm_test --parallel
ctest --test-dir build-asan --output-on-failure
```

Run ThreadSanitizer separately because it is incompatible with AddressSanitizer:

```bash
cmake -S . -B build-tsan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DMINI_LSM_BUILD_BENCHMARKS=OFF \
  -DMINI_LSM_ENABLE_TSAN=ON
cmake --build build-tsan --target mini_lsm_test --parallel
TSAN_OPTIONS=halt_on_error=1:detect_deadlocks=1 \
  ctest --test-dir build-tsan --output-on-failure
```

## Coverage

Coverage instrumentation requires GCC:

```bash
cmake -S . -B build-coverage \
  -DCMAKE_BUILD_TYPE=Debug \
  -DMINI_LSM_BUILD_BENCHMARKS=OFF \
  -DMINI_LSM_ENABLE_COVERAGE=ON
cmake --build build-coverage --target mini_lsm_test --parallel
ctest --test-dir build-coverage --output-on-failure
gcovr --root . --filter sources/ --xml --output coverage.xml
```

## VS Code

Open the repository through WSL, install the recommended extensions, and use the provided tasks for configure, build, test, and formatting. The debugger launches `build/mini_lsm` through GDB.

## Continuous integration

The CI workflow checks clang-format, builds the release executable, uploads benchmark output as an artifact, runs ASan/UBSan with coverage upload, and runs TSan.
