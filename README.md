# 📊 QuantumGradesApp

> **Current Version:** `v0.7.0`  
> **Build System:** CMake  
> **IDE Support:** Visual Studio Code (fully configured)  
> **Documentation:** Auto-generated with Doxygen  
> **Unit Testing:** Doctest 

C++20 project evolving from a simple grades/statistics demo into a **quantitative backtesting framework** with clean modular design.

---


## 🧠 Features (current – v0.7.0)

- **Config system**: JSON + ENV overrides, validation (fully functional)
- **Statistics**: basic metrics (avg, variance, percentiles)
- **FileManager**: lightweight I/O utilities
- **Logger**: integrated with async `spdlog`
- **DataIngest**: CSV + HTTP ingestion
- **Domain models**:
  - Backtest: `Engine`, `BarSeries`, `Portfolio`, `Orders`, `Trades`, `Execution`
  - Strategies: `Buy & Hold`, `Moving Average Crossover`
- **Tests**: `doctest`-based, modular with separate `test_main.cpp`
- **Docs**: Doxygen + optional Graphviz/Mermaid diagrams
- **CI**: Ubuntu (`ccache`), Windows (`sccache`), optional ThreadSanitizer build

---

## Roadmap


### v0.7.1
- Implement CSV/JSON Reporters.
- Add 'IReporter' interface and observer notifications.
- Archive legacy demos (examples/) and update Doxygen diagrams.
- Cleanup & refactor documentation structure.

### v0.8.0
- CLI for scenario runs.
- Scenario profiles in Config.
- End-to-end flow tests.
- Automated test validation scripts.

### v0.9.0
- Metrics: MDD, Sharpe, Sortino, CAGR.
- clang-format/tidy integration in CI.
- Add benchmark and profiling support.

### v1.x
- Stable MVP + REST API.
- Python bindings (pybind11).
- Desktop UI (Qt/QML) **or Web UI (React/Angular)**.
- API v2 (auth, pagination, OpenAPI).
- Observability & monitoring.

### v2.x
- Parallel backtest engine, GPU offload (CUDA/SYCL).
- Scaling: streams, mmap, checkpointing.
- Quantum SDK PoC strategy.
- ML pipeline integration.
- Benchmarks + fuzz/system tests.

### v3.0.0
- Mobile app (Android/iOS).
- Public benchmarks.
- Production-ready release.

---

## Architecture

To better understand the design of **QuantumGradesApp**, see the diagrams below:

- [Backtest Flow](docs/pages/backtest_flow.md)  
  Shows the step-by-step data flow inside the backtest engine  
  (BarSeries → Engine → Strategy → Execution → Portfolio → Result).

- [Class Diagram](docs/pages/class_diagram.md)  
  Shows the relationships between the main domain classes  
  (Engine, BarSeries, Portfolio, Orders/Trades, Strategies).

---

## 🧱 Project Structure

```bash
QuantumGradesApp/
├─ cmake/ # config, version.h.in, helpers
├─ data/ # sample input data
├─docs/
│ ├─ diagrams/
│ │ ├─ backtest_flow.dot
│ │ ├─ backtest_flow.mmd
│ │ ├─ class_diagram.dot
│ │ └─ class_diagram.mmd
│ ├─ backtest_flow.dox
│ ├─ backtest_flow.md
│ ├─ class_diagram.dox
│ └─ class_diagram.md
├─ external/ # third-party libraries (via FetchContent)
├─ include/
│ ├─ Config.hpp
│ ├─ FileManager.hpp
│ ├─ Logger.hpp
│ ├─ Statistics.hpp
│ └─ domain/
│    ├─ backtest/
│    │  ├─ Engine.hpp
│    │  ├─ BarSeries.hpp
│    │  ├─ Order.hpp
│    │  ├─ Portfolio.hpp
│    │  ├─ Trade.hpp
│    │  └─ Execution.hpp
│    └─ strategy/
│       ├─ IStrategy.hpp
│       ├─ BuyHold.hpp
│       └─ MACrossover.hpp
├─ src/
│  ├─ Config.cpp
│  ├─ Logger/
│  ├─ DataIngest/
│  ├─ FileManager.cpp
│  └─ domain/
│     ├─ backtest/
│     └─ strategy/
├─ tests/
│  ├─ test_main.cpp
│  ├─ test_data_ingest.cpp
│  ├─ test_logger.cpp
│  └─ data/test_http.csv
├─ CMakeLists.txt
├─ Doxyfile
├─ README.md
└─ LICENSE.txt
```

