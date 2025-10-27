# src/utils/CMakeLists.txt
target_sources(qga_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LoggerFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpdLogger.cpp
)
