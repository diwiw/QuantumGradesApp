cmake_minimum_required(VERSION 3.21)
project(QuantumGradesApp VERSION 0.7.1 LANGUAGES CXX)

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
# 🧱 Version.hpp (auto-generated)
# ============================================================
execute_process(
    COMMAND git describe --tags --always --dirty
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/version.h.in
    ${INCLUDE_DIR}/core/Version.hpp
    @ONLY
)

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

# ============================================================
# 🧩 Modular source subdirectories
# ============================================================
add_subdirectory(${SRC_DIR}/core)
add_subdirectory(${SRC_DIR}/domain)
add_subdirectory(${SRC_DIR}/ingest)
add_subdirectory(${SRC_DIR}/io)
add_subdirectory(${SRC_DIR}/persistence)
add_subdirectory(${SRC_DIR}/reporting)
add_subdirectory(${SRC_DIR}/strategy)
add_subdirectory(${SRC_DIR}/utils)


# ============================================================
# 💡 Examples
# ============================================================
add_subdirectory(${EXAMPLES_DIR}/backtest_demo)

if(BUILD_LEGACY_DEMOS)
    add_subdirectory(${EXAMPLES_DIR}/grades_demo)
    add_subdirectory(${EXAMPLES_DIR}/logger_demo)
endif()

# ============================================================
# 🧪 Tests
# ============================================================
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)
list(FILTER TEST_FILES EXCLUDE REGEX ".*test_main.cpp$")

add_executable(tests
    ${TEST_FILES}
    ${TEST_DIR}/test_main.cpp
)

target_include_directories(tests PRIVATE ${INCLUDE_DIR})
target_link_libraries(tests PRIVATE
    qga_core
    qga_utils
    qga_io
    fmt::fmt
    spdlog::spdlog
    CURL::libcurl
    nlohmann_json::nlohmann_json
    SQLite::SQLite3
)
target_compile_definitions(tests PRIVATE UNIT_TEST USE_CURL)

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
