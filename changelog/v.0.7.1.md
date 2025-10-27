# v0.7.1 – Reporter Foundations & CSV Ingest Stabilization (October 2025)

### 🚀 Overview
This release finalizes the ingestion stability work and introduces the foundation for the **reporting and observer system**.  
It also improves the async logging lifecycle, adds new interfaces for modularity, and enhances build configuration for demos.

---

### ✨ Added
- **`IReporter`** – abstract interface defining `onQuote`, `onTrade`, and `onSummary` callbacks.
- **`ReporterManager`** – coordinates multiple reporter instances and propagates events.
- **`flush()` method** in `ILogger`, implemented in `SpdLogger` for graceful shutdown.
- **Default constructor for `Instrument`** (enabled only under `UNIT_TEST`).
- **Demo dataset** `data/demo.csv` used by ingestion demo (`demo_db_logger_config`).

---

### 🔧 Changed
- **`DataIngest`**
  - Skips CSV header automatically.
  - Detects and parses both ISO 8601 (`%Y-%m-%dT%H:%M:%S`) and epoch-millisecond timestamps.
  - Improved exception safety and logging in `parseRow()`.
- **`Trade.cpp`** removed – constructor now defined inline in header.
- **`Portfolio`** refined for valuation consistency.
- **`SpdLogger`**
  - Added async `flush()` implementation.
  - Prevented “thread pool doesn’t exist anymore” warning during shutdown.
- **CMake**
  - Added `CMAKE_RUNTIME_OUTPUT_DIRECTORY` for cleaner binary organization.
- **Documentation**
  - Added Doxygen docs for `DataIngest`.
  - Updated `README.md` and `Version.hpp` to version **v0.7.1**.

---

### 🧪 Tests
- Added **`tests/test_reporter_manager.cpp`** to verify observer registration and event dispatch.
- Verified **demo pipeline**:
  - Successful CSV ingestion (6 bars)
  - Export to CSV and JSON
  - Clean async logger termination

---

### ✅ Example Output
Config log level: INFO
[info] Logger initialized with level INFO
[info] Ingested 6 bars
[info] DataExporter: successfully exported 6 bars to demo_out.csv
[info] Exported series to CSV and JSON
[info] Demo finished.


---

### 🧱 Internal Notes
This release completes milestone **v0.7.1** and closes issues:
- #119 – Implement CSV Reporter
- #120 – Add IReporter interface and observer logic

Next milestone: **v0.8.0 – Reporter Integration & Export Backends**
