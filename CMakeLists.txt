cmake_minimum_required(VERSION 3.16)
project(QuantumGradesApp VERSION 0.4.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# === Options ===
option(BUILD_DOCS "Build documentation with doxygen" OFF)

# === Directories ===
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)

include_directories(${INCLUDE_DIR} ${EXTERNAL_DIR})

# === Generate Version.h ===
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

# === Configure version header file ===
configure_file(
	${CMAKE_SOURCE_DIR}/cmake/version.h.in
	${INCLUDE_DIR}/Version.h
	@ONLY
)


# === Include TOML++ ===
include(FetchContent)
FetchContent_Declare(
	tomlplusplus
	GIT_REPOSITORY https://github.com/marzer/tomplusplus.git
	GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
target_include_directories(${PROJECT_NAME} PRIVATE ${tomlplusplus_SOURCE_DIR}/include)

# === Application sources (without main for tests) ===
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*main.cpp$")

# === Application build ===
add_executable(app ${SRC_FILES} ${SRC_DIR}/main.cpp)
target_include_directories(app PRIVATE ${INCLUDE_DIR})

# === Tests ===
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)
list(FILTER TEST_FILES EXCLUDE REGEX ".*test_main.cpp$")

add_executable(tests ${SRC_FILES} ${TEST_FILES} ${TEST_DIR}/test_main.cpp)
target_include_directories(tests PRIVATE ${INCLUDE_DIR} ${EXTERNAL_DIR})

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
	COMMAND "Removing all build files..."
)
