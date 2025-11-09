# ============================================================
# 📊 qga_ingest — Data ingestion from CSV/HTTP
# ============================================================

file(GLOB INGEST_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_ingest STATIC ${INGEST_SRC})

target_include_directories(qga_ingest
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_ingest
    PUBLIC
        qga_io
        qga_persistence
)

target_compile_features(qga_ingest PUBLIC cxx_std_23)

set_target_properties(qga_ingest PROPERTIES
    OUTPUT_NAME "qga_ingest"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
