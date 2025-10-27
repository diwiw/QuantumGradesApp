# src/CMakeLists.txt
add_library(qga_core STATIC)
target_compile_features(qga_core PUBLIC cxx_std_20)

# Public include directory
target_include_directories(qga_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

# === Submodules ===
add_subdirectory(domain)
add_subdirectory(io)
add_subdirectory(ingest)
add_subdirectory(persistence)
add_subdirectory(reporting)
add_subdirectory(strategy)
add_subdirectory(utils)
