cmake_minimum_required(VERSION 3.27)

add_executable(qga_tests_e2e
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main_e2e.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_cli_e2e.cpp    
)

# ============================================================
# Provide path to CLI binary for E2E tests
# ============================================================

if (CMAKE_CONFIGURATION_TYPES)
    set(_cfg "$<CONFIG>")
else()
    set(_cfg "${CMAKE_BUILD_TYPE}")
endif()

set(QGA_CLI_PATH_FULL
    "${CMAKE_BINARY_DIR}/${_cfg}/bin/qga_cli${CMAKE_EXECUTABLE_SUFFIX}"
)

target_include_directories(qga_tests_e2e PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(qga_tests_e2e PRIVATE
    qga_core
    qga_domain
    qga_strategy
    qga_utils
    qga_io
    qga_ingest
    qga_cli_lib
    CLI11::CLI11    
)

# Inject CLI binary path into tests
target_compile_definitions(qga_tests_e2e PRIVATE
    QGA_CLI_PATH="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qga_cli"
)

add_test(NAME e2e_cli COMMAND qga_tests_e2e)
