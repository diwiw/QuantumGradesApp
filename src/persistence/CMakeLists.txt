# ============================================================
# 💽 qga_persistence — SQLite database layer
# ============================================================

file(GLOB PERSIST_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_persistence STATIC ${PERSIST_SRC})

target_include_directories(qga_persistence
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_persistence
    PUBLIC
        qga_utils
        SQLite::SQLite3
)

target_compile_features(qga_persistence PUBLIC cxx_std_23)

set_target_properties(qga_persistence PROPERTIES
    OUTPUT_NAME "qga_persistence"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
