# v0.7.1 – Infra + Validation + Logger + Demo Refactor

### Added
- `DataExporter` class for saving `BarSeries` to CSV files.
- Support for exporting full series and custom ranges.
- `BarSeries` now exposes `data()`, `operator[]`, `clear()`, and `empty()` methods.
- Unit tests for `DataExporter` covering append, range, and full export modes.
- Introduced modular demo targets: `grades_demo` and `logger_demo`, each built independently.
- Added Doxygen documentation for demo examples to preserve educational context.
- Auto-generation of `Version.hpp` from Git tags and build timestamp in CMake.

### Changed
- Migrated all demo components to the new production logger system (`ILogger`, `SpdLogger`, `LoggerFactory`).
- Removed legacy static `Logger` from the main build; kept only under `examples/` for reference.
- Updated `grades_demo` to use asynchronous logger instance (`logger_` via factory).
- Updated `logger_demo` to reflect production-style initialization with async sinks and config-based setup.
- Adjusted build configuration to exclude `examples/` from the main build by default.
- Unified naming, includes, and namespace consistency across demos and `utils`.

### Cleaned
- Removed outdated `include/Version.h` (replaced with generated `Version.hpp`).
- Simplified include paths and reduced coupling between modules.
- Legacy logging code (`utils/Logger`) kept only for archival purposes in `examples/`.
- Updated `.gitignore` to exclude generated files (`Version.hpp`, local build artifacts).

### Notes
- Legacy logging API remains for backward compatibility in demos only.
- Future milestone (`v0.7.2`) will focus on:
  - modular CMake restructuring (`core/`, `io/`, `utils/` targets),
  - final cleanup of legacy includes,
  - and expanded Doxygen documentation coverage.

> **Migration note:**  
> This release completes the internal transition from the legacy synchronous logger to the asynchronous, DI-based logging system.  
> Future versions will focus on modular CMake restructuring and core HPC/QGA logic.
