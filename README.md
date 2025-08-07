# QuantumGradesApp

**Version:** 0.3.0
**Description:** Modular C++ application for managing and analyzing grades. Includes logging system, statistics calculations, file management, and unit tests.

---

## Features

- **Grades Module** - add, store, and diesplay grades.
- **Logger** - thread-safe singelton logger with log levels.
- **Statistics** - mean, median, standard deviation, min, max calculations.
- **FileManager** - static class for safe file read/write/append operations.
- **Unit Tests** - implemented with [doctest](https://github.com/doctest/doctest) for all modules. 
- **Versioning** - version information embedded in the build (`Version.h`);

---

## Requirements

- **Compiler**: GCC 11+ or Clang 12+ (C++20 support)
- **Make**: GNU Make 4.3+
- **Doxygen** (optional, for documentation generation)
- **Git** (for version information in builds)

---

## Build & Run


### Build application:
`bash`
`make`

### Run application:
`./bin/app`

### Build and run tests:
`make run_tests`


## Project Structure

QuantumGradeApp/
├─ src/         # Source files (.cpp)
├─ include/     # Header files (.h)
├─ tests/       # Unit tests
├─ data/        # Sample data files
├─ logs/        # Application & test logs
├─ Makefile     # Build system
├─ external/    # External dependencies (doctest.h)
├─ Doxyfile     # Doxygen configuration
└─ License      # License file

## Testing

Unit tests are implemented for:
- Grades
- Logger
- Statistics
- FileManager
To run:
`make run_tests`

## Documentation

Generate with:
`make docs`
Output will be in `docs/html/index.html`

## Releases

This project uses Git tags for versioning.
To create a release:
```git tag -a v0.3.0 -m "Release 0.3.0 - integrated Statistics & FileManager"
git push origin v0.3.0```

## Licence

This project is licensed under the MIT License.



