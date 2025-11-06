# ==============================================
# Examples collection for QuantumGradesApp
# ==============================================

add_subdirectory(backtest_demo)

if(BUILD_LEGACY_DEMOS)
    add_subdirectory(grades_demo)
    add_subdirectory(logger_demo)
endif()
