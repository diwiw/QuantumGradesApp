cmake_minimum_required(VERSION 3.27)
project(QuantumGradesApp LANGUAGES CXX)

# === Load version information ===
include(${CMAKE_SOURCE_DIR}/cmake/version.cmake)

message(STATUS "🧩 QuantumGradesApp version: ${QGA_VERSION} (${QGA_GIT_VERSION})")
message(STATUS "🕓 Build date: ${QGA_BUILD_DATE}")

# ============================================================
# ⚙️ Build configuration
# ============================================================

option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(BUILD_DOCS "Build documentation with doxygen" OFF)
option(BUILD_LEGACY_DEMOS "Build legacy demo targets (grades_demo, logger_demo)" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ThreadSanitizer (optional for CI)
if (ENABLE_TSAN)
    message(STATUS "🔬 ThreadSanitizer enabled")
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer -O1)
    add_link_options(-fsanitize=thread)
endif()

include(CTest)


# ============================================================
# 🧩 Ensure build directories exist (sanity check)
# ============================================================
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/_deps)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ============================================================
# 📁 Directories
# ============================================================
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${INCLUDE_DIR} ${EXTERNAL_DIR})

# ============================================================
# 🧱 Auto-generate Version.hpp from template
# ============================================================

set(VERSION_HEADER ${CMAKE_BINARY_DIR}/generated/Version.hpp)
set(VERSION_TEMPLATE ${CMAKE_SOURCE_DIR}/cmake/templates/version.h.in)

add_custom_command(
    OUTPUT ${VERSION_HEADER}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated
    COMMAND ${CMAKE_COMMAND} -D SRC=${VERSION_TEMPLATE}
                             -D DST=${VERSION_HEADER}
                             -P ${CMAKE_SOURCE_DIR}/cmake/generate_version.cmake
    DEPENDS ${VERSION_TEMPLATE}
    COMMENT "📦 Generating Version.hpp from ${VERSION_TEMPLATE}"
)

add_custom_target(generate_version ALL DEPENDS ${VERSION_HEADER})
include_directories(${CMAKE_BINARY_DIR}/generated)

# ============================================================
# 🧱 Install CCache
# ============================================================

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(STATUS "ccache found: ${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
else()
  message(STATUS "ccache not found — building without compiler cache")
endif()

# ============================================================
# 📦 Dependencies
# ============================================================
include(FetchContent)

# fmt
FetchContent_Declare(
  fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/10.2.1.tar.gz
)
FetchContent_MakeAvailable(fmt)

# spdlog
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt library" FORCE)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# JSON
FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.tar.gz
)
FetchContent_MakeAvailable(nlohmann_json)

# CURL
find_package(CURL REQUIRED)

# SQLite3
find_package(SQLite3 REQUIRED)

# CLI
# CLI11 dependency (via vcpkg or fallback)
find_package(CLI11 CONFIG)
if(NOT CLI11_FOUND)
    message(WARNING "CLI11 not found in vcpkg — fetching manually.")
    include(FetchContent)
    FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.4.2
    )
    FetchContent_MakeAvailable(CLI11)
endif()

# ============================================================
# 🧩 Modular source subdirectories
# ============================================================
add_subdirectory(${SRC_DIR}/utils)        # qga_utils — lowest layer
add_subdirectory(${SRC_DIR}/core)         # qga_core depends on utils
add_subdirectory(${SRC_DIR}/io)           # depends on core, utils
add_subdirectory(${SRC_DIR}/persistence)  # depends on core, utils
add_subdirectory(${SRC_DIR}/ingest)       # depends on io, persistence
add_subdirectory(${SRC_DIR}/strategy)     # depends on core, utils
add_subdirectory(${SRC_DIR}/reporting)    # depends on io, persistence
add_subdirectory(${SRC_DIR}/domain)       # depends on core, strategy, persistence

# ============================================================
# 🧩 Dependency: CLI11 (for CLI argument parsing)
# ============================================================
include(FetchContent)
find_package(CLI11 QUIET)

if(NOT CLI11_FOUND)
    message(STATUS "CLI11 not found — fetching via FetchContent...")
    FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.4.2
    )
    FetchContent_MakeAvailable(CLI11)
endif()

# ============================================================
# 🧩 Command-Line Interface (CLI)
# ============================================================
add_subdirectory(${SRC_DIR}/cli)

# ============================================================
# 💡 Examples
# ============================================================
add_subdirectory(${EXAMPLES_DIR}/backtest_demo)

if(BUILD_LEGACY_DEMOS)
    add_subdirectory(${EXAMPLES_DIR}/grades_demo)
    add_subdirectory(${EXAMPLES_DIR}/logger_demo)
endif()

# ==============================================
# === Tests ===
# ==============================================
enable_testing()
add_subdirectory(tests)

# ============================================================
# 📘 Doxygen
# ============================================================
if(BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "📘 Generating API documentation with Doxygen"
        )
    else()
        message(FATAL_ERROR "❌ Doxygen required when BUILD_DOCS=ON.")
    endif()
endif()

# ============================================================
# 🧹 Clean target
# ============================================================
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMENT "🧹 Cleaning build artifacts..."
)
