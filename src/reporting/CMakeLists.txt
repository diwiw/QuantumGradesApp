# ============================================================
# 📊 qga_reporting — reporters and observer interfaces
# ============================================================

file(GLOB REPORT_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(qga_reporting STATIC ${REPORT_SRC})

target_include_directories(qga_reporting
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(qga_reporting
    PUBLIC
        qga_core
        qga_utils
        qga_domain
        qga_io
)

target_compile_features(qga_reporting PUBLIC cxx_std_23)
