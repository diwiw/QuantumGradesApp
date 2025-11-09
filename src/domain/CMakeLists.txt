# ============================================================
# 🌐 qga_domain — Backtest engine, portfolio logic
# ============================================================

file(GLOB DOMAIN_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backtest/*.cpp
)

add_library(qga_domain STATIC ${DOMAIN_SRC})

target_include_directories(qga_domain
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_domain
    PUBLIC
        qga_core
        qga_strategy
        qga_persistence
)

target_compile_features(qga_domain PUBLIC cxx_std_23)

set_target_properties(qga_domain PROPERTIES
    OUTPUT_NAME "qga_domain"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
