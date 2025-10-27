# src/domain/backtest/CMakeLists.txt
target_sources(qga_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/BarSeries.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Order.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Portfolio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Position.cpp
)
