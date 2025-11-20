# QuantumGradesApp

> **Version:** `v0.8.0`
> **Build System:** CMake + Ninja
> **Standard:** C++23
> **Documentation:** Doxygen + Mermaid + Graphviz
> **Testing:** doctest (unit + E2E)
> **Platforms:** Linux, Windows (MSVC+vcpkg)
> **Architecture:** Modular, layered, Clean Architecture

QuantumGradesApp is a modular, extensible quantitative backtesting framework written in modern C++.  
Originally built as a grades/statistics demo, it has evolved into a clean, layered architecture suitable for quantitative research, algorithmic trading, data ingestion, reporting, and performance‑oriented extensions (GPU/HPC).

---

# Prerequisites

To build v0.8.0, install the following depending on your platform.

### Linux (recommended)

```
sudo apt install -y \
  cmake ninja-build g++ pkg-config \
  libcurl4-openssl-dev libsqlite3-dev libspdlog-dev libfmt-dev \
  ccache graphviz doxygen
```
Optionally Clang for TSAN:
```
sudo apt install clang
```  
### Windows (MSVC + vcpkg REQUIRED)
- Install Visual Studio Build Tools or full Visual Studio (C++ Desktop).

- Install Ninja

- Clone vcpkg:
```
git clone https://github.com/microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
```
- Install dependencies:
```
vcpkg install fmt spdlog sqlite3 curl
```
- Enable toolchain when configuring:
```
-DCMAKE_TOOLCHAIN_FILE=%CD%/vcpkg/scripts/buildsystems/vcpkg.cmake
-DVCPKG_TARGET_TRIPLET=x64-windows-static
```

---

# Features (v0.8.0)

### CLI & Configuration
- Full CLI interface for executing backtests:
  - `--config <file>` (required)
  - `--input <csv>` (override JSON)
  - `--output <csv>` (override JSON)
- `config.json` supports:
  - `input.path`, `input.format`
  - `output.path`, `output.format`
- Environment override support (`QGA_*`).
- Integrated asynchronous logging.
- Deterministic E2E tests for CLI.

## Module Breakdown

### `core/`
Shared, foundational infrastructure:
- `Config` – JSON/ENV configuration
- `Statistics` – metrics calculations
- `Platform` – platform utilities
- `Version` – semantic versioning info

### `domain/`
Business logic and quantitative model:
- **backtest:** BarSeries, Engine, Execution, Portfolio, Order, Trade, Position, Result, Instrument, Quote
- **strategy:** IStrategy, Buy & Hold, Moving Average Crossover

### `ingest/`
Data acquisition layer:
- `DataIngest` orchestrates ingestion

### `io/`
Low-level file operations:
- `CsvLoader`
- `FileManager`
- `DataExporter`

### `persistence/`
Database abstraction:
- `IDataStore` (interface)
- `SQLiteStore`
- `Statement` wrapper
- `DatabaseWorker` (async)

### `reporting/`
Observer-based reporting:
- `IReporter`
- `ReporterManager`

### `utils/`
Technical utilities:
- `ILogger`, `SpdLogger`, `NullLogger`, `MockLogger`
- `LoggerFactory`

---

# Architecture Overview

````mermaid
flowchart TD
    A[BarSeries] --> B[Engine]
    B --> C[Strategy]
    C --> D[Execution]
    D --> E[Portfolio]
    E --> F[Result]
    F --> G[DataExporter]
````

---

# Class Diagram

See `docs/pages/class_diagram.md` for extended diagrams.

````mermaid
classDiagram
    class Engine
    class BarSeries
    class Strategy
    class Portfolio
    class Order
    class Trade
    class Execution

    Engine --> BarSeries
    Engine --> Strategy
    Portfolio --> Trade
    Portfolio --> Order
````

---

# Project Structure

```
QuantumGradesApp/
├─ .github/ # CI/CD pipelines (Ubuntu/Windows)
├─ build/ # Out-of-source build directory
├─ changelog/ # Version changelogs
├─ config/ # Runtime configuration profiles (dev/test/prod)
├─ data/ # Sample datasets (optional)
├─ docs/ # Doxygen, diagrams, developer notes
│ ├─ diagrams/
│ ├─ pages/
│ ├─ developer_notes.md
│ └─ *.md / .dox
├─ external/ # Third-party libs (header-only, vendored)
│ └─ doctest.h
├─ include/ # Public headers (installed by project)
│ ├─ common/
│ ├─ core/
│ ├─ domain/
│ ├─ ingest/
│ ├─ io/
│ ├─ persistence/
│ ├─ reporting/
│ ├─ strategy/
│ └─ utils/
├─ logs/ # App/runtime logs
├─ sql/
│ └─ migrations/ # SQLite schema migrations
├─ src/ # Implementation (.cpp)
│ ├─ cli/
│ ├─ core/
│ ├─ domain/
│ ├─ ingest/
│ ├─ io/
│ ├─ persistence/
│ ├─ reporting/
│ ├─ strategy/
│ └─ utils/
├─ tests/ # Unit / integration / E2E tests
│ ├─ e2e/
│ ├─ fixtures/
│ │ └─ e2e/
│ └─ test_.cpp
├─ tools/ # Internal tools / scripts
├─ vcpkg_triplets/ # Custom vcpkg triplets
├─ CMakeLists.txt
└─ LICENSE.txt
```

