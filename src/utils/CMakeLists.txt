# ============================================================
# 🧰 qga_utils — Logging, diagnostics, and helpers
# ============================================================

file(GLOB UTILS_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_utils STATIC ${UTILS_SRC})

target_include_directories(qga_utils
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_utils
    PUBLIC
        fmt::fmt
        spdlog::spdlog
)

target_compile_features(qga_utils PUBLIC cxx_std_23)

set_target_properties(qga_utils PROPERTIES
    OUTPUT_NAME "qga_utils"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
