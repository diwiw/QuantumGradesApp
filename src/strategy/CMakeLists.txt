# ============================================================
# 📈 qga_strategy — trading strategies (BuyHold, MACrossover, etc.)
# ============================================================

file(GLOB STRATEGY_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_strategy STATIC ${STRATEGY_SRC})

target_include_directories(qga_strategy
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_strategy
    PUBLIC
        qga_core
        qga_utils
        qga_domain
)

target_compile_features(qga_strategy PUBLIC cxx_std_23)
