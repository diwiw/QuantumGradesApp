# src/domain/CMakeLists.txt
target_sources(qga_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Statistics.cpp
)

# Dodaj podmoduł "backtest"
add_subdirectory(backtest)