---

## 🚀 Getting Started

### 🔧 Build with CMake

```bash
## Build

### Requirements
- CMake ≥ 3.20
- C++20 toolchain
- (Optional) Doxygen for docs

### Build & Test
## Linux (Ubuntu)
```bash
sudo apt install cmake ninja-build pkg-config libcurl4-openssl-dev libsqlite3-dev libspdlog-dev libfmt-dev ccache
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```
## Windows (MSVC + Ninja + vcpkg)
cmake -S . -B build -G "Ninja" ^
  -DCMAKE_TOOLCHAIN_FILE=%CD%\vcpkg\scripts\buildsystems\vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows-static ^
  -DCMAKE_CXX_COMPILER_LAUNCHER=sccache
cmake --build build --config Release
ctest --test-dir build --output-on-failure

## Optional ThreadSanitizer (Linux only)
sudo apt install clang
cmake -S . -B build-tsan -DENABLE_TSAN=ON -DCMAKE_CXX_COMPILER=clang++
cmake --build build-tsan
ctest --test-dir build-tsan --output-on-failure

### 🧪 Run Tests

```bash
. /build/bin/tests
```
For testing HTTP in file test_data_ingest, there should be server locally started, before start test:

```bash
cd tests/data
python3 -m http.server 8000
```
---

## 📝 Doxygen Documentation

Generate HTML docs with:

```bash
cmake -S . -B build -DBUILD_DOCS=ON

cmake --build build --target docs
```

Output: `docs/html/index.html`

---

### 📊 UML Diagrams (optional)

To generate UML-style class diagrams with Doxygen, install [Graphviz](https://graphviz.org):

```bash
sudo apt install graphviz
```
Then rebuild the docs:
```
cmake --build build --target docs
```
## 🗒 Release Notes

- [v0.7.0](changelog/release_notes_v0.7.0.md) – dataIngest: CSV, HTTP, Persistence: SQLite3, JSON validation, Async spdlog, config/module struct
- [v0.6.0](changelog/release_notes_v0.6.0.md) – strategy framework, full backtest engine, domain model
- [v0.5.0](changelog/release_notes_v0.5.0.md) – CI/CD, CMake refactor, Logger, Statistics, Grades, FileManager 
- [v0.4.0](changelog/release_notes_v0.4.0.md) – CMake migration, VSCode setup
- [v0.3.0](changelog/release_notes_v0.3.0.md) – full Makefile-based system

---

## 📦 CI/CD Status

| Platform   | Build  | Cache             | Sanitizer                    |
| ---------- | ------ | ----------------- | ---------------------------- |
| 🐧 Ubuntu  | ✅     | `ccache`          | `ThreadSanitizer` (optional) |
| 🪟 Windows | ✅     | `sccache + vcpkg` | —                            |
| 📚 Docs    | ✅     | —                 | —                            |

[CI/CD](https://github.com/diwiw/QuantumGradesApp/actions/workflows/build.yml)
[Releases](https://github.com/diwiw/QuantumGradesApp/releases)

---

## 🙌 Contributors

Project created and maintained by [diwiw](https://github.com/diwiw)
with focus on C++ mastery, quantitative programming, and CI/CD practices.

---

## ⚖️ License

MIT License – see [LICENSE](https://github.com/diwiw/QuantumGradesApp/blob/main/LICENSE.txt)
