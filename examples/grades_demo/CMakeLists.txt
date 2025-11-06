# ==============================================
# Grades Demo Example (using SpdLogger)
# ==============================================

file(GLOB_RECURSE APP_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(grades_demo
    ${APP_SRC}
    grades_demo.cpp
    Grades.cpp
)

target_include_directories(grades_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(grades_demo PRIVATE
    qga_logger
    qga_config
    qga_filemanager
    qga_statistics
    nlohmann_json::nlohmann_json
    CURL::libcurl
    spdlog::spdlog
    fmt::fmt
    SQLite::SQLite3
)

target_compile_definitions(grades_demo PRIVATE
    USE_CURL
    DATA_PATH=\"${CMAKE_SOURCE_DIR}/data\"
)

set_target_properties(grades_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
