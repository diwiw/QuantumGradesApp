# 📊 QuantumGradesApp

> **Current Version:** `v0.6.0`  
> **Build System:** CMake  
> **IDE Support:** Visual Studio Code (fully configured)  
> **Documentation:** Auto-generated with Doxygen  
> **Unit Testing:** Doctest 

C++20 project evolving from a simple grades/statistics demo into a **quantitative backtesting framework** with clean modular design.

---


## Features (current – v0.6.x)

- **Config system**: JSON + ENV overrides, validation. // fully functional with release 0.7.0
- **Statistics**: basic metrics (avg, variance, percentiles).
- **FileManager**: simple I/O utilities.
- **Logger**: prepared for `spdlog` (coming in 0.7.0).
- **Domain models**:
  - Backtest: Engine, BarSeries, Portfolio, Orders, Trades, Execution.
  - Strategies: Buy & Hold, Moving Average Crossover.
- **Tests**: doctest-based, modular (separate test main).
- **Docs**: Doxygen + optional Graphviz diagrams.
- **Data**: sample CSVs in `/data/`.

---

## Roadmap


### v0.7.0
- Config JSON validation + error handling.
- **spdlog** integration (async, rolling file).
- Reporters (CSV/JSON).
- Data ingest (CSV/HTTP), SQLite persistence.

### v0.8.0
- CLI for scenario runs.
- Scenario profiles in Config.
- End-to-end flow tests.

### v0.9.0
- Metrics: MDD, Sharpe, Sortino, CAGR.
- clang-format/tidy integration in CI.

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
│ └─ backtest/
│ ├─ Engine.hpp
│ ├─ BarSeries.hpp
│ ├─ Order.hpp
│ ├─ Portfolio.hpp
│ ├─ Trade.hpp
│ └─ Execution.hpp
│ └─ strategy/
│ ├─ IStrategy.hpp
│ ├─ BuyHold.hpp
│ └─ MACrossover.hpp
├─ src/
│ └─ domain/
│ └─ backtest/
│ └─ strategy/
├─ tests/
│ ├─ test_main.cpp
│ └─ test_*.cpp
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

### Build & run
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```
Run example (Config):
```bash
./build/examples/examples_config
```
Run example (Backtest):
```bash
./build/examples/examples_backtest
```

### 🧪 Run Tests

```bash
./bin/tests
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

- [v0.6.0](changelog/release_notes_v0.6.0.md) – strategy framework, full backtest engine, domain model
- [v0.5.0](changelog/release_notes_v0.5.0.md) – CI/CD, CMake refactor, Logger, Statistics, Grades, FileManager 
- [v0.4.0](changelog/release_notes_v0.4.0.md) – CMake migration, VSCode setup
- [v0.3.0](changelog/release_notes_v0.3.0.md) – full Makefile-based system

---

## 🙌 Contributors

Project created and maintained by [diwiw](https://github.com/diwiw)
with focus on C++ mastery, quantitative programming, and CI/CD practices.

---

## ⚖️ License

MIT License – see [`LICENSE.txt`](LICENSE.txt)