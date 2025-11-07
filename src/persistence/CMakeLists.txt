# ============================================================
# 💽 qga_persistence — SQLite persistence and DB abstraction
# ============================================================

file(GLOB PERSIST_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_persistence STATIC ${PERSIST_SRC})

target_include_directories(qga_persistence
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_persistence
    PUBLIC
        qga_core
        qga_utils
        SQLite::SQLite3
)

target_compile_features(qga_persistence PUBLIC cxx_std_23)
