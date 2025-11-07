# ============================================================
# qga_utils — logging and diagnostic utilities
# ============================================================

file(GLOB UTILS_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_utils STATIC ${UTILS_SRC})

target_include_directories(qga_utils
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_utils
    PUBLIC
        fmt::fmt
        spdlog::spdlog
)

target_compile_features(qga_utils PUBLIC cxx_std_20)
