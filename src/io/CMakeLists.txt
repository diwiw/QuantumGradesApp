# src/io/CMakeLists.txt
target_sources(qga_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/DataExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CsvLoader.cpp
)
