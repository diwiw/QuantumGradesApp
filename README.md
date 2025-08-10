# 📊 QuantumGradesApp

![Build](https://github.com/diwiw/QuantumGradesApp/actions/workflows/build.yml/badge.svg)

A modular and extensible C++ application for managing student grades, calculating basic statistics, and demonstrating modern C++ design patterns (RAII, Singleton, stateless utility classes). The project is designed to evolve into a quantum data simulation and analysis framework.

> **Current Version:** `v0.4.0`  
> **Build System:** CMake  
> **IDE Support:** Visual Studio Code (fully configured)  
> **Documentation:** Auto-generated with Doxygen  
> **Unit Testing:** Doctest  

---

## ✨ Features

- Modular architecture using modern C++17.
- Grade management with safe file read/write (via `FileManager`).
- Basic statistics (mean, median, std. dev, min, max) via `Statistics`.
- Singleton-based logging system (`Logger`).
- Full test suite with `doctest` framework.
- Automated documentation with Doxygen (HTML output).
- Clean separation of `src/`, `include/`, `tests/`, and `data/`.
- Cross-platform build system with CMake (Release/Debug).
- Ready for CI/CD integration and future quantum extensions.

---

## 🧱 Project Structure

```bash
QuantumGradesApp/
├── CMakeLists.txt
├── include/
│   ├── FileManager.h
│   ├── Grades.h
│   ├── Logger.h
│   ├── Statistics.h
│   └── Version.h   # auto-generated
├── src/
│   ├── FileManager.cpp
│   ├── Grades.cpp
│   ├── Logger.cpp
│   ├── Statistics.cpp
│   └── main.cpp
├── tests/
│   ├── test_filemanager.cpp
│   ├── test_grades.cpp
│   ├── test_statistics.cpp
│   └── test_main.cpp
├── data/
│   └── readGrades.txt   # sample input
├── docs/
│   └── html/            # generated docs
├── changelog/
│   ├── release_notes_v0.3.0.md
│	└── release_notes_v0.4.0.md
├── LICENSE.txt
└── README.md
```

---

## 🚀 Getting Started

### 🔧 Build with CMake

```bash
# Create build directory
mkdir -p build && cd build

# Configure project
cmake ..

# Build application and tests
cmake --build . --target app
cmake --build . --target tests
```

### 🧪 Run Tests

```bash
./bin/tests
```

### 📂 Run Application

```bash
./bin/app
```

---

## 📝 Doxygen Documentation

Generate HTML docs with:

```bash
cd build
make docs
```

Output: `docs/html/index.html`

---

## 🗒 Release Notes

- [v0.4.0](changelog/release_notes_v0.4.0.md) – migrated to CMake, VSCODE support, automatic data file copy
- [v0.3.0](changelog/release_notes_v0.3.0.md) – full Makefile-based implementation with tests, modular code, Logger, Statistics

---

## ⚖️ License

MIT License – see [`LICENSE.txt`](LICENSE.txt)

---

## 💡 Roadmap

- ✅ v0.4.0 – CMake, IDE, Doxygen, release packaging  
- ⏳ v0.5.0 – CI/CD, GitHub Actions  
- ⏳ v1.0.0 – Quantum/Simulation extension, CLI interface  
- ⏳ v2.0.0+ – Python bindings, HPC integrations, ML analysis  

---

## 👤 Author

Maintained by [diwiw](https://github.com/diwiw)  
Contact: [GitHub Issues](https://github.com/diwiw/QuantumGradesApp/issues)