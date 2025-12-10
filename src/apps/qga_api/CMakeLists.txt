add_executable(qga_api
    main.cpp
)

target_include_directories(qga_api
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(qga_api
    PRIVATE
        qga_api_lib
        qga_core
        qga_utils
        fmt::fmt
)
