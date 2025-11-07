# ============================================================
# 🧩 qga_domain — domain models and backtesting engine
# ============================================================

file(GLOB DOMAIN_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backtest/*.cpp
)

add_library(qga_domain STATIC ${DOMAIN_SRC})

target_include_directories(qga_domain
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_domain
    PUBLIC
        qga_core
        qga_utils
)

target_compile_features(qga_domain PUBLIC cxx_std_23)
