---
type: operation
---

# Development Operations

Run all commands from the repository root in Ubuntu or WSL.

## Prerequisites

| Tool | Purpose |
| --- | --- |
| CMake 3.20 or newer | Configure and build the project |
| GCC 11 or newer | Compile, run sanitizers, and collect coverage |
| clang-format 14 or newer | Format C++ sources |
| GDB | Debug the executable from VS Code |
| Git | Download GoogleTest and Google Benchmark through CMake |
| gcovr | Generate the coverage report |

GoogleTest and Google Benchmark are downloaded automatically during CMake configuration.

```bash
sudo apt update
sudo apt install -y cmake g++ clang clang-format gdb git gcovr
```

## Build and Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/mini_lsm
```

The executable exits successfully.

## Test

Configure and build the project before running the tests.

```bash
ctest --test-dir build --output-on-failure
```

CTest reports that all tests passed.

## Format

Configure the project before running either format target.

Apply the repository's Google-based clang-format rules:

```bash
cmake --build build --target format
```

Check formatting without changing files:

```bash
cmake --build build --target format-check
```

The format check exits without reporting replacements.

## Benchmark

Build the default configuration before running the benchmark:

```bash
./build/mini_lsm_bench --benchmark_min_time=0.01s
```

The benchmark prints timing results to standard output.

## Sanitizers

### AddressSanitizer and UndefinedBehaviorSanitizer

```bash
cmake -S . -B build-asan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DMINI_LSM_BUILD_BENCHMARKS=OFF \
  -DMINI_LSM_ENABLE_ASAN_UBSAN=ON
cmake --build build-asan --target mini_lsm_test --parallel
ctest --test-dir build-asan --output-on-failure
```

The test run completes without sanitizer diagnostics.

### ThreadSanitizer

ThreadSanitizer runs separately because it is incompatible with AddressSanitizer:

```bash
cmake -S . -B build-tsan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DMINI_LSM_BUILD_BENCHMARKS=OFF \
  -DMINI_LSM_ENABLE_TSAN=ON
cmake --build build-tsan --target mini_lsm_test --parallel
TSAN_OPTIONS=halt_on_error=1:detect_deadlocks=1 \
  ctest --test-dir build-tsan --output-on-failure
```

The test run completes without thread-safety diagnostics.

If WSL reports `ThreadSanitizer: unexpected memory mapping`, disable address randomization for that test process:

```bash
setarch "$(uname -m)" -R env \
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

The command creates `coverage.xml` in the repository root.

## VS Code

Open the repository through WSL and install the recommended extensions. Saving any C++ source, header, template implementation, or module file formats it through clangd using the repository's clang-format configuration. Use the provided tasks to configure, build, test, and format the project. The debugger launches `build/mini_lsm` through GDB.

## Continuous Integration

The CI workflow checks clang-format, builds the release executable, uploads benchmark output as an artifact, runs ASan/UBSan with coverage upload, and runs TSan.
