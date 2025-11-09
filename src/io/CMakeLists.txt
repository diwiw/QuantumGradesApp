# ============================================================
# 📤 qga_io — FileManager, DataExporter, network I/O
# ============================================================

file(GLOB IO_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_io STATIC ${IO_SRC})

target_include_directories(qga_io
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_io
    PUBLIC
        qga_utils
        fmt::fmt
        spdlog::spdlog
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

target_compile_features(qga_io PUBLIC cxx_std_23)

set_target_properties(qga_io PROPERTIES
    OUTPUT_NAME "qga_io"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
