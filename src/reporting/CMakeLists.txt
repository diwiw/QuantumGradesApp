# ============================================================
# 🧾 qga_reporting — Reporting, summaries, analytics output
# ============================================================

file(GLOB REPORT_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_reporting STATIC ${REPORT_SRC})

target_include_directories(qga_reporting
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_reporting
    PUBLIC
        qga_io
        qga_persistence
        qga_utils
)

target_compile_features(qga_reporting PUBLIC cxx_std_23)

set_target_properties(qga_reporting PROPERTIES
    OUTPUT_NAME "qga_reporting"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
