# QuantumGradesApp

> **Version:** `v0.7.1`
> **Build System:** CMake + Ninja
> **Standard:** C++23
> **Documentation:** Doxygen + Mermaid + Graphviz
> **Testing:** doctest
> **Platforms:** Linux, Windows (MSVC+vcpkg)

QuantumGradesApp is a modular, extensible quantitative backtesting framework written in modern C++20.  
Originally built as a grades/statistics demo, it has evolved into a clean, layered architecture suitable for quantitative research, algorithmic trading, data ingestion, reporting, and performance‑oriented extensions (GPU/HPC).

---

# Features (v0.7.1)

### Core Functionality
- **Configuration System:** JSON + ENV overrides, schema validation, typed access.
- **Statistics Engine:** average, variance, percentiles; refactored into `core/`.
- **Data Ingestion:** CSV + HTTP; extensive validation and dedicated loaders.
- **Domain Model:** BarSeries, Engine, Strategy API, Orders, Trades, Portfolio, Execution, Position, Result, Quote.
- **Reporting System:** `IReporter`, `ReporterManager`, CSV/JSON reporters.
- **Persistence:** Async `DatabaseWorker`, `SQLiteStore`, migrations.
- **Logging:** Async `spdlog`, DI via `LoggerFactory`, `NullLogger`, `MockLogger`.
- **Utilities:** formatting, logger abstractions, platform helpers.

### Development Tooling
- **CI/CD:** Ubuntu + Windows pipelines, ccache/sccache acceleration.
- **TSAN Build:** Optional ThreadSanitizer instrumentation (Linux).
- **Documentation:** Doxygen, diagrams, clean navigation.
- **Examples:** runnable demos (`backtest_demo`, `logger_demo`, etc.).

---

# Architecture Overview

QuantumGradesApp uses a layered, modular architecture inspired by Clean Architecture.

````mermaid
flowchart TD
    A[BarSeries] --> B[Engine]
    B --> C[Strategy]
    C --> D[Execution]
    D --> E[Portfolio]
    E --> F[Result]
    F --> G[ReporterManager]
````

---

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
├─ .github/
├─ .vscode/
├─ build/
├─ changelog/
├─ config/
├─ data/
├─ docs/
│  ├─ diagrams/
│  ├─ *.md
│  └─ *.dox
├─ examples/
│  ├─ backtest_demo/
│  ├─ grades_demo/
│  ├─ logger_demo/
│  └─ CMakeLists.txt
├─ external/
│  └─ doctest.h
├─ include/
│  ├─ common/
│  ├─ core/
│  ├─ domain/
│  ├─ ingest/
│  ├─ io/
│  ├─ persistence/
│  ├─ reporting/
│  ├─ strategy/
│  └─ utils/
├─ logs/
├─ sql/migrations/
├─ src/
├─ tests/
├─ tools/
├─ vcpkg_triplets/
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

| Platform   | Build | Cache | Sanitizer |
|-----------|--------|--------|-------------|
| Ubuntu    | ✅     | ccache | TSAN (opt) |
| Windows   | ✅     | sccache + vcpkg | — |
| Docs      | ✅     | — | — |

---

## Contributors
Project created and maintained by https://github.com/diwiw.

---

# License
MIT License – see `LICENSE.txt`

