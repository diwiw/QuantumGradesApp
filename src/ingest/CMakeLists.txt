# ============================================================
# 📥 qga_ingest — CSV and HTTP data ingestion
# ============================================================

file(GLOB INGEST_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_ingest STATIC ${INGEST_SRC})

target_include_directories(qga_ingest
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_ingest
    PUBLIC
        qga_core
        qga_utils
        qga_io
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

target_compile_features(qga_ingest PUBLIC cxx_std_23)
