# ============================================================
# ♟ qga_strategy — Trading and analytical strategies
# ============================================================

file(GLOB STRATEGY_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_strategy STATIC ${STRATEGY_SRC})

target_include_directories(qga_strategy
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_strategy
    PUBLIC
        qga_core
        qga_utils
)

target_compile_features(qga_strategy PUBLIC cxx_std_23)

set_target_properties(qga_strategy PROPERTIES
    OUTPUT_NAME "qga_strategy"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
