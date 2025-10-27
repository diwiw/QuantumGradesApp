# src/persistence/CMakeLists.txt
target_sources(qga_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/DatabaseWorker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PersistenceFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SQLiteStore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Statement.cpp
)
