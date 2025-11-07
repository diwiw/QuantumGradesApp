# ============================================================
# qga_core — core logic (Config, Statistics)
# ============================================================

file(GLOB CORE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_core STATIC ${CORE_SRC})

target_include_directories(qga_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_core
    PUBLIC
        fmt::fmt
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        SQLite::SQLite3
)

target_compile_features(qga_core PUBLIC cxx_std_20)