---

# Building the Project

## Linux (GCC/Clang)

```
sudo apt install cmake ninja-build pkg-config \
    libcurl4-openssl-dev libsqlite3-dev libspdlog-dev libfmt-dev ccache

cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

---

## Windows (MSVC + Ninja + vcpkg)

### Prerequisites
```
git clone https://github.com/microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
```
### Build
```
cmake -S . -B build -G "Ninja" ^
  -DCMAKE_TOOLCHAIN_FILE=%CD%/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows-static ^
  -DCMAKE_CXX_COMPILER_LAUNCHER=sccache

cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

---

## ThreadSanitizer (Linux only)

```
sudo apt install clang
cmake -S . -B build-tsan -DENABLE_TSAN=ON -DCMAKE_CXX_COMPILER=clang++
cmake --build build-tsan
ctest --test-dir build-tsan --output-on-failure
```

---

# Running Tests

### Full test suite
```
./build/bin/tests
```
### E2E tests (added in v0.8.0)
```
./build/bin/qga_tests_e2e
```

### HTTP-related tests require a local server:
```
cd tests/data
python3 -m http.server 8000
```

---

# Documentation (Doxygen)

```
cmake -S . -B build -DBUILD_DOCS=ON
cmake --build build --target docs
```
Output: `docs/html/index.html`

Install Graphviz for UML diagrams:
```
sudo apt install graphviz
```

---

# CI/CD Status

> **Note:**  
> As of release **v0.8.0**, CI builds fail on both Ubuntu and Windows due to  
> ongoing refactors in **Config**, **profiles**, **HTTP ingest**, and  
> test-suite alignment. The build itself succeeds, but several tests do not.

| Platform   | Build | Cache | Tests | Notes |
|-----------|--------|--------|--------|--------|
| **Ubuntu** | ⚠️ Builds, ❌ Tests | ccache | ❌ | Failing profile tests, HTTP ingest tests, CLI tests (`--config required`) |
| **Windows** | ⚠️ Builds, ❌ Tests | sccache + vcpkg | ❌ | Test binaries not discovered (`tests.exe` / `qga_tests_e2e.exe` not generated or path mismatch). |
| **Docs** | ✅ | — | — | Doxygen builds correctly |

### ❌ Why CI currently fails

#### Ubuntu
- New Config fields: `input`, `output`, `input.path`, `output.path` not handled by older tests  
- Profile-based configs (`dev/test/prod`) changed → profile tests expect old schema  
- HTTP ingest test tries to reach a server not available on GitHub Actions  
- CLI tests now require `--config`, causing doctest auto-discovery failures  
- Some Stats tests expect old behavior (e.g. mean/median error messages)

#### Windows
- Build succeeds, but CTest **cannot find test executables**: 
- - Could not find executable .../build/bin/tests.exe
- - Could not find executable .../build/bin/qga_tests_e2e.exe
- This is due to Ninja + MSVC generating binaries in:
- - build/Debug/tests.exe
- - build/Debug/qga_tests_e2e.exe
but current CTest config still expects:
- - build/bin/tests.exe
- - build/bin/qga_tests_e2e.exe

This will be aligned in milestone **0.9.0** when the test suite is updated  
and CMake unified across platforms.

---

## Planned Fix (Milestone 0.9.0)

- 🔧 Update all Config/Profiles tests to new schema  
- 🔧 Add mock HTTP server for DataIngest HTTP tests  
- 🔧 Fix CLI tests for required `--config`  
- 🔧 Unify test binary output (`build/bin/` on Linux + Windows)  
- 🔧 Patch Windows CTest discovery  
- 🔧 Add missing fixtures for E2E tests  
- 🔧 Enable optional TSAN job again  

When fixed, CI table will return to:

| Platform   | Build | Cache | Sanitizer | Tests |
|-----------|--------|--------|-------------|--------|
| Ubuntu    | ✅ | ccache | TSAN (opt) | ✅ |
| Windows   | ✅ | sccache + vcpkg | — | ✅ |
| Docs      | ✅ | — | — | — |

---

## Contributors
Project created and maintained by https://github.com/diwiw.

---

# License
MIT License – see `LICENSE.txt`

