# ============================================================
# qga_io — FileManager, DataExporter, persistence helpers
# ============================================================

file(GLOB IO_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_io STATIC ${IO_SRC})

target_include_directories(qga_io
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_io
    PUBLIC
        qga_core
        qga_utils
        fmt::fmt
        spdlog::spdlog
        CURL::libcurl
)

target_compile_features(qga_io PUBLIC cxx_std_20)
