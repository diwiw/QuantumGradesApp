# src/strategy/CMakeLists.txt
target_sources(qga_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/BuyHold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MACrossover.cpp
)
