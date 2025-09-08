cmake_minimum_required(VERSION 3.20)
project(QuantumGradesApp VERSION 0.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

if (WIN32)
	add_compile_definitions(
		WIN32_LEAN_AND_MEAN
		NOMINMAX
		_CRT_SECURE_NO_WARNINGS
	)
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
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
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
FetchContent_Declare(
	nlohmann_json
	URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.tar.gz
)
FetchContent_MakeAvailable(nlohmann_json)

# === HTTP Curl Library ===
find_package(CURL REQUIRED)

# === spdlog ===
find_package(spdlog REQUIRED)

# === spdlog ===
find_package(SQLite3 REQUIRED)



# === Optional Examples build ===
if(BUILD_EXAMPLES)
	add_executable(examples_config ${SRC_FILES} ${EXAMPLES_DIR}/main_config_grades.cpp)
	target_include_directories(examples_config PRIVATE ${INCLUDE_DIR})
	target_link_libraries(examples_config PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog sqlite3::sqlite3)
	target_compile_definitions(examples_config PRIVATE USE_CURL)

	add_executable(examples_backtest ${SRC_FILES} ${EXAMPLES_DIR}/main_backtest.cpp)
	target_include_directories(examples_backtest PRIVATE ${INCLUDE_DIR})
	target_link_libraries(examples_backtest PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog sqlite3::sqlite3)
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
	target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog sqlite3::sqlite3)
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