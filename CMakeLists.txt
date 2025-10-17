cmake_minimum_required(VERSION 3.20)
project(QuantumGradesApp VERSION 0.7.1 LANGUAGES CXX)

# ============================================================
# 🔬 Optional ThreadSanitizer support (for CI)
# ============================================================

option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

if (ENABLE_TSAN)
  message(STATUS "🔬 ThreadSanitizer enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer -O1")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=thread")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# === Build type ===
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# === Compiler flags ===
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# === clang ===
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Options ===
option(BUILD_DOCS "Build documentation with doxygen" OFF)

# === Directories ===
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
unset(EXAMPLES_DIR CACHE)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)


include_directories(${INCLUDE_DIR} ${EXTERNAL_DIR})

# === Generate Version.hpp ===
execute_process(
	COMMAND git describe --tags --always --dirty
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_VERSION
	OUTPUT_STRIP_TRAILING_WHITESPACE
)



# === Generate 'data' catalog in binary
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

# === Copying file from source to binary catalogue
file(COPY ${CMAKE_SOURCE_DIR}/data/readGrades.txt
		DESTINATION data)

string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

configure_file(
	${CMAKE_SOURCE_DIR}/cmake/version.h.in
	${INCLUDE_DIR}/Version.hpp
	@ONLY
)

# === Application sources (without main for tests) ===
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*/main\\.cpp$")

# === External libraries === === ===

include(FetchContent)

# === CURL (prefer vcpkg/system; fallback to source build) ===
find_package(CURL QUIET)

if (NOT CURL_FOUND)
    message(WARNING "libcurl not found — using portable source build (Windows CI fallback)")

    FetchContent_Declare(
        curl
        URL https://github.com/curl/curl/releases/download/curl-8_8_0/curl-8.8.0.tar.gz
    )
    FetchContent_MakeAvailable(curl)

    # Define alias only if missing (prevents 'ALIAS already exists' errors)
    if (NOT TARGET CURL::libcurl AND TARGET libcurl)
        add_library(CURL::libcurl ALIAS libcurl)
    endif()
else()
    message(STATUS "✅ CURL found (version: ${CURL_VERSION_STRING})")
    message(STATUS "    Include dirs: ${CURL_INCLUDE_DIRS}")
    message(STATUS "    Libraries   : ${CURL_LIBRARIES}")
endif()

# === Fix for Ubuntu CI runner include path ===
if (TARGET CURL::libcurl)
    get_target_property(_curl_includes CURL::libcurl INTERFACE_INCLUDE_DIRECTORIES)
    if (NOT EXISTS "/usr/include/curl" AND EXISTS "/usr/include/x86_64-linux-gnu/curl")
        message(WARNING "⚠️ Patching CURL::libcurl include path for GitHub Actions Ubuntu runner")
        set_property(TARGET CURL::libcurl PROPERTY INTERFACE_INCLUDE_DIRECTORIES "/usr/include/x86_64-linux-gnu/curl")
    endif()
endif()

# === Final validation ===
if (NOT TARGET CURL::libcurl)
    message(FATAL_ERROR "❌ No CURL::libcurl target — setup failed")
endif()

# JSON
FetchContent_Declare(
	nlohmann_json
	URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.tar.gz
)
FetchContent_MakeAvailable(nlohmann_json)

# === fmt ===
FetchContent_Declare(
  fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/10.2.1.tar.gz
)
FetchContent_MakeAvailable(fmt)

# === spdlog (via FetchContent, portable, with ext fmt) ===
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt library" FORCE)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1  # stabilna wersja
)
FetchContent_MakeAvailable(spdlog)

# === SQLite3 (via vcpkg or fallback) ===
find_package(SQLite3 REQUIRED)

if (SQLite3_FOUND)
    message(STATUS "✅ SQLite3 found")
    message(STATUS "    Include dirs: ${SQLite3_INCLUDE_DIRS}")
    message(STATUS "    Libraries   : ${SQLite3_LIBRARIES}")
else()
    message(FATAL_ERROR "❌ SQLite3 not found — please install it or use vcpkg")
endif()

# Add alias target if missing
if (NOT TARGET SQLite::SQLite3 AND TARGET SQLite::sqlite3)
    # Some systems name it sqlite3, not SQLite3
    add_library(SQLite::SQLite3 ALIAS SQLite::sqlite3)
elseif (NOT TARGET SQLite::SQLite3 AND TARGET sqlite3)
    add_library(SQLite::SQLite3 ALIAS sqlite3)
endif()



# === Optional Examples build ===
if(BUILD_EXAMPLES)
	add_executable(demo_db_logger_config ${SRC_FILES} ${EXAMPLES_DIR}/demo_db_logger_config.cpp)
	target_include_directories(demo_db_logger_config PRIVATE ${INCLUDE_DIR})
	target_link_libraries(demo_db_logger_config PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog SQLite::SQLite3)
	target_compile_definitions(demo_db_logger_config PRIVATE USE_CURL
		DATA_PATH=\"${CMAKE_SOURCE_DIR}/data\"
	)

	add_executable(examples_config ${SRC_FILES} ${EXAMPLES_DIR}/main_config_grades.cpp)
	target_include_directories(examples_config PRIVATE ${INCLUDE_DIR})
	target_link_libraries(examples_config PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog SQLite::SQLite3)
	target_compile_definitions(examples_config PRIVATE USE_CURL)

	add_executable(examples_backtest ${SRC_FILES} ${EXAMPLES_DIR}/main_backtest.cpp)
	target_include_directories(examples_backtest PRIVATE ${INCLUDE_DIR})
	target_link_libraries(examples_backtest PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog SQLite::SQLite3)
	target_compile_definitions(examples_backtest PRIVATE USE_CURL)
endif()

# === Tests ===
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)
list(FILTER TEST_FILES EXCLUDE REGEX ".*test_main.cpp$")

add_executable(tests ${SRC_FILES} ${TEST_FILES} ${TEST_DIR}/test_main.cpp)
target_include_directories(tests PRIVATE ${INCLUDE_DIR} ${EXTERNAL_DIR})
target_compile_definitions(tests PRIVATE UNIT_TEST USE_CURL)

# Make sure, that targets sees nlohmann_json
if(TARGET tests)
	target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog SQLite::SQLite3)
endif()


# === Doxygen ===
if(BUILD_DOCS)
	find_package(Doxygen QUIET)
	if (DOXYGEN_FOUND)
		set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
		set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs)

		add_custom_target(docs
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM
		)
	endif()
	if (NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen required when BUILD_DOCS=ON. Install doxygen or set BUILD_DOCS=OFF.")
	endif()
endif()

# === Clean-all ===
add_custom_target(clean-all
	COMMAND ${CMAKE_COMMAND} -E rm ${CMAKE_BINARY_DIR}/*	
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/data	
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/test_tmp_cfg_json
	COMMAND "Removing all build files and temp directories..."
)