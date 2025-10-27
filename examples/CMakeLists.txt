# examples/CMakeLists.txt
option(BUILD_EXAMPLES "Build example demos" ON)

if(BUILD_EXAMPLES)
    add_executable(demo_db_logger_config demo_db_logger_config.cpp)
    target_link_libraries(demo_db_logger_config PRIVATE qga_core)

    add_executable(examples_config main_config_grades.cpp)
    target_link_libraries(examples_config PRIVATE qga_core)

    add_executable(examples_backtest main_backtest.cpp)
    target_link_libraries(examples_backtest PRIVATE qga_core)
endif()
