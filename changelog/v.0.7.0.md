# 🧩 v0.7.0 – Infra + Validation + Logger  
_Released: October 2025_

---

### ✅ Added
- **DataExporter** module for saving `BarSeries` to CSV  
  - Supports full export, partial range, and append mode
- **BarSeries** extended with:
  - `data()`
  - `operator[]`
  - `clear()`
  - `empty()`
- Unit tests for **DataExporter** covering range and append modes
- **ILogger** interface with production `SpdLogger`, `NullLogger`, and `MockLogger`
- **LoggerFactory** with async logging support (`spdlog::async_logger`)
- Configurable logging (pattern, sink, log level) via JSON **Config**
- Continuous Integration pipeline for:
  - 🐧 **Ubuntu (ccache)**
  - 🪟 **Windows (MSVC + Ninja + vcpkg + sccache)**
- **Full caching support for Windows builds**
  - Persistent compiler cache via `sccache`
  - `vcpkg` dependency caching (`installed`, `buildtrees`, `downloads`)
- **Optional ThreadSanitizer job** for concurrency testing  
  - Added `ENABLE_TSAN` CMake flag  
  - Runs via `ubuntu-tsan` job in CI  
  - Non-blocking validation of multithreaded modules
- **Artifact upload** for test/sample outputs (placeholder before full reports)

---

### 🔄 Changed
- Unified build scripts across Ubuntu/Windows platforms
- Improved **CMake toolchain** configuration and `vcpkg` detection  
- `DataIngest` unified to handle both CSV and HTTP sources
- Enhanced configuration handling (JSON validation + default values)
- Improved **logging consistency** and standardized log output format
- Better error propagation and validation in infrastructure modules
- Refined CI logs and diagnostic uploads (CMake/vcpkg logs on failure)

---

### 🧹 Internal / Infra
- MSVC environment activation via `ilammy/msvc-dev-cmd`
- Fallback mechanism for compiler detection on Windows CI
- Repository cleanup:
  - updated `.gitignore`
  - standardized directory structure  
- Prepared infrastructure for **artifact exports** (CSV/JSON reporters in v0.7.1)
- Added automated compiler/dependency caching (`ccache` / `sccache`)
- Added optional ThreadSanitizer integration (`ENABLE_TSAN`)

---

### ⚠️ Known Limitations
- Sample output artifacts are placeholders —  
  full report generation will be enabled in **v0.7.1** (`Reporters` + `Observer` pattern).

---

**Next milestone:** [v0.7.1 – Reporters & Observer Pattern](https://github.com/diwiw/QuantumGradesApp/milestone/4)  
**Compare changes:** [`v0.6.0...v0.7.0`](https://github.com/diwiw/QuantumGradesApp/compare/v0.6.0...v0.7.0)
