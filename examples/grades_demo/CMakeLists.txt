# ==============================================
# Grades Demo Example
# ==============================================

set(GRADES_DEMO_SOURCES
    Grades.cpp
    grades_demo.cpp    
)

add_executable(grades_demo ${GRADES_DEMO_SOURCES})

target_include_directories(grades_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(grades_demo PRIVATE
    qga_logger
    qga_statistics
    qga_filemanager
    qga_config
    fmt::fmt
    spdlog::spdlog
    sqlite3
    CURL::libcurl
)


set_target_properties(grades_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

