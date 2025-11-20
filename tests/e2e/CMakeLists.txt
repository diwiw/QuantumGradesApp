cmake_minimum_required(VERSION 3.20)

add_executable(qga_tests_e2e
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main_e2e.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_cli_e2e.cpp    
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

set_target_properties(qga_tests_e2e PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_test(NAME e2e_cli COMMAND qga_tests_e2e)
